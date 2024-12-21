# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {

    $EnablePrintMap = $false
    $PrintFullMap = $true

    $Guard = [pscustomobject]@{X=0;Y=0;DirX=0;DirY=-1;Steps = 0;Turns=0;Visited=@()}
    $Expr = [regex]::new('\^')
    for($y=0;$y -lt $Height; $y++) {
        $Result = $Expr.Match($InputData[$y])
        if($Result.Success) {
            $Guard.X,$Guard.Y = $Result.Index,$y
            break
        }
    }
    $GuardStarts = $Guard.PSObject.Copy()
    $GuardBlocked = $null
    $GuardEnds = $null

    if($EnablePrintMap) {
        PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData -Full:$PrintFullMap
    }
    Write-Warning "Entering from $($Guard.X),$($Guard.Y)"

    $InZone = $true
    while($InZone) {
        # Let a trace on the map
        $InputData[$Guard.Y] = $InputData[$Guard.Y].remove($Guard.X,1).insert($Guard.X,'X')
        # Keep track of the visited places
        $Guard.Visited += ,"$($Guard.X),$($Guard.Y)"

        # Check if the guard will be exiting the zone
        if(($Guard.Y + $Guard.DirY) -ge $Height -or ($Guard.Y + $Guard.DirY) -lt 0 -or ($Guard.X + $Guard.DirX) -ge $Width -or ($Guard.X + $Guard.DirX) -lt 0) {
            Write-Warning "Exiting Zone in $($Guard.X),$($Guard.Y)"
            $GuardEnds = $Guard.PSObject.Copy()
            $InZone = $false
        } else {
            # Check if there is an obstacle ahead
            $Ahead = $InputData[$Guard.Y + $Guard.DirY][$Guard.X + $Guard.DirX]
            if($Ahead -eq '#') {
                # Change direction 90° clockwise
                if($EnablePrintMap) {
                    PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData -Full:$PrintFullMap   
                }
                #Write-Warning "Ahead is a wall in $($Guard.X + $Guard.DirX),$($Guard.Y + $Guard.DirY)"
                $GuardBlocked = $Guard.PSObject.Copy()
                $Guard.DirX,$Guard.DirY = -$Guard.DirY,$Guard.DirX
                $Guard.Turns = $Guard.Turns + 1
            }
        }
        # Let the guard move ahead
        $Guard.Steps ++
        $Guard.X += $Guard.DirX
        $Guard.Y += $Guard.DirY
    }
    if($EnablePrintMap) {
        PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData -Full:$PrintFullMap  
    }

    $Guard | Add-Member -NotePropertyName UniquePlaces -NotePropertyValue ($Guard.Visited | Select-Object -Unique | Measure-Object | Select-Object -ExpandProperty Count)
    #$Guard | Add-Member -NotePropertyName UniquePlaces2 -NotePropertyValue (([regex]::new('X')).Matches($InputData)|Measure-Object|Select-Object -ExpandProperty Count)
    #$Guard | Add-Member -NotePropertyName UniquePlaces3 -NotePropertyValue (($InputData|%{$_ -replace '[^X]'}) -join '').Length
    
    <#
    "Starts" | Write-Warning
    $GuardStarts | Write-Output
    "Ends" | Write-Warning
    $GuardEnds | Write-Output
    "Metrics" | Write-Warning
    $Guard | Write-Output
    #>

    "Unique places visited: $($Guard.UniquePlaces)" | Write-Host -ForegroundColor Green


    # Look up for possible infinite loops
    # Infinte loops are rectangles of the form:
    # 
    #  .#....       .#....       A rectangle where corners are adjacent to a blocker
    #  .....#       .@@@@#       The top left corner has a blocker on the right
    #  ......  -->  .@..@.  -->  The top right corner has a blocker on the bottom
    #  #.....  -->  #@@@@.  -->  The bottom right corner has a blocker on the right
    #  ....#.       ....#.       The bottom left corner has a blocker on the top
    #                            
    #
    # It is about finding "rectangles" matching exactly 3 of the 4 criteria (assuming there is no infinite loop in the input, there are no 4/4 cases
    #
    # Take each '#' that is adjacent to an 'X' (as if it is not the case, the guard is never approaching that '#')
    # Test if that '#' can be one of the corners of a rectangle
    #
    # The idea is good; and works well for all cases where there are one 4 turns to make an infinite loop
    # but there are other cases than rectangles: loops of 8, 12, ... turns
    # Find something else, See Day6.2.bis.ps1

    $EnablePrintMap = $true
    $PrintFullMap = $true

    $Candidates = @{
        TopLeft = @()
        TopRight = @()
        BottomRight = @()
        BottomLeft = @(([pscustomobject]@{x=$GuardStarts.x-1;y=$GuardStarts.y}))
    }
    if($GuardEnds.DirY -eq 1) {
        # Guard leaves from the bottom
        $Candidates.BottomRight = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
        $Candidates.BottomLeft = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
    } elseif($GuardEnds.DirY -eq -1) {
        # Guard leaves from the top
        $Candidates.TopRight = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
        $Candidates.TopLeft = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
    } elseif($GuardEnds.DirX -eq 1) {
        # Guard leaves from the right
        $Candidates.TopRight = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
        $Candidates.BottomRight = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
    } elseif($GuardEnds.DirX -eq -1) {
        # Guard leaves from the left
        $Candidates.TopLeft = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
        $Candidates.BottomLeft = @(([pscustomobject]@{x=$GuardEnds.x;y=$GuardEnds.y}))
    }
    $rxFindBlocker = [regex]::new('#')
    for($y=0; $y -lt $Height; $y ++) {
        $rxFindBlocker.Matches($InputData[$y]) | Where-Object { $_.Success } | ForEach-Object {
            $x = $_.Index
            if($y -lt $Height-1   -and $InputData[$y+1][$x] -eq 'X') {
                $Candidates.TopLeft += ,([pscustomobject]@{x=$x;y=$y+1})
            }

            if($x -gt 0         -and $InputData[$y][$x-1] -eq 'X') {
                $Candidates.TopRight += ,([pscustomobject]@{x=$x-1;y=$y})
            }

            if($y -gt 0         -and $InputData[$y-1][$x] -eq 'X') {
                $Candidates.BottomRight += ,([pscustomobject]@{x=$x;y=$y-1})
            }

            if($x -lt $Width-1    -and $InputData[$y][$x+1] -eq 'X') {
                $Candidates.BottomLeft += ,([pscustomobject]@{x=$x+1;y=$y})
            }
        }
    }

    $Rectangles = @()
    # Find rectangles with TopLeft, TopRight and BottomRight
    $Candidates.TopLeft |% {
        $TL = $_
        $Candidates.TopRight |? { $_.y -eq $TL.y -and $_.x -gt $TL.x} |% {
            $TR = $_
            $Candidates.BottomRight |? { $_.x -eq $TR.x -and $_.y -gt $TR.y} |% {
                $BR = $_
                $Rectangles += ,@{TL=$TL;TR=$TR;BR=$BR;BL=$null;Missing=[pscustomobject]@{x=$TL.x;y=$BR.y};Blocker=[pscustomobject]@{x=$TL.x-1;y=$BR.y}}
            }
        }
    }
    # Find rectangles with TopRight, BottomRight and BottomLeft
    $Candidates.TopRight |% {
        $TR = $_
        $Candidates.BottomRight |? { $_.x -eq $TR.x -and $_.y -gt $TR.y} |% {
            $BR = $_
            $Candidates.BottomLeft |? { $_.y -eq $BR.y -and $_.x -lt $BR.x} |% {
                $BL = $_
                $Rectangles += ,@{TL=$null;TR=$TR;BR=$BR;BL=$BL;Missing=[pscustomobject]@{x=$BL.x;y=$TR.y};Blocker=[pscustomobject]@{x=$BL.x;y=$TR.y-1}}
            }
        }
    }
    # Find rectangles with BottomRight, BottomLeft and TopLeft
    $Candidates.BottomRight |% {
        $BR = $_
        $Candidates.BottomLeft |? { $_.y -eq $BR.y -and $_.x -lt $BR.x} |% {
            $BL = $_
            $Candidates.TopLeft |? { $_.x -eq $BL.x -and $_.y -lt $BL.y} |% {
                $TL = $_
                $Rectangles += ,@{TL=$TL;TR=$null;BR=$BR;BL=$BL;Missing=[pscustomobject]@{x=$BR.x;y=$TL.y};Blocker=[pscustomobject]@{x=$BR.x+1;y=$TL.y}}
            }
        }
    }
    # Find rectangles with BottomLeft, TopLeft and TopRight
    $Candidates.BottomLeft |% {
        $BL = $_
        $Candidates.TopLeft |? { $_.x -eq $BL.x -and $_.y -lt $BL.y} |% {
            $TL = $_
            $Candidates.TopRight |? { $_.y -eq $TL.y -and $_.x -gt $TL.x} |% {
                $TR = $_
                $Rectangles += ,@{TL=$TL;TR=$TR;BR=$null;BL=$BL;Missing=[pscustomobject]@{x=$TR.x;y=$BL.y};Blocker=[pscustomobject]@{x=$TR.x;y=$BL.y+1}}
            }
        }
    }

    # Discard items that are not in the guards' path
    $InfiniteLoops = ($Rectangles |? { $InputData[$_.Blocker.y][$_.Blocker.x] -eq 'X'})
    "Infinite Loops including '4 Turns': $($InfiniteLoops.Count)" | Write-Warning

    PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData -Full:$PrintFullMap -Rectangles ($InfiniteLoops)
    
}
Begin {
    $Year = 2024
    $Day = 6
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
    $Width = $InputData[0].Length
    $Height = $InputData.Length

    Function PrintGuard {
        <#
        .SYNOPSIS
            Prints the map with the guard position and path
        #>
        param($GuardStarts,$Guard,[AllowNull()]$GuardBlocked,[AllowNull()]$GuardEnds,$InputData,$Rectangles,[switch]$Full)
        if($null -ne $GuardBlocked) {
            if(($GuardBlocked.X) -eq ($Guard.X)) {
                $YellowX = ($Guard.X)
                $YellowY = ($GuardBlocked.Y)..($Guard.Y)
            } else {
                $YellowX = ($GuardBlocked.X)..($Guard.X)
                $YellowY = ($Guard.Y)
            }
        }
        for($y=0;$y -lt $InputData.Count;$y++) {
            if(($null -eq $Full) -or (($Full -ne $true) -and ($InputData[$y] -notmatch 'X'))) {
                continue
            }
            for($x=0;$x -lt $InputData[$y].Length;$x++) {
                $CANDIDATE = $false
                if($null -ne $Rectangles) {
                    $CANDIDATE = ($Rectangles.Blocker |? { $_.x -eq $x -and $_.y -eq $y }).Count -gt 0
                }
                if($CANDIDATE) {                                    Write-Host -NoNewline -BackgroundColor Yellow $InputData[$y][$x]
                } elseif($x -eq $Guard.X -and $y -eq $Guard.Y) {            Write-Host -NoNewline -BackgroundColor Green 'G'
                } elseif($x -eq $GuardStarts.X -and $y -eq $GuardStarts.Y) {        Write-Host -NoNewline -BackgroundColor Cyan 'S'
                } elseif($x -eq $GuardEnds.X -and $y -eq $GuardEnds.Y) {  Write-Host -NoNewline -BackgroundColor Magenta 'E'
                } elseif($x -in $YellowX -and $y -in $YellowY) {    Write-Host -NoNewline -BackgroundColor Blue $InputData[$y][$x]
                } else {
                    if($InputData[$y][$x] -eq '#') {                Write-Host -NoNewline -ForegroundColor Red '#'
                    } else {                                        Write-Host -NoNewline -ForegroundColor White $InputData[$y][$x]
                    }
                }
            }
            Write-Host ""
        }
        Write-Host ""
    }
}