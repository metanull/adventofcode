<#
    .SYNOPSIS
    
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory, Position = 0)]
    [System.Collections.ICollection]$ValueItem
)
End {
    $ValueItemFootprint = $ValueItem -join ':'
    $script:AOC_2024_19_DICTIONARY.GetEnumerator() | Where-Object {
        for($k = 0; $k -lt $_.Value.Parts.Count; $k ++) {
            $valueParts = [System.Collections.ArrayList]::new()
            for($l = 0; $l -lt $_.Value.Parts[$k].Count; $l ++) {
                $valueParts.Add($_.Value.Parts[$k][$l]) | Out-Null
            }
            if(($valueParts -join ':') -eq $ValueItemFootprint) {
                return $true
            }
        }
    } | Select-Object -ExpandProperty Key
}