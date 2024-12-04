# https://adventofcode.com/2024/day/1#part2
$Data = Get-Content (Join-Path $PSScriptRoot Input/Day1.txt -Resolve) | Foreach-Object {
    $s=$_ -split '\s+'
    [pscustomobject]@{Left=$s[0];Right=$s[1]}
}
$Right = ($Data.Right | Sort-Object)
$Left = ($Data.Left | Sort-Object)

$Left | ForEach-Object {
    $L = [int]$_
    $R = ($Right | Where-Object { $_ -eq $L }).Count
    ($L * $R)
} | Measure-Object -Sum
