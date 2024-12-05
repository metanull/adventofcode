# https://adventofcode.com/2023/day/2#part2
Process {
    $Rules = @{
        red  = 12
        green = 13
        blue = 14
    }
    $InputData | ForEach-Object {
        $Game = [pscustomobject]@{Id=$null;Sets=@()}
        if(-not ($_ -match '^Game (\d+):( .*)$')) {
            throw "Line [$_] is not compliant"
        }
        $Game.Id = [int]$matches[1]
        $Game.Sets = $matches[2] -split ';' | Foreach-Object {
            $Set = @()
            $_ -split ',' | Foreach-Object {
                if(-not ($_ -match '^ (\d+) (red|green|blue)$')) {
                    throw "Subset [$_] is not compliant"
                }
                $Set += ,([pscustomobject]@{Count = [int]$matches[1];Color = $matches[2]})
            }
            [pscustomobject]@{Cubes=$Set} | Write-Output
        }
        $Game | Write-Output
    } | Foreach-Object {
        # Find the minimum required number of cubes per color, and multiply them together
        $Game = $_
        $Power = 1
        $Rules.keys | Foreach-Object {
            $Color = $_
            $Power *= ($Game.Sets.Cubes|?{$_.Color -eq $Color}|Measure-Object Count -Maximum|Select-Object -ExpandProperty Maximum)
        }
        $Power  | Write-Output
    } | Measure-Object -Sum
}
Begin {
    $PuzzleUrl = "https://adventofcode.com/$(Get-Date -Format 'yyyy')/day/$([int](Get-Date -Format 'dd'))"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
}