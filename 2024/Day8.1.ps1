# https://adventofcode.com/2024/day/8
Process {
    $Origin = [pscustomobject]@{name='O';X=0;y=0}
    $End = [pscustomobject]@{name='Z';X=$InputData[0].length -1;y=$InputData.count -1}
    $Point = for($y=0;$y -lt $InputData.count; $y++) {
        ([regex]::new('([a-zA-Z0-9])')).Matches($InputData[$y])|Where-Object{
            $_.Success
        } | Foreach-Object {
            [pscustomobject]@{freq=[string]($_.Value);X=[long]($_.Index);Y=[long]($y)}
        }
    }

    $Point | select-object -ExpandProperty freq -Unique | Foreach-Object {
        $freq = $_
        $set = $Point|Where-Object{$_.freq -ceq $freq}  # -ceq = CASE SENSITIVE
        for($i=0;$i -lt $set.count - 1;$i++){
            $A = [pscustomobject]@{name='A';x=$set[$i].x;y=$set[$i].y}
            for($j=1;$j -lt $set.count -$i;$j++){
                $B = [pscustomobject]@{name='B';x=$set[$i+$j].x;y=$set[$i+$j].y}
                $T = [pscustomobject]@{x=($B.x-$A.x);y=($B.y-$A.y)}
                [pscustomobject]@{x=$A.x - $T.x; y=$A.y - $T.y} | Write-Output
                [pscustomobject]@{x=$B.x + $T.x; y=$B.y + $T.y} | Write-Output
            }
        }
    } | Where-Object {
        $_.x -ge $Origin.x -and $_.x -le $End.x -and $_.y -ge $Origin.y -and $_.y -le $End.y
    } | Group-Object x,y | Tee-Object -Variable UniqueAntinodes | Measure-Object


    # Print the MAP
    for($y = 0; $y -lt 2; $y++) {
        for($x = 0; $x -lt $InputData[$y].Length; $x++) {
            switch($y) {
                0 { [int][Math]::Truncate($x / 10) | Write-Host -NoNewline}
                1 { $x % 10 | Write-Host -NoNewline }
            }
        }
        '|' | Write-Host
    }
    for($y = 0; $y -lt $InputData.count; $y++) {
        $Line = $InputData[$y]
        for($x = 0; $x -lt $Line.Length; $x++) {
            $Char = $Line[$x]
            
            $FG = $host.UI.RawUI.ForegroundColor
            $BG = $host.UI.RawUI.BackgroundColor
            if("$x, $y" -in $UniqueAntinodes.Name) {
                $BG = [ConsoleColor]::Magenta
                $FG = [ConsoleColor]::Cyan
                if($Char -eq '.') {
                    $Char = "#"
                }
            }
            if($Char -ne '.') {
                #$BG = [ConsoleColor]::Cyan
                $FG = [ConsoleColor]::Cyan
            }
            $Char | Write-Host -ForegroundColor $FG -BackgroundColor $BG -NoNewline
        }
        "|$y`t" | Write-Host -ForegroundColor $host.UI.RawUI.ForegroundColor -BackgroundColor $host.UI.RawUI.BackgroundColor -NoNewline
        $Legend = ($UniqueAntinodes.Name | Select-Object -Unique |? { $_ -match ", $y`$"}) -join '); ('
        "($Legend)" | Write-Host
    }

}
Begin {
    $Year = 2024 #[int]$(Get-Date -Format 'yyyy')
    $Day = 8 #[int](Get-Date -Format 'dd')
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    Function PointSub {
        [CmdletBinding()]
        [OutputType([pscustomobject])]
        param(
            [pscustomobject]$A,
            [pscustomobject]$B
        )
        return New-Point -x ($B.x-$A.x) -y($B.y-$A.y) -name "($($B.name) - $($A.name))"
    }
    Function PointAdd {
        [CmdletBinding()]
        [OutputType([pscustomobject])]
        param(
            [pscustomobject]$A,
            [pscustomobject]$B
        )
        return New-Point -x ($B.x+$A.x) -y($B.y+$A.y) -name "($($B.name) + $($A.name))"
    }

    Function Test-Point {
        [CmdletBinding()]
        [OutputType([bool])]
        param(
            [pscustomobject]$A,
            [pscustomobject]$B
        )
        return -not (Compare-Object $A $B -Property x,y)
    }
    Function Test-PointInRange {
        [CmdletBinding()]
        [OutputType([bool])]
        param(
            [pscustomobject]$A,
            [pscustomobject]$Origin,
            [pscustomobject]$End
        )
        return ($A.x -ge $Origin.x -and $A.Y -ge $Origin.y -and $A.x -lt $End.x -and $A.y -lt $End.y)
    }
    Function Select-UniquePoint {
        [CmdletBinding()]
        [OutputType([pscustomobject[]])]
        param(
            [pscustomobject[]]$Point
        )
        $Point | Group-Object x,y | Select-Object x,y -First 1 | Foreach-Object { New-Point -x $_.x -y $_.y }
    }

    Function New-Point {
        [CmdletBinding()]
        [OutputType([pscustomobject])]
        param(
            [long]$x,
            [long]$y,
            [Parameter(Mandatory=$false)]
            [string]$name=$null
        )
        [pscustomobject]@{x=$x;y=$y;name=$name}
    }
}