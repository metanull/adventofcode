# https://adventofcode.com/2024/day/6#part2

Process {
    # Initializes the Guard's Watch (find the starting point)
    $Guard = [pscustomobject]@{X=0;Y=0;DirX=0;DirY=-1;Steps = 0;Turns=0;Visited=@()}
    $Expr = [regex]::new('\^')
    for($y=0;$y -lt $Height; $y++) {
        $Result = $Expr.Match($InputData[$y])
        if($Result.Success) {
            $Guard.X,$Guard.Y = $Result.Index,$y
            break
        }
    }

    # Perform the watch
    $GuardStarts = $Guard.PSObject.Copy()
    $GuardEnds = $null
    try {
        # Leave some footsteps behind (useful to see where the guard has been)
        $InputData[$Guard.Y] = $InputData[$Guard.Y].remove($Guard.X,1).insert($Guard.X,'X')
        $PotentialLoops = 0
        $LoopBlockers = @()
        while($true) {
            # Give a look far to the side, for a blocker or an existing footstep
            try {
                WhatIfThereWasABlockerAhead -Width $Width -Height $Height -InputData $InputData -Guard $Guard | Out-Null
            } catch [AlreadyBeenHereException] {
                # Potential Infinite Loop, that's interresting
                $PotentialLoops ++ 

                $CurrentBlocker = "($($Guard.X + $Guard.DirX),$($Guard.Y + $Guard.DirY))"
                if($LoopBlockers -notcontains $CurrentBlocker) {
                    # Write-Warning "Current Blocker: $CurrentBlocker; Total unique blockers: $($LoopBlockers.Count)"
                    $LoopBlockers += ,($CurrentBlocker)
                } else {
                    Write-Warning "Current Blocker: $CurrentBlocker; WAS ALREADY IN THE LIST!"
                }
                Write-Warning "Potential Infinite Loop #$($PotentialLoops) by blocking Guard where he is ($($Guard.X),$($Guard.Y)) using ($($_.Exception.GuardStarts.X + $_.Exception.GuardStarts.DirX),$($_.Exception.GuardStarts.Y + $_.Exception.GuardStarts.DirY)); Total unique blockers: $($LoopBlockers.Count); He'd do $($_.Exception.GuardEnds.Turns) turns and $($_.Exception.GuardEnds.Steps) steps; Trail Length: $($_.Exception.Trail.Length)"
            } catch [ExitsException] {
                # Guard would exit, we don't want that
                # Write-Warning "Clear sight to the exit on $($_.Exception.Guard.X),$($_.Exception.Guard.Y) after $($_.Exception.Guard.Steps) steps and $($_.Exception.Guard.Turns) turns"
            }

            # Let the Guard walk their way
            try {
                # Write-Warning "$($Guard.X), $($Guard.Y) - Direction: $($Guard.DirX), $($Guard.DirY)"
                Walk -Width $Width -Height $Height -InputData $InputData -Guard ([ref]$Guard) | Out-Null
            } catch [BlockedException] {
                # Guard is blocked, let's turn
                # Write-Warning "Blocked in $($Guard.X),$($Guard.Y), turning" 
                Turn -Width $Width -Height $Height -InputData $InputData -Guard ([ref]$Guard)
            } finally {
                # Leave some footsteps behind (useful to see where the guard has been)
                $InputData[$Guard.Y] = $InputData[$Guard.Y].remove($Guard.X,1).insert($Guard.X,'X')
            }
        }
    } catch [ExitsException] {
        $GuardEnds = $_.Exception.Guard
    } finally {
        Write-Warning "Entering from $($GuardStarts.X),$($GuardStarts.Y)"
        Write-Warning "Exiting from  $($GuardEnds.X),$($GuardEnds.Y)"
    }

    Write-Warning "Unique places (counting footsteps on the ground): $(([regex]::new('X')).Matches($InputData)|Measure-Object|Select-Object -ExpandProperty Count)"
    Write-Warning "Potential infinite loops: $($PotentialLoops)"
    Write-Warning "Unique Blockers for infinite loops: $($LoopBlockers.Count)"
    $LoopBlockers | Write-Output
}

Begin {
    $Year = 2024
    $Day = 6
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.(\w+\.)?ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning

    # Load InputData
    $InputData = Get-Content $InputPath -ErrorAction Stop
    $Width = $InputData[0].Length
    $Height = $InputData.Length

    # Function definition
    class ExitsException : Exception {
        [object]$Guard
        ExitsException([object]$Guard) : base('EXIT') {
            $this.Guard = $Guard.PSObject.Copy()
        }
    }
    class BlockedException : Exception{
        [object]$Guard
        BlockedException([object]$Guard) : base('BLOCKED') {
            $this.Guard = $Guard.PSObject.Copy()
        }
    }
    class AlreadyBeenHereException : Exception{
        [object]$GuardStarts
        [object]$GuardEnds
        [string]$Trail
        AlreadyBeenHereException([object]$GuardStarts,[object]$GuardEnds,[string]$Trail) : base('ALREADY') {
            $this.GuardStarts = $GuardStarts.PSObject.Copy()
            $this.GuardEnds = $GuardEnds.PSObject.Copy()
            $this.Trail = $Trail.PSObject.Copy()
        }
    }
    Function Walk {
        param(
            [int]$Width,
            [int]$Height,
            $InputData,
            [ref]$Guard
        )
        $Ahead = LookAhead -Width $Width -Height $Height -InputData $InputData -Guard $Guard.Value
        if($Ahead -eq '#') {
            throw [BlockedException]::new($Guard.Value)
        }
        $Guard.Value.Steps = $Guard.Value.Steps + 1
        $Guard.Value.X += $Guard.Value.DirX
        $Guard.Value.Y += $Guard.Value.DirY
        return $Ahead
    }
    Function Turn {
        param(
            [int]$Width,
            [int]$Height,
            $InputData,
            [ref]$Guard
        )
        $Guard.Value.DirX,$Guard.Value.DirY = -$Guard.Value.DirY,$Guard.Value.DirX
        $Guard.Value.Turns = $Guard.Value.Turns + 1
    }
    Function LookAhead {
        param(
            [int]$Width,
            [int]$Height,
            $InputData,
            $Guard
        )
        if($Guard.Y -le 0 -and $Guard.DirY -eq -1) {throw [ExitsException]::new($Guard)}
        if($Guard.Y -ge $Height -1 -and $Guard.DirY -eq 1) {throw [ExitsException]::new($Guard)}
        if($Guard.X -le 0 -and $Guard.DirX -eq -1) {throw [ExitsException]::new($Guard)}
        if($Guard.X -ge $Width -1 -and $Guard.DirX -eq 1) {throw [ExitsException]::new($Guard)}
        return $InputData[$Guard.Y + $Guard.DirY][$Guard.X + $Guard.DirX]
    }

    Function WhatIfThereWasABlockerAhead {
        param(
            [int]$Width,
            [int]$Height,
            $InputData,
            $Guard,
            [int]$MaxTurns = 250,
            [int]$AbsoluteMaxTurns = 500
        )
        $ImaginaryMap = $InputData.Clone()
        $ImaginaryGuard = $Guard.PSObject.Copy()
        $ImaginaryGuard.Steps = 0
        $ImaginaryGuard.Turns = 0
        $ImaginaryGuard | Add-Member -NotePropertyName LineOfSight -NotePropertyValue ''
        
        # Test what's ahead to avoid breaking the boundaries of the map
        LookAhead -Width $Width -Height $Height -InputData $ImaginaryMap -Guard $ImaginaryGuard
        # Add a blocker in front of the guard, and pretend walking
        $ImaginaryMap[$ImaginaryGuard.Y + $ImaginaryGuard.DirY] = $ImaginaryMap[$ImaginaryGuard.Y + $ImaginaryGuard.DirY].remove($ImaginaryGuard.X + $ImaginaryGuard.DirX,1).insert($ImaginaryGuard.X + $ImaginaryGuard.DirX,'#')

        # Let the Guard walk
        while($ImaginaryGuard.Turns -lt $AbsoluteMaxTurns <# Prevent a real infinite loop #>) {
            try {
                # Write-Warning "SIGHT $($ImaginaryGuard.X), $($ImaginaryGuard.Y) - Direction: $($ImaginaryGuard.DirX), $($ImaginaryGuard.DirY)"
                $ImaginaryGuard.LineOfSight += Walk -Width $Width -Height $Height -InputData $ImaginaryMap -Guard ([ref]$ImaginaryGuard)
                if($ImaginaryGuard.Turns -gt $MaxTurns -and $ImaginaryGuard.LineOfSight -match '(X*X{100})$') {
                    # Potential Infinite Loop, that's interresting
                    throw [AlreadyBeenHereException]::new($Guard,$ImaginaryGuard,$matches[1])
                }
            } catch [BlockedException] {
                # Guard is blocked, let's turn and keep on
                Turn -Width $Width -Height $Height -InputData $ImaginaryMap -Guard ([ref]$ImaginaryGuard)
            } finally {
                # Leave some footsteps behind (useful to see where the guard has been)
                $ImaginaryMap[$ImaginaryGuard.Y] = $ImaginaryMap[$ImaginaryGuard.Y].remove($ImaginaryGuard.X,1).insert($ImaginaryGuard.X,'X')
            }
        }
        if( $ImaginaryGuard.Turns -ge 499 ) {
            throw "Unwanted Infinite Loop..."
        }
    }
}