# https://adventofcode.com/2024/day/1
$Data = Get-Content (Join-Path $PSScriptRoot Input/Day1.txt -Resolve) | Foreach-Object {
    $s=$_ -split '\s+'
    [pscustomobject]@{Left=$s[0];Right=$s[1]}
}
$Right = ($Data.Right | Sort-Object)
$Left = ($Data.Left | Sort-Object)

$k = 0
$Left | ForEach-Object {
    $L = $_
    $R = $Right | Select-Object -First 1 -Skip $k
    [Math]::Abs($L -$R) | Write-Output
    $k++
} | Measure-Object -Sum