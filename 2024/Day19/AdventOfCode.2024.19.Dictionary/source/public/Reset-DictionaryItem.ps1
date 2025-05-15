<#
    .SYNOPSIS
    A dummy function, that doesn't not do anything
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory, Position = 0)]
    [string]$Key
)
End {
    $script:AOC_2024_19_DICTIONARY[$Key] = [PSCustomObject]@{
        Pattern = $Key
        Parts = [System.Collections.ArrayList]::new()
    }
}