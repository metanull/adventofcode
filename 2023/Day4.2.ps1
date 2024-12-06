Process {
    $Cards = $InputData | ForEach-Object {
        $Card = $_
        $Card | Add-Member -NotePropertyName Instance -NotePropertyValue 1
        $Card | Add-Member -NotePropertyName Gain -NotePropertyValue ($Card.Winning | Where-Object {$_ -in $Card.Numbers} | Measure-Object | Select-Object -ExpandProperty Count)
        $Card | Write-Output
    }

    
    #$MyCards = [int[]](0..($Cards.Count -1) |% { 1 })
    #$MyGains = [int[]]::new($Cards.Count)
    $MyCards = [int[]]($Cards|Sort-Object Card|Select-Object -ExpandProperty Instance)
    $MyGains = [int[]]($Cards|Sort-Object Card|Select-Object -ExpandProperty Gain)
    for($y=0;$y -lt $MyCards.Count; $y++) {
        $CardCount = $MyCards[$y]
        $CardGain = $MyGains[$y]
        # "@$y : Count=$CardCount Gain=$CardGain" | Write-Warning
        for($n = 0; $n -lt $CardGain; $n++) {
            $MyCards[$y + $n +1] = $MyCards[$y + $n+1] + $CardCount
        }
    }
    $MyCards | Measure-Object -Sum | Select-Object -ExpandProperty Sum
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
            Card = [int]$Matches.Card
            Winning = $Matches.Winning -split '\s+' | Where-Object {$_} | Foreach-Object {[int]$_}
            Numbers = $Matches.Numbers -split '\s+' | Where-Object {$_} | Foreach-Object {[int]$_}
        } | Write-Output
    }

}