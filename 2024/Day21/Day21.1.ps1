Function Find-KeyPadPath {
    [CmdletBinding()]
    param(
        [hashtable]$KeyPad,
        [hashtable]$GapPos,
        [string]$TargetKey,
        [string]$CurrentKey = 'A'
    )
    $Start = $KeyPad[$CurrentKey]
    $End = $KeyPad[$TargetKey]

    $dx = $End.X - $Start.X
    $dy = $End.Y - $Start.Y

    # Generate horizontal moves (negative dx = left '<', positive dx = right '>')
    $HorizontalMoves = for($i = 0; $i -lt [Math]::Abs($dx); $i++) {
        if($dx -lt 0) { '<' } else { '>' }
    }

    # Generate vertical moves (negative dy = up '^', positive dy = down 'v')
    $VerticalMoves = for($i = 0; $i -lt [Math]::Abs($dy); $i++) {
        if($dy -lt 0) { '^' } else { 'v' }
    }

    # Check if horizontal-first path crosses gap
    $hFirstCrossesGap = ($Start.Y -eq $GapPos.Y) -and ($End.X -eq $GapPos.X)
    # Check if vertical-first path crosses gap
    $vFirstCrossesGap = ($Start.X -eq $GapPos.X) -and ($End.Y -eq $GapPos.Y)

    if($hFirstCrossesGap -and -not $vFirstCrossesGap) {
        # Must go vertical first
        $VerticalMoves
        $HorizontalMoves
    } elseif($vFirstCrossesGap -and -not $hFirstCrossesGap) {
        # Must go horizontal first
        $HorizontalMoves
        $VerticalMoves
    } else {
        # No gap conflict - prefer left moves first, then vertical, then right
        if($dx -lt 0) {
            # Going left: do horizontal first
            $HorizontalMoves
            $VerticalMoves
        } else {
            # Going right or no horizontal: do vertical first
            $VerticalMoves
            $HorizontalMoves
        }
    }

    # Always output 'A' to press the button
    'A'
}

Function Invoke-RobotChain {
    [CmdletBinding()]
    Param(
        [System.Collections.ArrayList] $Code
    )
    Begin {
        $Keypads = Import-PowerShellDataFile -Path (Join-Path -Path $PSScriptRoot -ChildPath Keypads.psd1)
    } 
    Process {
        $CurrentKey = 'A'
        $Robot1 = foreach($TargetKey in $Code) {
            Find-KeyPadPath -KeyPad $Keypads.Numeric.Moves -GapPos $Keypads.Numeric.Gap -TargetKey $TargetKey -CurrentKey $CurrentKey
            $CurrentKey = $TargetKey
        }
        $CurrentKey = 'A'
        $Robot2 = foreach($TargetKey in $Robot1) {
            Find-KeyPadPath -KeyPad $Keypads.Directional.Moves -GapPos $Keypads.Directional.Gap -TargetKey $TargetKey -CurrentKey $CurrentKey
            $CurrentKey = $TargetKey
        }
        $CurrentKey = 'A'
        $Robot3 = foreach($TargetKey in $Robot2) {
            Find-KeyPadPath -KeyPad $Keypads.Directional.Moves -GapPos $Keypads.Directional.Gap -TargetKey $TargetKey -CurrentKey $CurrentKey
            $CurrentKey = $TargetKey
        }

        $Final = $Robot3
        $ShortestSolutionLength = [int]($Final | Measure-Object | Select-Object -ExpandProperty Count)
        $CodeFingerprint = [int](($Code -join '') -replace '^\D+|\D+$')
        $Solution = $ShortestSolutionLength * $CodeFingerprint

        [PSCustomObject]@{
            Code = $Code -join ''
            CodeValue = $CodeFingerprint
            Solution = $Final -join ''
            SolutionLength = $ShortestSolutionLength
            Complexity = $Solution
        }
    }
}

#$Codes = @('029A','980A','179A','456A','379A')
$Codes = @('780A','539A','341A','189A','682A')
$Solutions = foreach($Code in $Codes) {
    Invoke-RobotChain -Code $Code.ToCharArray()
}
$Solutions | Select-Object -Property Code,CodeValue,SolutionLength,Solution
$Solutions | Measure-Object -Sum -Property Complexity | Select-Object -ExpandProperty Sum

