# https://adventofcode.com/2024/day/4#part1

Process {
    $Needle = 'XMAS'
    
    $Position=@{X=0;Y=0;V=$null}
    $Matrix.GetEnumerator() | Foreach-Object {
        $_.GetEnumerator() | Foreach-Object {
            $Position.V=$_
            [pscustomobject]$Position | Write-Output
            $Position.X = $Position.X+1
        }
        $Position.Y = $Position.Y+1
        $Position.X=0
    } | Where-Object { $_.V -eq $Needle[0] } | Foreach-Object {
        $CurrentIndex = $_
        @{
            North     = @{ XInc =  0; YInc =  1}
            NorthEast = @{ XInc =  1; YInc =  1}
            East      = @{ XInc =  1; YInc =  0}
            SouthEast = @{ XInc =  1; YInc = -1}
            South     = @{ XInc =  0; YInc = -1}
            SouthWest = @{ XInc = -1; YInc = -1}
            West      = @{ XInc = -1; YInc =  0}
            NorthWest = @{ XInc = -1; YInc =  1}
        }.GetEnumerator() | Foreach-Object {
            $Direction = $_.key
            $Value = $_.value
            if( (Test-MatrixSubstring -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Value.XInc -YInc $Value.YInc -MatrixSize $MatrixSize -Length $Needle.Length)) {
                $SubString = Get-MatrixSubstring -Matrix $Matrix -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Value.XInc -YInc $Value.YInc -Length $Needle.Length
                if($Substring -eq $Needle) {
                    [pscustomobject]@{
                        X = $CurrentIndex.X
                        Y = $CurrentIndex.Y
                        Direction = $Direction
                        SubString = $Substring
                    } | Write-Output
                }
            }
        }
    
    } | Measure-Object
}

Begin {

    # Import Matrix functions
    . (Join-Path $PSScriptRoot ..\Template\Matrix.ps1)
    

    <#
    $File = @'
XMAS...
.XMAS..
..XMAS.
...XMAS
..XMAS.
.XMAS..
XMAS...
'@
    $Matrix = $File -split "`r?`n"
    #>
    $Matrix = Get-Content (Join-Path $PSScriptRoot Input\Day4.txt -Resolve)
    if($Matrix.Count -ne $Matrix[0].Length) {
        throw 'Input matrix not a square!'
    }
    $MatrixSize = $Matrix.Count
}