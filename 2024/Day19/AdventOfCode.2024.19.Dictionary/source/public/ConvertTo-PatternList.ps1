<#
    .SYNOPSIS
    
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(ValueFromPipeline)]
    [AllowEmptyString()]
    [AllowNull()]
    $PatternList = $null
)
Begin {
    $TemplateObject = [pscustomobject]@{
        Patterns = [System.Collections.Generic.HashSet[string]]::new()
    }
    $TemplateObject | Add-Member -MemberType ScriptProperty -Name Count -Value {$this.Patterns.Count}
    $TemplateObject | Add-Member -MemberType ScriptMethod -Name Contains -Value {param([Parameter(Mandatory)][string]$Pattern) $this.Patterns -contains $Pattern}
    $TemplateObject | Add-Member -MemberType ScriptProperty -Name SortedPatterns -Value {$this.Patterns|Sort-Object {$_.Length},{$_}}
    $TemplateObject | Add-Member -MemberType AliasProperty -Name Values -Value SortedPatterns
    $TemplateObject | Add-Member -MemberType ScriptProperty -Name MinLength -Value {[int]($this.SortedPatterns | Select-Object -First 1 | Select-Object -ExpandProperty Length)}
    $TemplateObject | Add-Member -MemberType ScriptProperty -Name MaxLength -Value {[int]($this.SortedPatterns | Select-Object -Last 1 | Select-Object -ExpandProperty Length)}
    $TemplateObject | Add-Member -MemberType ScriptMethod -Name ValuesByLength -Value {param([Parameter(Mandatory)][int]$Length) $this.SortedPatterns|Where-Object{$_.Length -eq $Length}}
    $TemplateObject | Add-Member -MemberType ScriptMethod -Name CountByLength -Value {param([Parameter(Mandatory)][int]$Length) $this.ValuesByLength($Length)|Measure-Object|Select-Object -ExpandProperty Count}
    $TemplateObject | Add-Member -MemberType ScriptMethod -Name LengthList -Value {[int[]]($this.Patterns.Length | Sort-Object -Unique)}
    $AllPatterns = [System.Collections.Generic.HashSet[string]]::new()
}
Process {
    if([string]::IsNullOrEmpty($PatternList)) {
        Write-Verbose "Pattern is null or empty"
        return
    }
    $PatternList | ForEach-Object {
        $AllPatterns.Add($_) | Out-Null
    }
}
End {
    $AllPatterns | Sort-Object {$_.Length},{$_} | Foreach-Object {
        $TemplateObject.Patterns.Add($_) | Out-Null
    }
    return $TemplateObject
}