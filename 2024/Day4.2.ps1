# https://adventofcode.com/2024/day/4#part2
Process {
    # Define what we are looking at
    $Needle = 'MAS'

    if($Needle.Length % 2 -eq 0) {
        throw "Needle has an even number of characters!"
    }

    # Browse the matrix
    $Position=@{X=0;Y=0;V=$null}
    $Matrix.GetEnumerator() | Foreach-Object {
        $_.GetEnumerator() | Foreach-Object {
            $Position.V=$_
            if($_ -eq $Needle[($Needle.Length-1)/2]){
                # Look for the middle character
                [pscustomobject]$Position | Write-Output
            }
            $Position.X = $Position.X+1
        }
        $Position.Y = $Position.Y+1
        $Position.X=0
    } | Foreach-Object {
        $CurrentIndex = $_
        # Get the Words crossing at the current point in diagonal
        @{
            NorthEast = @{ XInc =  1; YInc =  1}
            SouthEast = @{ XInc =  1; YInc = -1}
            SouthWest = @{ XInc = -1; YInc = -1}
            NorthWest = @{ XInc = -1; YInc =  1}
        }.GetEnumerator() | Foreach-Object {
            $Label = $_.key
            $Direction = $_.value
            if( (Test-MatrixSubstringFromMiddle -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Direction.XInc -YInc $Direction.YInc -MatrixSize $MatrixSize -Length $Needle.Length)) {
                $SubString = Get-MatrixSubstringFromMiddle -Matrix $Matrix -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Direction.XInc -YInc $Direction.YInc -MatrixSize $MatrixSize -Length $Needle.Length
                if($Substring -eq $Needle) {
                    # Only retain the points, where words crossing is matching Needle
                    [pscustomobject]@{
                        X = $CurrentIndex.X
                        Y = $CurrentIndex.Y
                        Direction = $Label
                        SubString = $Substring
                    } | Write-Output
                }
            }
        }
    } | Select-Object @{Name='Point';Expression={"$($_.X),$($_.Y)"}},Direction | Group-Object Point | Foreach-Object {
        if(      [bool]($_.Group.Direction | Where-Object { $_ -in 'SouthWest','NorthEast' }) `
            -and [bool]($_.Group.Direction | Where-Object { $_ -in 'SouthEast','NorthWest' })) {
            # Only retain points where both branches of the cross match the Needle
            $_.Name
        }
    } | Measure-Object
}
Begin {
    function Test-MatrixSubstringFromMiddle {
        <#
            .SYNOPSIS
            Test if it is possible to obtain a word of a given length from a matrix, starting from a given point - which is to be at the center of that word - in a given direction
        #>
        param([long]$X,[long]$Y,[long]$XInc,[long]$YInc,[long]$MatrixSize,[long]$Length)
        if(-Not($Length%2)) {
            throw "Needle's length is even!"
        }
        $Length = ($Length -1)/2

        if(     ($X - ($Length * [Math]::Abs($XInc))) -ge 0 `
           -and ($Y - ($Length * [Math]::Abs($YInc))) -ge 0 `
           -and ($X + ($Length * [Math]::Abs($XInc))) -lt $MatrixSize `
           -and ($Y + ($Length * [Math]::Abs($YInc))) -lt $MatrixSize ) {
            return $true
        }
        return $false
    }
    function Get-MatrixSubstringFromMiddle {
        <#
            .SYNOPSIS
            Get a word from a matrix, starting from a given point - which is to be at the center of that word - in a given direction
        #>
        param([String[]]$Matrix,[long]$X,[long]$Y,[long]$XInc,[long]$YInc,[long]$MatrixSize,[long]$Length)
        if(-not (Test-MatrixSubstringFromMiddle -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Value.XInc -YInc $Value.YInc -MatrixSize $MatrixSize -Length $Length)) {
            return $null
        }
        $Offset = (($Length -1)/2)
        $NewX = $X - ($Offset * $XInc)
        $NewY = $Y - ($Offset * $YInc)

        $Str = ''
        for($i=0;$i -lt $Length;$i++) {
            $Str += $Matrix[$NewY+($i*$YInc)][$NewX+($i*$XInc)]
        }
        $Str | Write-Output
    }

    # Read the input
    <#
$File = @'
M.S.S.S
.A...A.
M.S.M.M
.A.A...
M.S.M.S
.A.A.A.
M.S.M.S
'@

        $Matrix = $File -split "`r?`n"
    #>
    $Matrix = Get-Content (Join-Path $PSScriptRoot Input\Day4.txt -Resolve)
    if($Matrix.Count -ne $Matrix[0].Length) {
        throw 'Input matrix not a square!'
    }
    $MatrixSize = $Matrix.Count
}