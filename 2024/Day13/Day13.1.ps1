# https://adventofcode.com/2024/day/13
Process {
    $InputData | ForEach-Object {
        # Write-Warning "Prize($($_.Prize.X), $($_.Prize.Y)); A($($_.A.X), $($_.A.Y)); B($($_.B.X), $($_.B.Y))"
        $Solutions = @()
        for($i = 0; $i -lt 100; $i ++) {
            $MulA = $_.A.PSObject.Copy();       $MulA.X *= $i; $MulA.Y *= $i
            $MulB = $_.B.PSObject.Copy();       $MulB.X *= $i; $MulB.Y *= $i
            $SolA = $_.Prize.PSObject.Copy();   $SolA.X -= $MulA.X; $SolA.Y -= $MulA.Y
            $SolB = $_.Prize.PSObject.Copy();   $SolB.X -= $MulB.X; $SolB.Y -= $MulB.Y

            # Write-Warning "A*i($($MulA.X), $($MulA.Y)); B*i($($MulB.X), $($MulB.Y)); Prize-(A*i)($($SolA.X), $($SolA.Y)); Prize-(B*i)($($SolB.X), $($SolB.Y))"
            if(($SolA.X -lt 0 -or $SolA.Y -lt 0) -and ($SolB.X -lt 0 -or $SolB.Y -lt 0)) {
                #Write-Warning "No solution found"
                break
            }
            if($SolA.X -eq 0 -and $SolA.Y -eq 0) {
                #Write-Warning "Solution found: A=$i, B=0"
                $Solutions += ,([pscustomobject]@{ A = $i; B = 0})
            }
            if($SolB.X -eq 0 -and $SolB.Y -eq 0) {
                #Write-Warning "Solution found: A=0, B=$i"
                $Solutions += ,([pscustomobject]@{ A = 0; B = $i})
            }
            if($SolA.X % $_.B.X -eq 0) {
                $N = $SolA.X / $_.B.X
                if($N -gt 0 -and $SolA.Y / $_.B.Y -eq $N) {
                    #Write-Warning "Solution found (A): A=$i, B=$N"
                    $Solutions += ,([pscustomobject]@{ A = $i; B = $N})
                }
            }
            if($SolB.X % $_.A.X -eq 0) {
                $N = $SolB.X / $_.A.X
                if($N -gt 0 -and $SolB.Y / $_.A.Y -eq $N) {
                    #Write-Warning "Solution found (B): A=$N, B=$i"
                    $Solutions += ,([pscustomobject]@{ A = $N; B = $i})
                }
            }
        }
        if($Solutions) {
            # Write-Warning "Solution(s) found: $(($Solutions|Select-Object -Unique A,B).Count)"
            $_ | Add-Member -MemberType NoteProperty -Name Solutions -Value ($Solutions|Select-Object -Unique A,B) -PassThru
        }
    } | Where-Object {$_.Solutions.Count -gt 0} | Foreach-Object {
        $Machine = $_
        $BestSolution = $Machine.Solutions | ForEach-Object -Begin {
                $CostA = 3
                $CostB = 1
            } -Process {
                $Cost = ($_.A * $CostA) + ($_.B * $CostB)
                $_ | Add-Member -MemberType NoteProperty -Name Cost -Value $Cost -PassThru
            } | Sort-Object -Property Cost | Select-Object -First 1
        $Machine | Add-Member -MemberType NoteProperty -Name BestSolution -Value $BestSolution

        $Machine.BestSolution.Cost | Write-Output
    } | Measure-Object -Sum | Select-Object -ExpandProperty Sum
}
Begin {
    $Year = 2024
    $Day = 13
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "..\Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning

    Function BufferToObject {
        [CmdletBinding()]
        [OutputType([pscustomobject])]
        param(
            [Parameter(Mandatory)]
            [System.Array]
            $Buffer
        )
        Begin {
            $Parser = [regex]::new('^(?<Label>.*?): X[=\+](?<X>\d+), Y[=\+](?<Y>\d+)$')
            Function RegexGroupToObject {
                [CmdletBinding()]
                [OutputType([pscustomobject])]
                param(
                    [Parameter(Mandatory)]
                    [ValidateScript({$_ -is [System.Text.RegularExpressions.Match]})]
                    [System.Text.RegularExpressions.Match]
                    $Match
                )
                [pscustomobject]@{
                    #Label = $Match.Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                    X = [int]($Match.Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value)
                    Y = [int]($Match.Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value)
                }
            }
        }
        Process {
            [pscustomobject]@{
                A = RegexGroupToObject -Match $Parser.Match($Buffer[0])
                B = RegexGroupToObject -Match $Parser.Match($Buffer[1])
                Prize = RegexGroupToObject -Match $Parser.Match($Buffer[2])
            }
        }
    }

    $InputData = Get-Content $InputPath -ErrorAction Stop | Foreach-Object -Begin {
        $Parser = [regex]::new('^(?<Label>.*?): X[=\+](?<X>\d+), Y[=\+](?<Y>\d+)$')
        $Buffer = @()
        $Object = $null
    } -Process {
        if(-not $_) {
            $Buffer = @()
            return
        }
        if($Buffer.Count -le 3) {
            $Buffer += ,$_
        }
        if($Buffer.Count -eq 3) {
            BufferToObject -Buffer ($Buffer) | Write-Output
        }
    }
}