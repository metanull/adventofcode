# https://adventofcode.com/2023/day/2
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
        # Check compliance
        $Game = $_
        try {
            $Rules.GetEnumerator() | Foreach-Object {
                $Color = $_.Key
                $Available = [int]$_.Value
                if($Available -lt ($Game.Sets.Cubes|?{$_.Color -eq $Color}|Measure-Object Count -Maximum|Select-Object -ExpandProperty Maximum)) {
                    throw "Not enough $($Color) cubes for Game $($Game.Id)"
                }
            }
            $Game | Write-Output
        } catch {
            # Non compliant, skipp
            $_ | Write-Warning
        }
    } | ForEach-Object {
        # We care only about the Sum of the "Id" of the compliant games
        $_.Id
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