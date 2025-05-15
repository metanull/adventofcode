<#
    .SYNOPSIS
    A dummy function, that doesn't not do anything
#>
[CmdletBinding()]
[OutputType([hashtable])]
param()
End {
    $script:AOC_2024_19_DICTIONARY.Clone()
}