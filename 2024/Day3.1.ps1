# https://adventofcode.com/2024/day/3

Process {
    $Reg = [regex]::new('mul\(\d+,\d+\)')
    $Reg.Matches($Mem) |% {
        $x = [int]($matches[1])
        $y = [int]($matches[2])
        # "$($_.value)`t$x * $y = $($x * $y)"|Write-Verbose
        ($x * $y) | Write-Output
    } | Measure-Object -Sum
}
Begin {
    $Mem = Get-Content -Raw (Join-Path -Resolve $PSScriptRoot Input\Day3.txt)
}