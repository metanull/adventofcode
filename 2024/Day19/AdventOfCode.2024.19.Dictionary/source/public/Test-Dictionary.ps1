<#
    .SYNOPSIS
    A dummy function, that doesn't not do anything
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param()
End {
    try {
        Get-Variable -Name AOC_2024_19_DICTIONARY -Scope script -ErrorAction Stop
        return $true
    } catch {
        return $false
    }
}
