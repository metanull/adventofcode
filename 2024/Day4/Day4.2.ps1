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
                $SubString = Get-MatrixSubstringFromMiddle -Matrix $Matrix -X $CurrentIndex.X -Y $CurrentIndex.Y -XInc $Direction.XInc -YInc $Direction.YInc -Length $Needle.Length
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
    
    # Import Matrix functions
    . (Join-Path $PSScriptRoot ..\Template\Matrix.ps1)

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