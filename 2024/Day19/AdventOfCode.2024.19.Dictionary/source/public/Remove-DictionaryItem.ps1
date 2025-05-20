<#
    .SYNOPSIS
    
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory, Position = 0)]
    [string]$Key
)
End {
    $script:AOC_2024_19_DICTIONARY.Remove($Key)
}