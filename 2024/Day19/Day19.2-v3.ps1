[CmdletBinding()]
param()
Begin {
    function aoc2024_19_2_v3_preprocess_patterns {
        [CmdletBinding()]
        param(
            [Parameter(Mandatory)]
            [string]$Patterns
        )
        Process {
            $obj = [pscustomobject]@{
                Patterns = [System.Collections.ArrayList]::new(($Patterns -split ', ' | where-object { -not [string]::IsNullOrEmpty($_.Trim()) } | sort-object { $_.Length }))
            }
            $obj | Add-Member -MemberType ScriptProperty -Name Count -Value {$this.Patterns.Count}
            $obj | Add-Member -MemberType ScriptMethod -Name Contains -Value {param([Parameter(Mandatory)][string]$Pattern) $this.Patterns -contains $Pattern}
            $obj | Add-Member -MemberType ScriptProperty -Name SortedPatterns -Value {$this.Patterns|Sort-Object {$_.Length},{$_}}
            $obj | Add-Member -MemberType AliasProperty -Name Values -Value SortedPatterns
            $obj | Add-Member -MemberType ScriptProperty -Name MinLength -Value {$this.SortedPatterns | Select-Object -First 1 | Select-Object -ExpandProperty Length}
            $obj | Add-Member -MemberType ScriptProperty -Name MaxLength -Value {$this.SortedPatterns | Select-Object -Last 1 | Select-Object -ExpandProperty Length}
            $obj | Add-Member -MemberType ScriptMethod -Name ValuesByLength -Value {param([Parameter(Mandatory)][int]$Length) $this.SortedPatterns|Where-Object{$_.Length -eq $Length}}
            $obj | Add-Member -MemberType ScriptMethod -Name CountByLength -Value {param([Parameter(Mandatory)][int]$Length) $this.ValuesByLength($Length)|Measure-Object|Select-Object -ExpandProperty Count}

            $obj | Write-Output
        }
    }

    function aoc2024_19_2_v3_recurse_find_solutions {
        [CmdletBinding()]
        param(
            [Parameter(Mandatory)][string]$Needle,
            [Parameter(Mandatory)][ref]$Patterns,
            [Parameter(Mandatory)][ref]$Dictionary,
            [switch]$NoRecurse
        )
        Process {
            #Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:     '.PadRight(16,' ')
            #Write-Host -ForegroundColor Cyan $Needle.PadLeft(30,' ')

            if($Dictionary.Value.Keys -contains $Needle) {
                Write-Host -ForegroundColor Green -NoNewline 'DICTIONARY:'.PadRight(16,' ')
                Write-Host -ForegroundColor Green $Needle.PadLeft(30,' ')
                return $Dictionary.Value[$Needle]
            }
            if($Patterns.Value.Contains($Needle)) {
                Write-Host -ForegroundColor Green -NoNewline 'PATTERNS  :'.PadRight(16,' ')
                Write-Host -ForegroundColor Green $Needle.PadLeft(30,' ')
                $Dictionary.Value += @{
                    $Needle = [pscustomobject]@{
                        Pattern = $Needle
                        Parts = [System.Collections.ArrayList]::new()
                    }
                }
                $Items = [System.Collections.ArrayList]::new()
                $Items.Add($Needle) | Out-Null
                $Dictionary.Value[$Needle].Parts.Add($Items) | Out-Null
                return $Dictionary.Value[$Needle]
            }
            if($NoRecurse.IsPresent -and $NoRecurse) {
                # Finding child solutions by recursion is not wanted
                #Write-Host -ForegroundColor Gray -NoNewline '          :'.PadRight(16,' ')
                #Write-Host -ForegroundColor Gray 'Recursion not wanted!'.PadLeft(30,' ')
                return
            }

            1..($Needle.Length-1) | Foreach-Object {
                $i = $_
                
                $Left = $Needle.Substring(0, $i)
                $Right = $Needle.Substring($i)

                if([string]::IsNullOrEmpty($Left) -or [string]::IsNullOrEmpty($Right)) {
                    Write-Warning "Not found: $Needle for length $i"
                    return
                }

                $LeftSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Left -Patterns $Patterns -Dictionary $Dictionary -NoRecurse
                $RightSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Right -Patterns $Patterns -Dictionary $Dictionary
                if($LeftSolution -and $RightSolution) {
                    if($Dictionary.Value.Keys -notcontains $Needle) {
                        $Dictionary.Value += @{
                            $Needle = [pscustomobject]@{
                                Pattern = $Needle
                                Parts = [System.Collections.ArrayList]::new()
                            }
                        }
                    }
                    <#
                    $Items = [System.Collections.ArrayList]::new()
                    $Items.AddRange($LeftSolution.Parts) | Out-Null
                    $Items.AddRange($RightSolution.Parts) | Out-Null
                    $Dictionary.Value[$Needle].Parts.Add($Items) | Out-Null
                    #>
                    $LeftSolution.Parts |% {
                        $CurrentLeft = $_
                        $RightSolution.Parts |% {
                            $CurrentRight = $_
                            $Items = [System.Collections.ArrayList]::new()
                            $Items.AddRange($CurrentLeft) | Out-Null
                            $Items.AddRange($CurrentRight) | Out-Null

                            Write-Host -ForegroundColor Green -NoNewline 'NEW       :'.PadRight(16,' ')
                            Write-Host -ForegroundColor Magenta -NoNewline $Left.PadLeft(30 - $Right.Length,' ')
                            Write-Host -ForegroundColor Cyan -NoNewline $Right
                            Write-Host -NoNewline ' - '
                            Write-Host -ForegroundColor Magenta -NoNewline (($Items | Select-Object -First $CurrentLeft.Count)  -join '; ')
                            Write-Host -NoNewline ' ; '
                            Write-Host -ForegroundColor Cyan (($Items | Select-Object -Last $CurrentRight.Count)  -join '; ')

                            $Dictionary.Value[$Needle].Parts.Add($Items) | Out-Null
                        }
                    }

                }
            }
            # If the loop produced some outputs, return them
            if($Dictionary.Value.Keys -contains $Needle) {
                return $Dictionary.Value[$Needle]
            }
        }
    }

    # Load input data
    $DataFile = Import-PowerShellDataFile -Path "$PSScriptRoot\Day19.test.psd1"
    # $Patterns = [System.Collections.ArrayList]($DataFile.patterns -split ', ' | where-object { -not [string]::IsNullOrEmpty($_) })
    $Patterns = aoc2024_19_2_v3_preprocess_patterns -Patterns $DataFile.Patterns
    $Needles = [System.Collections.ArrayList]($DataFile.needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })

    # Progress Bar
    $Progress = [PSCustomObject]@{
        Current = 0
        Total = 1
        Params = @{
            Activity = 'AdventOfCode 2024-19.2'
            Status = 'In progress'
            CurrentOperation = 'Initializing'
            Id = 1
        }
    }
    $Progress | Add-Member -MemberType ScriptMethod -Name Reset -Value {param([ValidateScript({$_ -gt 0 })][int]$Max) $this.Total = $Max}
    $Progress | Add-Member -MemberType ScriptMethod -Name Step -Value {
        param($CurrentOperation)
        $this.Current ++
        $this.Params.CurrentOperation = $CurrentOperation
        $Params = $this.Params
        Write-Progress @Params -PercentComplete ($this.Current / $this.Total * 100)
    }
    $Progress | Add-Member -MemberType ScriptMethod -Name Done -Value {
        $Params = $this.Params
        Write-Progress @Params -Completed
    }
}
Process {
    $Dictionary = @{}
    $Needles | ForEach-Object -Process {
        $Progress.Step($_)

        Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:'.PadRight(16,' ')
        Write-Host -ForegroundColor Green "$_"
        aoc2024_19_2_v3_recurse_find_solutions -Needle $_ -Patterns ([ref]$Patterns) -Dictionary ([ref]$Dictionary)

    } -Begin { 
        $Progress.Reset($Needles.Count)
        $Total = 0
    } -End { 
        $Progress.Done()
        Write-Host "Total solutions found: $Total" -ForegroundColor Magenta
    }
}