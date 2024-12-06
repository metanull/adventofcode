# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {

    $EnablePrintMap = $true

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
        PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData -Full
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
                    PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData    
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
        PrintGuard -GuardStarts $GuardStarts -Guard $Guard -GuardBlocked $GuardBlocked -GuardEnds $GuardEnds -InputData $InputData    
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
        param($GuardStarts,$Guard,[AllowNull()]$GuardBlocked,[AllowNull()]$GuardEnds,$InputData,[switch]$Full)
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
            if($Full -or $InputData[$y] -notmatch 'X') {
                continue
            }
            for($x=0;$x -lt $InputData[$y].Length;$x++) {
                if($x -eq $Guard.X -and $y -eq $Guard.Y) {            Write-Host -NoNewline -BackgroundColor Green 'G'
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