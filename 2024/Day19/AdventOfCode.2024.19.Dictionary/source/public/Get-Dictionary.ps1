<#
    .SYNOPSIS
    
#>
[CmdletBinding()]
[OutputType([hashtable])]
param()
End {
    $script:AOC_2024_19_DICTIONARY.Clone()
}