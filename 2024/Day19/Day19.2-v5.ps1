#requires -Version 5.1
[CmdletBinding()]
param(
    [switch]$UseTestData
)
Begin {

    function aoc2024_19_2_v3_recurse_find_solutions {
        [CmdletBinding(DefaultParameterSetName='NoProgress')]
        param(
            [Parameter(Mandatory)][string]$Needle,
            [Parameter(Mandatory)][ref]$Patterns,
            [Parameter(Mandatory)][ref]$AntiPatterns,
            [Parameter(Mandatory)][int]$MaxPattern,
            [Parameter(Mandatory,ParameterSetName='WithProgress')][ref]$Progress,
            [switch]$NoRecurse
        )
        Process {
            # If the item was found in the AntiPatterns dictionary, do not bother checking any further
            if($AntiPatterns.Value.Contains($Needle)) {
                return
            }

            # If the item was found in the dictionary, return it as-is
            if(Test-DictionaryItem -Key $Needle) {
                $Item = Get-DictionaryItem -Key $Needle
                return $Item
            }

            # Else, if the item was found in the patterns, add that pattern to the dictionary
            # ... and keep on processing (no return yet), as even if the item was found in the patterns, there might exists other ways to compose it
            if($Patterns.Value.Contains($Needle)) {
                # Add the item to the dictionary
                New-DictionaryItem -Key $Needle
                $Items = [System.Collections.ArrayList]::new()
                $Items.Add($Needle) | Out-Null

                # Add the item values to the dictionary (if it does not exist)
                if(-not (Test-DictionaryItemValue -Key $Needle -ValueItem $Items)) {
                    New-DictionaryItemValue -Key $Needle -ValueItem $Items
                }

                # DO NOT RETURN the item. Keep on processing to look for other ways to compose the item
                # IN ADDITION, DISABLE $NoRecurse TO allow splitting further the "pattern" into smaller chunks
                $NoRecurse = $false
            }

            # Finding child solutions by recursion is not wanted, break
            if($NoRecurse.IsPresent -and $NoRecurse) {
                return
            }

            # Break the needle into two parts, and check if each are in the dictionary
            # Gradually increase the size of the left part, and decrease the size of the right part
            1..([Math]::min($MaxPattern,$Needle.Length-1)) | Foreach-Object {
            #1..($Needle.Length-1) | Foreach-Object {
                $i = $_

                # Break the needle into two parts
                $Left = $Needle.Substring(0, $i)
                $Right = $Needle.Substring($i)
                if([string]::IsNullOrEmpty($Left) -or [string]::IsNullOrEmpty($Right)) {
                    Write-Verbose "$Needle can't be broken into two parts on character [$i] (Length: $Needle.Length)"
                    return
                }

                # Print progress
                if($null -ne $Progress) {
                    $Progress.Value.SetStatus("$Left | $Right")
                    $Progress.Value.Refresh()
                }

                # Check if the left and right parts are in the dictionary
                $LeftSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Left -Patterns $Patterns -AntiPatterns $AntiPatterns -MaxPattern $MaxPattern -NoRecurse -Progress $Progress
                if(-not $LeftSolution) {
                    # If the left part has no solution, add it to the AntiPatterns, and return
                    $AntiPatterns.Value.Add($Left) | Out-Null
                    return
                }
                $RightSolution = aoc2024_19_2_v3_recurse_find_solutions -Needle $Right -Patterns $Patterns -AntiPatterns $AntiPatterns -MaxPattern $MaxPattern -Progress $Progress
                if(-not $RightSolution) {
                    # If the right part has no solution, add it to the AntiPatterns, and return
                    $AntiPatterns.Value.Add($Right) | Out-Null
                    return
                }
                
                # There was one (or more) solutions, add a new item to the dictionary
                if(-not (Test-DictionaryItem -Key $Needle)) {
                    New-DictionaryItem -Key $Needle
                }
                # Add the item's values to the dictionary
                $LeftSolution.Parts | Foreach-Object {
                    $CurrentLeft = $_
                    $RightSolution.Parts | Foreach-Object {
                        $CurrentRight = $_
                        $Items = [System.Collections.ArrayList]::new()
                        $Items.AddRange($CurrentLeft) | Out-Null
                        $Items.AddRange($CurrentRight) | Out-Null
                        # Add the item values to the dictionary (if it does not exist)                        
                        if(-not (Test-DictionaryItemValue -Key $Needle -ValueItem $Items)) {
                            New-DictionaryItemValue -Key $Needle -ValueItem $Items
                        }
                    }
                }
            }
            
            # If the loop produced some outputs, return them
            if((Test-DictionaryItem -Key $Needle)) {
                # Return the Item
                $Item = Get-DictionaryItem -Key $Needle
                return $Item
            } else {
                # Otherwise, log the AntiPattern
                $AntiPatterns.Value.Add($Needle) | Out-Null
            }
        }
    }
    
    # Initialization
    if($UseTestData.IsPresent -and $UseTestData) {
        $DataFilePath = "$PSScriptRoot\Day19.test.psd1"
    } else {
        $DataFilePath = "$PSScriptRoot\Day19.real.psd1"
    }
    $DataFile = Import-PowerShellDataFile -Path $DataFilePath
    Import-Module "$PSScriptRoot\AdventOfCode.2024.19.Dictionary\build\AdventOfCode.2024.19.Dictionary" -Force
    Reset-Dictionary

    # Load input data
    $Patterns = $DataFile.Patterns -split ', ' | ConvertTo-PatternList
    $AntiPatterns = [System.Collections.Generic.HashSet[string]]::new()
    $Needles = [System.Collections.ArrayList]($DataFile.needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })

    # Progress Bar
    $Progress = New-ProgressBarObject -Activity 'AdventOfCode 2024-19.2-v4'
}
Process {
    # First process all the patterns (in order to get a preliminary dictionary for faster handling of the needles)
    $Patterns.SortedPatterns | Foreach-Object -Process {
            $Progress.Step($_)
            aoc2024_19_2_v3_recurse_find_solutions -Needle $_ -Patterns ([ref]$Patterns) -AntiPatterns ([ref]$AntiPatterns) -MaxPattern $Patterns.MaxLength -Progress ([ref]$Progress) | Out-Null
        } -Begin { 
            $Progress.Reset($Patterns.SortedPatterns.Count)
        } -End { 
            $Progress.Done()
            Write-Host "Dictionary pre-initialized." -ForegroundColor Magenta
        }
    

    # Then process the actual needles and count solutions
    $Needles | Select-Object -First 1 | ForEach-Object -Process {
            $Needle = $_
            $Progress.Step($_)

            $Iterations = $Needle.Length - ($Patterns.MinLength )
            for($ijk = 1; $ijk -le $Iterations; $ijk++) {
                $Needle = $_
                
                $SubNeedle = $Needle.Substring($Needle.Length - ($Patterns.MinLength + $ijk), ($Patterns.MinLength + $ijk))

                Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:'.PadRight(16,' ')
                Write-Host -ForegroundColor Green "$SubNeedle - ($ijk. $Needle)"

                $Item = aoc2024_19_2_v3_recurse_find_solutions -Needle $SubNeedle -Patterns ([ref]$Patterns) -AntiPatterns ([ref]$AntiPatterns) -MaxPattern $Patterns.MaxLength -Progress ([ref]$Progress)
                if($Item) {
                    $Item | Format-DictionaryItem | Write-Host -ForegroundColor Green | Out-Null
                } else {
                    Write-Warning "No solution found for $SubNeedle"
                    break
                }
            }
        } -Begin { 
            $Progress.Reset($Needles.Count)
        } -End { 
            $Progress.Done()
            Write-Host "Dataset pre-processed for needle $Needle" -ForegroundColor Magenta
        }

    # Write-Warning "FINAL"
    # Then process the actual needles and count solutions
    $Needles | ForEach-Object -Process {
            $Progress.Step($_)
            Write-Host -ForegroundColor Yellow -NoNewline 'INPUT:'.PadRight(16,' ')
            Write-Host -ForegroundColor Green "$_"
            $Item = aoc2024_19_2_v3_recurse_find_solutions -Needle $_ -Patterns ([ref]$Patterns) -AntiPatterns ([ref]$AntiPatterns) -MaxPattern $Patterns.MaxLength -Progress ([ref]$Progress)
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

    [pscustomobject]@{d=Get-Dictionary;
        p=$Patterns;
        a=$AntiPatterns;
        n=$Needles;
        t=$Total}
}