# https://adventofcode.com/2024/day/2
$Reports = @()
$k = 0
$Reports = Get-Content (Join-Path $PSScriptRoot Input\Day2.txt) | Foreach-Object {
    $Levels = ($_ -split '\s+') | Foreach-Object {
        [int]$_ | Write-Output
    }
    [pscustomobject]@{k=$k++; Levels=$Levels}
}

$Reports | Foreach-Object {
    $Levels = $_.Levels
    if($Levels[0] -lt $Levels[1]) {
        $IncreaseDirection = 1
    } else {
        $IncreaseDirection = -1
    }

    try {
        for($i=1;$i -lt $Levels.Count; $i++) { 
            $Diff = $Levels[$i] - $Levels[$i-1] 
            $AbsDiff = [Math]::Abs($Diff)
            if($Diff -gt 0 -and $IncreaseDirection -lt 0) {
                throw "$($Levels[$i-1]) -> $($Levels[$i]): increase instead of decrease"
            }
            if($Diff -lt 0 -and $IncreaseDirection -gt 0) {
                throw "$($Levels[$i-1]) -> $($Levels[$i]): decrease instead of increase"
            }
            if( -not ($AbsDiff -in 1,2,3)) {
                throw "$($Levels[$i-1]) -> $($Levels[$i]): ||increase ::= $AbsDiff|| not in (1,2,3)"
            }
        }
        $_ | write-Output
    } catch {
        "$($Levels -join ','): $_" | Write-Verbose
    }
} | Measure-Object