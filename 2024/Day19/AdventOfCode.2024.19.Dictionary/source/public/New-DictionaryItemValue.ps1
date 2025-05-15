<#
    .SYNOPSIS
    A dummy function, that doesn't not do anything
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory, Position = 0)]
    [string]$Key,
    [Parameter(Mandatory, Position = 1)]
    [System.Collections.ICollection]$ValueItem
)
End {
    $script:AOC_2024_19_DICTIONARY[$Key].Parts.Add($ValueItem) | Out-Null
}