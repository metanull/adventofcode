Process {
    $rxLine = [Regex]::new('^(?<x>\d+):\s(?<values>.*)$')
    $InputData | ForEach-Object {
        $rxLine.Matches($_) | Where-Object { $_.Success } | ForEach-Object {
            [pscustomobject]@{
                X = [long]$_.Groups['x'].Value
                Values = ($_.Groups['values'].Value -split '\s+' | ForEach-Object { [long]$_ })
            }
        }
    } | Foreach-Object {
        # Compute the possible operations
        # Write-Warning "X ::= $($_.X), Values ::= $($_.Values -join ', ')"
        $List = $_.Values + ,($_.x)
        $_ | Add-Member -MemberType NoteProperty -Name MulSum -Value (MulSum -List $List) -PassThru
    } | ForEach-Object {
        # Browse the results; foreach of them, select the initial result (X), and compute a sum of all these results
        $Z = $_
        # WalkMulSum returns a list of strings, describing the possible operations (or nothing when no such operation exists)
        # We take the first one of each batch, just to make sure that there was at least one result
        # Then we take the X corresponding to this result, and sum all these X
        WalkMulSum -MulSum $_ | Select-Object -First 1 | ForEach-Object {
            $Z.X
        } | Measure-Object -Sum
    }

}
Begin {
    $Year = 2024
    $Day = 7
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    Function WalkNode {
        [CmdletBinding()]
        [OutputType([bool])]
        param(
            [Parameter(Mandatory)]
            [pscustomobject]$Node,

            [Parameter(Mandatory)]
            [long]$Level,

            [string]$Stack
        )
        
        $ret = @()
        if($Node.MulRecurse -is [bool] -and $Node.MulRecurse) {
            $ret += ,"$($Stack)*"
        } elseif($Node.MulRecurse) {
            WalkNode -Node $Node.MulRecurse -Level ($Level +1) -Stack "$($Stack)*" | Foreach-Object {
                $ret += ,$_
            }
        }
        if($Node.SumRecurse -is [bool] -and $Node.SumRecurse) {
            $ret += ,"$($Stack)+"
        } elseif($Node.SumRecurse) {
            WalkNode -Node $Node.SumRecurse -Level ($Level +1) -Stack "$($Stack)+" | Foreach-Object {
                $ret += ,$_
            }
        }
        return $ret
    }
    Function WalkMulSum {
        [CmdletBinding()]
        param(
            [Parameter(Mandatory)]
            [pscustomobject]$MulSum
        )

        if($MulSum.MulSum.MulRecurse) {
            WalkNode -Node $MulSum.MulSum -Level 1 -Stack '' | ForEach-Object {
                $_ -join ',' | Write-Output
            }
        }
        if($MulSum.MulSum.SumRecurse) {
            WalkNode -Node $MulSum.MulSum -Level 1 -Stack '' | ForEach-Object {
                $_ -join ',' | Write-Output
            }
        }
    }
    Function MulSum {
        [CmdletBinding()]
        param(
            [AllowEmptyCollection()]
            [AllowNull()]
            [long[]]$List
        )

        $Mul,$MulRecurse = $null,$null
        $Sum,$SumRecurse = $null,$null
        # Start-Sleep -Milliseconds 500

        if($List) {
            Write-Warning "List: [$($List -join ', ')]"
            $Mul = Mul -List $List
            if($Mul) {
                if($Mul.Count -eq 2) {
                    Write-Warning "Mul is $($Mul[0] -eq $Mul[1])"
                    $MulRecurse = ($Mul[0] -eq $Mul[1])
                } else {
                    Write-Warning "Mul!"
                    $MulRecurse = MulSum -List $Mul
                }
            }
            $Sum = Sum -List $List
            if($Sum) {
                if($Sum.Count -eq 2) {
                    Write-Warning "Sum is $($Sum[0] -eq $Sum[1])"
                    $SumRecurse = ($Sum[0] -eq $Sum[1])
                } else {
                    Write-Warning "Sum!"
                    $SumRecurse = MulSum -List $Sum
                }
            }
        }
        [pscustomobject]@{
            Mul = $Mul
            MulRecurse = $MulRecurse
            Sum = $Sum
            SumRecurse = $SumRecurse
        }
    }

    Function Mul {
        [CmdletBinding()]
        param([long[]]$List)

        if($List.Count -le 2) {
            throw 'Not enough items for Mul'
        }
        $Res = $List | Select-Object -Last 1
        $Prv = $List | Select-Object -Last 1 -Skip 1
        if($Res % $Prv -eq 0) {
            $List | Select-Object -First ($List.Length - 2) | Write-Output
            $Res / $Prv | Write-Output
        }
    }
    Function Sum {
        [CmdletBinding()]
        param([long[]]$List)
        
        if($List.Count -le 2) {
            throw 'Not enough items for Mul'
        }
        $Res = $List | Select-Object -Last 1
        $Prv = $List | Select-Object -Last 1 -Skip 1
        if($Res - $Prv -ge 0) {
            $List | Select-Object -First ($List.Length - 2) | Write-Output
            $Res - $Prv | Write-Output
        }
    }
}

