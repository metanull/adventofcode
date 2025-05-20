<#
    .SYNOPSIS
    
#>
[CmdletBinding()]
[OutputType([pscustomobject])]
param(
    [Parameter(Mandatory, Position = 0)]
    [string]$Key
)
End {
    return $script:AOC_2024_19_DICTIONARY[$Key]
}