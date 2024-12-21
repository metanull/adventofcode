# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]

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

class ExitsException : Exception {
    [object]$Guard
    ExitsException([object]$Guard) : base('EXIT') {
        $this.Guard = $Guard
    }
}
class BlockedException : Exception{
    [object]$Guard
    [object]$Blocker
    BlockedException([object]$Guard,[object]$Blocker) : base('BLOCKED') {
        $this.Guard = $Guard
        $this.Blocker = $Blocker
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
        throw [BlockedException]::new($Guard.Value.PSObject.Copy(),[pscustomobject]@{X=$Guard.Value.X;Y=$Guard.Value.Y;Value=$Ahead})
    }
    $Guard.Value.Steps = $Guard.Value.Steps + 1
    $Guard.Value.X += $Guard.Value.DirX
    $Guard.Value.Y += $Guard.Value.DirY
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
    if($Guard.Y -le 0 -and $Guard.DirY -eq -1) {throw [ExitsException]::new($Guard.PSObject.Copy())}
    if($Guard.Y -ge $Height -1 -and $Guard.DirY -eq 1) {throw [ExitsException]::new($Guard.PSObject.Copy())}
    if($Guard.X -le 0 -and $Guard.DirX -eq -1) {throw [ExitsException]::new($Guard.PSObject.Copy())}
    if($Guard.X -ge $Width -1 -and $Guard.DirX -eq 1) {throw [ExitsException]::new($Guard.PSObject.Copy())}
    return $InputData[$Guard.Y + $Guard.DirY][$Guard.X + $Guard.DirX]
}

# Load InputData
$InputData = Get-Content $InputPath -ErrorAction Stop
$Width = $InputData[0].Length
$Height = $InputData.Length

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
    while($true) {
        try {
            # Let the Guard walk
            # Write-Warning "$($Guard.X), $($Guard.Y) - Direction: $($Guard.DirX), $($Guard.DirY)"
            Walk -Width $Width -Height $Height -InputData $InputData -Guard ([ref]$Guard)
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