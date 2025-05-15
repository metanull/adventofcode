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
    if(-not ($script:AOC_2024_19_DICTIONARY.Keys -contains $Key)) {
        return
    }
    $ValueItemFootprint = $ValueItem -join ':'
    for($k = 0; $k -lt $script:AOC_2024_19_DICTIONARY[$Key].Parts.Count; $k ++) {
        $valueParts = [System.Collections.ArrayList]::new()
        for($l = 0; $l -lt $script:AOC_2024_19_DICTIONARY[$Key].Parts[$k].Count; $l ++) {
            $valueParts.Add($script:AOC_2024_19_DICTIONARY[$Key].Parts[$k][$l]) | Out-Null
        }
        if(($valueParts -join ':') -eq $ValueItemFootprint) {
            $script:AOC_2024_19_DICTIONARY[$Key].Parts.RemoveAt($k) | Out-Null
            break
        }
    }
}
