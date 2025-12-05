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

# Global cache for memoization
$Script:LengthCache = @{}

Function Get-SequenceLength {
    [CmdletBinding()]
    param(
        [string]$FromKey,
        [string]$ToKey,
        [int]$Depth,
        [hashtable]$KeyPad,
        [hashtable]$GapPos
    )
    
    # Base case: if we're at the human level (depth 0), count the moves directly
    if($Depth -eq 0) {
        $Moves = @(Find-KeyPadPath -KeyPad $KeyPad -GapPos $GapPos -TargetKey $ToKey -CurrentKey $FromKey)
        return $Moves.Count
    }
    
    # Check cache
    $CacheKey = "$FromKey->$ToKey@$Depth"
    if($Script:LengthCache.ContainsKey($CacheKey)) {
        return $Script:LengthCache[$CacheKey]
    }
    
    # Get the moves needed for this transition
    $Moves = @(Find-KeyPadPath -KeyPad $KeyPad -GapPos $GapPos -TargetKey $ToKey -CurrentKey $FromKey)
    
    # Calculate the length needed at the next level
    $TotalLength = 0
    $CurrentPos = 'A'
    
    $Keypads = Import-PowerShellDataFile -Path (Join-Path -Path $PSScriptRoot -ChildPath Keypads.psd1)
    
    foreach($Move in $Moves) {
        # Each move requires a transition from currentPos to move at depth-1
        $TotalLength += Get-SequenceLength -FromKey $CurrentPos -ToKey $Move -Depth ($Depth - 1) `
            -KeyPad $Keypads.Directional.Moves -GapPos $Keypads.Directional.Gap
        $CurrentPos = $Move
    }
    
    # Cache and return
    $Script:LengthCache[$CacheKey] = $TotalLength
    return $TotalLength
}

Function Invoke-RobotChainOptimized {
    [CmdletBinding()]
    Param(
        [string[]] $Code,
        [int]$NumDirectionalRobots = 25
    )
    Begin {
        $Keypads = Import-PowerShellDataFile -Path (Join-Path -Path $PSScriptRoot -ChildPath Keypads.psd1)
    } 
    Process {
        # Clear cache for each code
        $Script:LengthCache = @{}
        
        $TotalLength = 0
        $CurrentKey = 'A'
        
        foreach($TargetKey in $Code) {
            $Length = Get-SequenceLength -FromKey $CurrentKey -ToKey $TargetKey `
                -Depth $NumDirectionalRobots `
                -KeyPad $Keypads.Numeric.Moves -GapPos $Keypads.Numeric.Gap
            $TotalLength += $Length
            $CurrentKey = $TargetKey
        }
        
        $CodeString = $Code -join ''
        $CodeFingerprint = [long](($CodeString) -replace '^\D+|\D+$')
        $Complexity = $TotalLength * $CodeFingerprint

        [PSCustomObject]@{
            Code = $CodeString
            CodeValue = $CodeFingerprint
            SolutionLength = $TotalLength
            Complexity = $Complexity
        }
    }
}

$Codes = @('029A','980A','179A','456A','379A')
#$Codes = @('780A','539A','341A','189A','682A')
$Solutions = foreach($Code in $Codes) {
    Invoke-RobotChainOptimized -Code $Code.ToCharArray() -NumDirectionalRobots 25
}
$Solutions | Select-Object -Property Code,CodeValue,SolutionLength
$Solutions | Measure-Object -Sum -Property Complexity | Select-Object -ExpandProperty Sum

