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
                $SubString = Get-MatrixSubstring -Matrix $Matrix -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Value.XInc -YInc $Value.YInc -MatrixSize $MatrixSize -Length $Needle.Length
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
    function Test-MatrixSubstring {
        <#
            .SYNOPSIS
            Test if it is possible to obtain a word of a given length from a matrix, starting from a given point, in a given direction
        #>
        param([long]$X,[long]$Y,[long]$XInc,[long]$YInc,[long]$MatrixSize,[long]$Length)
        if(      ((($X) + ($Length * $XInc)) -le ($MatrixSize)) `
            -and ((($Y) + ($Length * $YInc)) -le ($MatrixSize)) `
            -and ((($X+1) + ($Length * $XInc)) -ge 0) `
            -and ((($Y+1) + ($Length * $YInc)) -ge 0) ) {
            return $true
        }
        return $false
    }
    function Get-MatrixSubstring {
        <#
            .SYNOPSIS
            Get a word from a matrix, starting from a given point, and in a given direction
        #>
        param([String[]]$Matrix,[long]$X,[long]$Y,[long]$XInc,[long]$YInc,[long]$MatrixSize,[long]$Length)
        if(-not (Test-MatrixSubstring -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Value.XInc -YInc $Value.YInc -MatrixSize $MatrixSize -Length $Length)) {
            return $null
        }
        $Str = ''
        for($i=0;$i -lt $Length;$i++) {
            $Str += $Matrix[$Y+($i*$YInc)][$X+($i*$XInc)]
        }
        $Str | Write-Output
    }

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