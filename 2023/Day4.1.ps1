Process {
    $InputData | ForEach-Object {
        $Card = $_
        $Card | Add-Member -NotePropertyName Gain -NotePropertyValue ($Card.Winning | Where-Object {$_ -in $Card.Numbers} | Measure-Object | Select-Object -ExpandProperty Count) -PassThru | Write-Output
    } | Foreach-Object {
        if($_.Gain -gt 0) {
            [Math]::Pow(2, $_.Gain - 1)
        }
    } | Measure-Object -Sum
}
Begin {
    $Year = 2023
    $Day = 4
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning

    $RegEx = [regex]::new('^Card\s+(?<Card>\d+): (?<Winning>[\s\d]+) \| (?<Numbers>[\s\d]+)$')
    $InputData = Get-Content $InputPath -ErrorAction Stop | ForEach-Object {
        if(-not ($_ -match $RegEx)) {
            throw "Invalid input: $_"
        }
        [pscustomobject]@{
            Card = $Matches.Card
            Winning = $Matches.Winning -split '\s+' | Where-Object {$_} | Foreach-Object {[int]$_}
            Numbers = $Matches.Numbers -split '\s+' | Where-Object {$_} | Foreach-Object {[int]$_}
        } | Write-Output
    }

}