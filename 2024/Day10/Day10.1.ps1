# https://adventofcode.com/2024/day/10
Process {
    $Width = $InputData[0].Length
    $Height = $InputData.Length
    WalkMap -map $InputData | Measure-Object score -Sum
}
Begin {
    $Year = 2024
    $Day = 10
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    Function WalkMap {
        [CmdletBinding()]
        [OutputType([long])]
        param(
            [object[]] $map
        )
        $TrailHeads = @()
        #$Total = 0
        for($y = 0; $y -lt $map.Length; $y++) {
            for($x = 0; $x -lt $map[$y].Length; $x++) {
                $Script:List = @()
                $PathCount = Next -map $map -previous $null -position (Point -x $x -y $y)
                #$Total += $PathCount
                if($PathCount -gt 0) {
                    $TrailHeads += [pscustomobject]@{head = (Point -x $x -y $y); score = $Script:List.Count}
                }
            }
        }
        # return $Total
        return $TrailHeads
    }
    Function Next {
        [CmdletBinding()]
        [OutputType([object[]])]
        param(
            [object[]] $map,
            [allownull()][object] $previous = $null,
            [allownull()][object] $position = $null
        )
        if($null -eq $position) {
            #Write-Warning "Next($null) ::= !NULL WAS EXPECTED"
            return 0
        }
        if($null -eq $previous) {
            if((ValueAt -map $map -position $position) -ne 0) {
                #Write-Warning "Next($null,($($position.x),$($position.y))) ::= $(ValueAt -map $map -position $position) => 0 WAS EXPECTED"
                return 0
            }
        } else {
            if((ValueAt -map $map -position $position) -ne (ValueAt -map $map -position $previous) + 1) {
                #Write-Warning "Next(($($previous.x),$($previous.y)),($($position.x),$($position.y))) ::= $(ValueAt -map $map -position $position) => $(ValueAt -map $map -position $previous) WAS EXPECTED"
                return 0
            }
            if((ValueAt -map $map -position $position) -eq 9) {
                #Write-Warning "Next(($($previous.x),$($previous.y)),($($position.x),$($position.y))) ::= 9 => RETURN 1 ******************************"

                if(-not ($Script:List|?{$_.x -eq $position.x -and $_.y -eq $position.y})){
                    $Script:List += ,$position
                }
                return 1
            }
        }
        $Value = 0
        
        $Value += Next -map $map -previous $position -position (Up -map $map -position $position)
        $Value += Next -map $map -previous $position -position (Right -map $map -position $position)
        $Value += Next -map $map -previous $position -position (Down -map $map -position $position)
        $Value += Next -map $map -previous $position -position (Left -map $map -position $position)

        <#
        $Spacer = '..' * (ValueAt -map $map -position $position)
        if($true) {Write-Warning "$($Spacer)Current: $(ValueAt -map $map -position $position) -> Up:    $(ValueAt -map $map -position (Up -map $map -position $position)) => Result: $U"}
        $U = Next -map $map -previous $position -position (Up -map $map -position $position)
        if($true) {Write-Warning "$($Spacer)Current: $(ValueAt -map $map -position $position) -> Right: $(ValueAt -map $map -position (Right -map $map -position $position)) => Result: $R"}
        $R = Next -map $map -previous $position -position (Right -map $map -position $position)
        if($true) {Write-Warning "$($Spacer)Current: $(ValueAt -map $map -position $position) -> Down:  $(ValueAt -map $map -position (Down -map $map -position $position)) => Result: $D"}
        $D = Next -map $map -previous $position -position (Down -map $map -position $position)
        if($true) {Write-Warning "$($Spacer)Current: $(ValueAt -map $map -position $position) -> Left:  $(ValueAt -map $map -position (Left -map $map -position $position)) => Result: $L"}
        $L = Next -map $map -previous $position -position (Left -map $map -position $position)
        $Value = $U + $R + $D + $L
        if($Value) {Write-Warning "$($Spacer)Current: $(ValueAt -map $map -position $position) -> All Directions ::= $Value ***************************"}
        #>

        return $Value
    }
    Function Point {
        [CmdletBinding()]
        [OutputType([pscustomobject])]
        param(
            [parameter(Position=0,Mandatory)][int]$x,
            [parameter(Position=1,Mandatory)][int]$y
        )
        return New-Object -TypeName PSObject -Property @{x=$x;y=$y}
    }
    Function ValueAt {
        [CmdletBinding()]
        [OutputType([int])]
        param(
            [object[]] $map,
            [object] $position
        )
        if($position.x -lt 0 -or $position.y -lt 0 -or $position.x -ge $map[0].Length -or $position.y -ge $map.Length) {
            return $null
        }
        try {
            $intValue = [int]::Parse($map[$position.y][$position.x])
        } catch {
            return $null
        }
        # Write-Warning "ValueAt($($position.x),$($position.y)) ::= $($intValue)"
        return $intValue
    }
    Function Down {
        [CmdletBinding()]
        [OutputType([object])]
        param(
            [object[]] $map,
            [object] $position
        )
        if($position.y -ge $map.Length - 1) {
            #Write-Warning "Down($($position.x),$($position.y)) ::= NULL"
            return $null
        }
        $newPosition = New-Object -TypeName PSObject -Property @{x=$position.x;y=$position.y+1}
        #Write-Warning "Down($($position.x),$($position.y)) ::= ($($newPosition.x),$($newPosition.y))"
        return $newPosition
    }
    function Up {
        [CmdletBinding()]
        [OutputType([object])]
        param(
            [object[]] $map,
            [object] $position
        )
        if($position.y -le 0) {
            #Write-Warning "Up($($position.x),$($position.y)) ::= NULL"
            return $null
        }
        $newPosition = New-Object -TypeName PSObject -Property @{x=$position.x;y=$position.y-1}
        #Write-Warning "Up($($position.x),$($position.y)) ::= ($($newPosition.x),$($newPosition.y))"
        return $newPosition
    }
    function Left {
        [CmdletBinding()]
        [OutputType([object])]
        param(
            [object[]] $map,
            [object] $position
        )
        if($position.x -le 0) {
            #Write-Warning "Left($($position.x),$($position.y)) ::= NULL"
            return $null
        }
        $newPosition = New-Object -TypeName PSObject -Property @{x=$position.x-1;y=$position.y}
        #Write-Warning "Left($($position.x),$($position.y)) ::= ($($newPosition.x),$($newPosition.y))"
        return $newPosition
    }
    function Right {
        [CmdletBinding()]
        [OutputType([object])]
        param(
            [object[]] $map,
            [object] $position
        )
        if($position.x -ge $map[0].Length - 1) {
            #Write-Warning "Right($($position.x),$($position.y)) ::= NULL"
            return $null
        }
        $newPosition = New-Object -TypeName PSObject -Property @{x=$position.x+1;y=$position.y}
        #Write-Warning "Right($($position.x),$($position.y)) ::= ($($newPosition.x),$($newPosition.y))"
        return $newPosition
    }
    
}