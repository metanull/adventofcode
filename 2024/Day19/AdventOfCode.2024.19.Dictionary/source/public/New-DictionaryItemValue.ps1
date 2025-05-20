<#
    .SYNOPSIS
    
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
    if(-not ($script:AOC_2024_19_DICTIONARY.Keys -contains $Key)) {
        throw "Key '$Key' not found in dictionary"
    }
    $script:AOC_2024_19_DICTIONARY[$Key].Parts.Add($ValueItem) | Out-Null
}