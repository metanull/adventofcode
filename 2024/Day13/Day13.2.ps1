# https://adventofcode.com/2024/day/13
Process {
    $InputData | ForEach-Object {
        # Write-Warning "Prize($($_.Prize.X), $($_.Prize.Y)); A($($_.A.X), $($_.A.Y)); B($($_.B.X), $($_.B.Y))"
        $Solutions = @()

        $Riddle = $_
        $Multiples = Find-Multiples -target ($Riddle.Prize.X) -a ($Riddle.A.X) -b ($Riddle.B.X)
        $Multiples | Foreach-Object {
            $Multiple = $_
            if( $Riddle.Prize.Y -eq ($Multiple.X * $Riddle.A.Y + $Multiple.Y * $Riddle.B.Y)) {
                $Solutions += ,([pscustomobject]@{ A = $_.X; B = $_.Y })
            }
        }
        if($Solutions) {
            Write-Warning "Solution(s) found: $(($Solutions|Select-Object -Unique A,B).Count)"
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

        $Machine 
        #$Machine.BestSolution.Cost | Write-Output
    } #| Measure-Object -Sum | Select-Object -ExpandProperty Sum
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

    function Find-Multiples {
        param (
            [long]$target,
            [long]$a,
            [long]$b
        )
        Write-Warning "Finding multiples of $a and $b to sum to $target"
        $Multiples = @()
        for ($x = 0; $x -le [math]::Floor($target / $a); $x++) {
            for ($y = 0; $y -le [math]::Floor($target / $b); $y++) {
                if (($a * $x + $b * $y) -eq $target) {
                    $Multiples += ,([pscustomobject]@{ X = $x; Y = $y })
                    Write-Warning "Found $x multiples of $a and $y multiples of $b; $target = $($a * $x) + $($b * $y)"
                }
            }
        }
        Write-Warning "Found $($Multiples.Count) multiples: $($Multiples|%{"$($_.X) multiples of $a and $($_.Y) multiples of $b; "})"
        $Multiples | Write-Output
    }    
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
            $Prize = RegexGroupToObject -Match $Parser.Match($Buffer[2])
            $Prize.X += 10000000000000
            $Prize.Y += 10000000000000
            [pscustomobject]@{
                A = RegexGroupToObject -Match $Parser.Match($Buffer[0])
                B = RegexGroupToObject -Match $Parser.Match($Buffer[1])
                Prize = $Prize
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