#requires -Version 5.1
[CmdletBinding()]
param()
Begin {
    Import-Module .\AdventOfCode.2024.19.Dictionary\build\AdventOfCode.2024.19.Dictionary -MinimumVersion 1.2.3.0 -Force
    Reset-Dictionary
    
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
            [switch]$NoRecurse
        )
        Process {
            # Write-Warning "Needle: $Needle"

            # If the item was found in the dictionary, return it as-is
            if(Test-DictionaryItem -Key $Needle) {
                ## Print
                #Write-Host -ForegroundColor Cyan -NoNewline 'DICTIONARY:'.PadRight(16,' ')
                #Write-Host -ForegroundColor Cyan $Needle.PadLeft(30,' ')

                # Get the item from the dictionary
                $Item = Get-DictionaryItem -Key $Needle
                
                ## Print
                #$Item | Format-DictionaryItem | Write-Host -ForegroundColor DarkGray

                # Return the item
                return $Item
            }

            # Else, if the item was found in the patterns, add it to the dictionary
            # ... and keep on processing (no return yet), as even if the item was found in the patterns, there might exists other ways to compose it
            if($Patterns.Value.Contains($Needle)) {
                ## Print
                #Write-Host -ForegroundColor Magenta -NoNewline 'PATTERNS  :'.PadRight(16,' ')
                #Write-Host -ForegroundColor Magenta $Needle.PadLeft(30,' ')

                # Add the item to the dictionary
                New-DictionaryItem -Key $Needle
                $Items = [System.Collections.ArrayList]::new()
                $Items.Add($Needle) | Out-Null

                if(-not (Test-DictionaryItemValue -Key $Needle -ValueItem $Items)) {
                    # Add the item values to the dictionary (if it does not exist)
                    New-DictionaryItemValue -Key $Needle -ValueItem $Items
                }

                # Get the item from the dictionary
                $Item = Get-DictionaryItem -Key $Needle

                ## Print
                #$Item | Format-DictionaryItem | Write-Host -ForegroundColor DarkGray

                # DO NOT RETURN the item. Keep on processing to look for other ways to compose the item
                # <# return $Item #>
                # IN ADDITION, DISABLE $NoRecurse. It allows splitting further the "pattern" into smaller parts
                $NoRecurse = $false
            }

            if($NoRecurse.IsPresent -and $NoRecurse) {
                ## Print
                #Write-Host -ForegroundColor Red -NoNewline 'INVALID   :'.PadRight(16,' ')
                #Write-Host -ForegroundColor Red $Needle.PadLeft(30,' ')

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
                    Write-Verbose "Not found: $Needle for length $i"
                    return
                }

                $LeftSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Left -Patterns $Patterns -NoRecurse
                $RightSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Right -Patterns $Patterns 
                if($LeftSolution -and $RightSolution) {
                    ## Print
                    #Write-Host -ForegroundColor DarkGray -NoNewline 'ADD NEW   :'.PadRight(16,' ')
                    #Write-Host -ForegroundColor DarkGray $Needle.PadLeft(30,' ')

                    if(-not (Test-DictionaryItem -Key $Needle)) {
                        # Add a new item to the dictionary
                        New-DictionaryItem -Key $Needle
                    }
                    $LeftSolution.Parts |% {
                        $CurrentLeft = $_
                        $RightSolution.Parts |% {
                            $CurrentRight = $_

                            # Add the item values to the dictionary
                            $Items = [System.Collections.ArrayList]::new()
                            $Items.AddRange($CurrentLeft) | Out-Null
                            $Items.AddRange($CurrentRight) | Out-Null
                            
                            if(-not (Test-DictionaryItemValue -Key $Needle -ValueItem $Items)) {
                                # Add the item values to the dictionary (if it does not exist)
                                New-DictionaryItemValue -Key $Needle -ValueItem $Items
                            }
                        }
                    }
                    # Get the item from the dictionary
                    $Item = Get-DictionaryItem -Key $Needle

                    ## Print
                    #$Item | Format-DictionaryItem | Write-Host -ForegroundColor DarkBlue
                }
            }
            # If the loop produced some outputs, return them
            if((Test-DictionaryItem -Key $Needle)) {
                ## Print
                #Write-Host -ForegroundColor Green -NoNewline 'DICTIONARY:'.PadRight(16,' ')
                #Write-Host -ForegroundColor Green $Needle.PadLeft(30,' ')

                # Get the item from the dictionary
                $Item = Get-DictionaryItem -Key $Needle
                
                ## Print
                #$Item | Format-DictionaryItem | Write-Host -ForegroundColor DarkGray

                # Return the item
                return $Item
            }
        }
    }

    # Load input data
    $DataFile = Import-PowerShellDataFile -Path "$PSScriptRoot\Day19.real.psd1"
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
    $Progress | Add-Member -MemberType ScriptMethod -Name Reset -Value {param([ValidateScript({$_ -gt 0 })][int]$Max) $this.Total = $Max; $this.Current = 0}
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
    # First process all the patterns (in order to get a preliminary dictionary for faster handling of the needles)
    $Patterns.SortedPatterns | Foreach-Object -Process {
        $Progress.Step($_)

        #Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:'.PadRight(16,' ')
        #Write-Host -ForegroundColor Green "$_"
        $Item = aoc2024_19_2_v3_recurse_find_solutions -Needle $_ -Patterns ([ref]$Patterns)
        <#if($Item) {
            $Item | Format-DictionaryItem | Write-Host -ForegroundColor Green
        } else {
            Write-Warning "No solution found for $_"
        }#>
    } -Begin { 
        $Progress.Reset($Patterns.SortedPatterns.Count)
    } -End { 
        $Progress.Done()
        Write-Host "Dictionary pre-initialized." -ForegroundColor Magenta
    }

    # Then process the actual needles and count solutions
    $Needles | ForEach-Object -Process {
        $Progress.Step($_)

        Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:'.PadRight(16,' ')
        Write-Host -ForegroundColor Green "$_"
        $Item = aoc2024_19_2_v3_recurse_find_solutions -Needle $_ -Patterns ([ref]$Patterns)
        if($Item) {
            $Item | Format-DictionaryItem | Write-Host -ForegroundColor Green
            $Total += $Item.Parts.Count
        } else {
            Write-Warning "No solution found for $_"
        }

    } -Begin { 
        $Progress.Reset($Needles.Count)
        $Total = 0
    } -End { 
        $Progress.Done()
        Write-Host "Total solutions found: $Total" -ForegroundColor Magenta
    }
}