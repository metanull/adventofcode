#requires -Version 5.1
[CmdletBinding()]
param(
    [switch]$UseRealData,
    [switch]$Brute
)
Begin {
    # Initialization
    if($UseRealData.IsPresent -and $UseRealData) {
        if(-not (Test-Path -Path "$PSScriptRoot\Day19.real.psd1")) {
            throw "InputFile not found: $PSScriptRoot\Day19.real.psd1. Please create it, eventually by copying Day19.test.psd1 and modifying it."
        }
        $DataFilePath = "$PSScriptRoot\Day19.real.psd1"
    } else {
        $DataFilePath = "$PSScriptRoot\Day19.test.psd1"
    }
    $DataFile = Import-PowerShellDataFile -Path $DataFilePath
    Import-Module "$PSScriptRoot\AdventOfCode.2024.19.Dictionary\build\AdventOfCode.2024.19.Dictionary" -Force
    Reset-Dictionary

    $script:Ticks = 0

    Function Start-Solver {
        [cmdletbinding()]
        param(
            [Parameter(Mandatory)]
            [ValidateScript({Test-Path -Path $_ -PathType Leaf})]
            [string]$DataFilePath,
            [switch]$ResetDB,
            [switch]$Brute
        )
        Begin {
            $DataFile = Import-PowerShellDataFile -Path $DataFilePath
            $PatternList = $DataFile.Patterns -split ', ' | ConvertTo-PatternList
            $NeedleList = [System.Collections.ArrayList]($DataFile.needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })
            $DatabasePath = "$PSScriptRoot\Day19.db"
        }
        End {
            Write-Host "Total Ticks: $Ticks"
        }
        Process {
            $Queries = @{
                CreateDatabase = @(
                    'CREATE TABLE Components (Needle VARCHAR(64), LeftPattern VARCHAR(64), RightSubstring VARCHAR(64), Solutions UNSIGNED BIG INT);'
                    'CREATE TABLE DeadEnds (Needle VARCHAR(64) PRIMARY KEY);'
                    'CREATE INDEX idx_needle ON Components (Needle);'
                )
                Reset = 'DELETE FROM Components; DELETE FROM DeadEnds;'
            }
            if(Test-Path -Path $DatabasePath -and $ResetDB.IsPresent -and $ResetDB) {
                # Reset the database
                Remove-Item -Path $DatabasePath -Force
            }
            $Connection = New-SQLiteConnection -DataSource $DatabasePath
            try {
                if(-not (Test-Path -Path $DatabasePath)) {
                    # Create the database
                    foreach($Query in $Queries.CreateDatabase) {
                        Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Query -SqlParameters @{}
                    }
                }
            
                # Solve the needles
                for($index = 0; $index -lt $NeedleList.Count; $index++) {
                    Write-Host -ForegroundColor Green "NEEDLE: $($NeedleList[$index])"
                    if($Brute.IsPresent -and $Brute) {
                        Invoke-BruteSolver -Needle $NeedleList[$index] -PatternList $PatternList.Values
                    } else {
                        $Solutions = Invoke-Solver -Needle $NeedleList[$index] -PatternList $PatternList.Values -Connection $Connection
                        Write-Host ''
                        Write-Warning "SOLUTIONS FOR $($NeedleList[$index]): $Solutions"
                        [pscustomobject]@{
                            Needle = $NeedleList[$index]
                            Solutions = [ulong]$Solutions
                        }
                    }
                }

                #Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Queries.Reset -SqlParameters @{}
            } finally {
                $Connection.Close()
            }
        }
    }
    Function Invoke-BruteSolver {
        [cmdletbinding()]
        param(
            [Parameter(Mandatory)]
            [string]$Needle,
            [Parameter(Mandatory)]
            [System.Collections.Generic.HashSet[string]]$PatternList
        )
        Process {
            $Solutions = [pscustomobject]@{
                Needle = $Needle
                Solutions = [System.Collections.ArrayList]::new()
            }
            foreach($Pattern in $PatternList) {
                $script:Ticks++
                Write-Host -ForegroundColor Yellow -NoNewLine "`e[K`e[0GTICK: $($Ticks), NEEDLE: $($Needle)"

                # Check if the pattern matches the beginning of the needle
                if($Needle -notmatch "^$Pattern") {
                    Write-Debug "Pattern $Pattern does not match the needle $Needle"
                    continue
                }
                # If it matches, extract the pattern from the needle
                $NeedleSubstring = $Needle.SubString($Pattern.Length)

                # Write-Warning "Needle: '$Needle' - Pattern: '$Pattern' ($($Pattern.Length)) - Substring: '$NeedleSubstring' ($($NeedleSubstring.Length))"
                # If the needle is empty, we have a solution
                if($NeedleSubstring.Length -eq 0) {
                    $TempSolution = [System.Collections.ArrayList]::new()
                    $TempSolution.Add($Pattern) | Out-Null
                    $Solutions.Solutions.Add($TempSolution) | Out-Null
                } else {
                    # If the needle is not empty, we need to continue searching
                    $SubSolutions = Invoke-BruteSolver -PatternList $PatternList -Needle $NeedleSubstring
                    if($null -eq $SubSolutions) {
                        Write-Debug "No solution found for $NeedleSubstring"
                    } else {
                        # Then merge the solutions of lower level
                        foreach($TempSubSolution in $SubSolutions.Solutions) {
                            $TempSolution = [System.Collections.ArrayList]::new()
                            $TempSolution.Add($Pattern) | Out-Null
                            $TempSolution.AddRange($TempSubSolution) | Out-Null
                            $Solutions.Solutions.Add($TempSolution) | Out-Null
                        }
                    }
                }
            }

            if($Solutions.Solutions.Count -eq 0) {
                # If we have no solutions, return null
                return $null
            } else {
                # If we have solutions, return them
                return $Solutions
            }
        }
    }

    Function Invoke-Solver {
        [cmdletbinding()]
        [OutputType([ulong])]
        param(
            [Parameter(Mandatory)]
            [string]$Needle,
            [Parameter(Mandatory)]
            [System.Collections.Generic.HashSet[string]]$PatternList,
            [Parameter(Mandatory)]
            [System.Data.SQLite.SQLiteConnection]$Connection
        )
        Begin {
            $Queries = @{
                InsertComponents = 'INSERT INTO Components (Needle, LeftPattern, RightSubstring, Solutions) VALUES (@Needle, @LeftPattern, @RightSubstring, @Solutions);'
                GetComponents = 'SELECT * FROM Components WHERE Needle = @Needle;'
                InsertDeadEnd = 'INSERT INTO DeadEnds (Needle) VALUES (@Needle);'
                GetDeadEnd = 'SELECT * FROM DeadEnds WHERE Needle = @Needle;'
            }
        }
        Process {
            # Check if we this needle is known for having no solution
            $KnownDeadEnds = Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Queries.GetDeadEnd -SqlParameters @{ Needle = $Needle }
            if($KnownDeadEnds) {
                $script:Ticks++
                Write-Host -ForegroundColor Red -NoNewLine "`e[K`e[0GTICK: $($Ticks), DEADEND: $($Needle)"
                return 0
            }

            # Check if we already have a solution for this needle
            $KnownNeedles = Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Queries.GetComponents -SqlParameters @{ Needle = $Needle }
            if($KnownNeedles) {
                $script:Ticks++
                Write-Host -ForegroundColor Green -NoNewLine "`e[K`e[0GTICK: $($Ticks), CACHED: $($Needle)"
                return ($KnownNeedles.Solutions | Measure-Object -Sum | Select-Object -ExpandProperty Sum)
            }

            # Look for solution(s) by looking for patterns
            $Found = 0
            foreach($Pattern in $PatternList) {
                $script:Ticks++
                # Check if the pattern matches the beginning of the needle
                if($Needle -notmatch "^$Pattern") {
                    continue
                }
                # If it matches, extract the pattern from the needle
                $NeedleSubstring = $Needle.SubString($Pattern.Length)

                # If the needle substring is empty, we have a unique solution
                if($NeedleSubstring.Length -eq 0) {
                    Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Queries.InsertComponents -SqlParameters @{ Needle = $Needle; LeftPattern = $Pattern; RightSubstring = $null; Solutions = 1 }
                    $Found += 1
                } else {
                    # If the needle is not empty, we need to continue searching
                    Invoke-Solver -PatternList $PatternList -Needle $NeedleSubstring -Connection $Connection | Foreach-Object {
                        Write-Host -ForegroundColor Cyan -NoNewLine "`e[K`e[0GTICK: $($Ticks), FOUND: $($Needle) as $($Pattern) + $($NeedleSubstring) WITH: $($_) Solution(s)"
                        Invoke-SqliteQuery -SQLiteConnection $Connection -Query $Queries.InsertComponents -SqlParameters @{ Needle = $Needle; LeftPattern = $Pattern; RightSubstring = $NeedleSubstring; Solutions = $_ }
                        $Found += $_
                    }
                }
            }
            return $Found
        }
    }
}
Process {
    $Solutions = Start-Solver -DataFilePath $DataFilePath -Brute:$Brute
    [ulong]($Solutions | Measure-Object Solutions -Sum | Select-Object -ExpandProperty Sum)
}