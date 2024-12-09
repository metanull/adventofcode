Process {
    $rxLine = [Regex]::new('^(?<x>\d+):\s(?<values>.*)$')
    $InputData | ForEach-Object {
        $rxLine.Matches($_) | Where-Object { $_.Success } | ForEach-Object {
            [pscustomobject]@{
                X = [long]$_.Groups['x'].Value
                Values = ($_.Groups['values'].Value -split '\s+' | ForEach-Object { [long]$_ })
            }
        }
    } | Foreach-Object {
        Write-Warning "X ::= $($_.X), Values ::= $($_.Values -join ', ')"
        
    }

}
Begin {
    $Year = 2024
    $Day = 7
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
}

