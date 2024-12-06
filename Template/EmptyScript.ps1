# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    $InputData | ForEach-Object {
        $Line = $_
    }
}
Begin {
    $Year = [int]$(Get-Date -Format 'yyyy')
    $Day = [int](Get-Date -Format 'dd')
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