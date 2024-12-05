# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    $Parts = @()
    $x = 0; $y = 0
    for( $y = 0; $y -lt $InputData.Count; $y++) {
        for( $x = 0; $x -lt $InputData[$y].Length; $x++) {
            if($InputData[$y][$x] -match '\d') {
                if($InputData[$y].SubString($x) -match '^(?<Number>\d+)') {
                    $Parts += [pscustomobject]@{Number=$matches.Number;X=$x;Y=$Y}
                    $x += $matches.Number.Length
                } else {
                    throw 'sbeuh'
                }
            }
        }
    }
    $Parts | ForEach-Object {
        $Part = $_
        $w = "$($Part.Number)".Length
        $x1 = $Part.X; $y1 = $Part.Y; $x2 = $Part.X + $w -1; $y2 = $Part.Y
        $sym = ''
        if($x1 -gt 0) {    $x1--}
        if($x2 -lt ($InputData[$Part.Y].Length -1)) { $x2++}
        if($y1 -gt 0) {    $y1--}
        if($y2 -lt ($InputData.Count - 1)) {          $y2++}
        if($y1 -ne $Part.Y) {
            $sym += $InputData[$y1].Substring($x1,$x2-$x1+1)
        }
        if($y2 -ne $Part.Y) {
            $sym += $InputData[$y2].Substring($x1,$x2-$x1+1)
        }
        if($x1 -ne $Part.X) {
            $sym += $InputData[$_.Y].Substring($x1,1)
        }
        if($x2 -ne $Part.X + $w -1) {
            $sym += $InputData[$Part.Y].Substring($x2,1)
        }
        # Add the identified symbols to the Part object, and retiurn to the pipeline
        $Part | Add-Member -NotePropertyName Symbols -NotePropertyValue ([RegEx]::new('([^\.])')).Matches($sym).Value -PassThru | Write-Output
    } | Where-Object {
        # Keep only Part numbers with a symbol
        $_.Symbols.Count -gt 0
    } | Measure-Object Number -Sum
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