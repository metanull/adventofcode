<#
    .SYNOPSIS
    
    .PARAMETER Key
        The Key in the dictionary
    .PARAMETER RefItem
        Powershell has the tendancy to flatten multidimentional arrays sent to the output-stream
        To get the actual Item, unmodified, it is preferable to fetch it using a reference (this parameter), than by using the returned values

    .EXAMPLE
        $ResultA = Get-DictionaryItemValue -Key abc
    .EXAMPLE
        $ResultB = $null
        $ResultA = Get-DictionaryItemValue -Key abc -RefItem ([ref]$ResultB)

        # ResultA is a flat array, ResultB is a multidimentional ArrayList
#>
[CmdletBinding()]
[OutputType([System.Collections.ArrayList])]
param(
    [Parameter(Mandatory, Position = 0)]
    [string]$Key,
    [Parameter(Mandatory = $false, Position = 1)]
    [ref]$RefItem = $null
)
End {
    if($script:AOC_2024_19_DICTIONARY[$Key].Parts -and $null -ne $RefItem) {
        $RefItem.Value = $script:AOC_2024_19_DICTIONARY[$Key].Parts.Clone()
    }
    return $script:AOC_2024_19_DICTIONARY[$Key].Parts
}