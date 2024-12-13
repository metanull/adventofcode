# https://adventofcode.com/2024/day/11
Process {
    $blink = 25

    for($i = 0; $i -lt $blink; $i++) {
        $Stones = Blink -Stones $Stones
        Write-Warning "Blink $i : $($Stones.Count)"
    }
    $Stones.Count
}
Begin {
    $Year = 2024
    $Day = 11
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
    $Stones = [regex]::new('(?<digit>\d+)') | ForEach-Object {
        $_.Matches($InputData) | ForEach-Object {
            [long]::parse($_.Groups['digit'].Value)
        }
    }

    Function Blink {
        [CmdletBinding()]
        [OutputType([long[]])]
        param(
            [long[]] $Stones
        )
        $Stones | ForEach-Object {
            if($_ -eq 0) {
                1 | Write-Output
                return
            }
            if("$_".length % 2 -eq 0) {
                $Div = [math]::Pow(10, "$_".length / 2)
                [math]::truncate($_ / $Div) | Write-Output
                [math]::truncate($_ % $Div) | Write-Output
                return
            }
            $_ * 2024 | Write-Output
        }
    }
}