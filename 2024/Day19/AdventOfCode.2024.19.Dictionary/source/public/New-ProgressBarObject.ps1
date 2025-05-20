<#
    .SYNOPSIS
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory = $false)]
    [string]$Activity = 'AdventOfCode 2024-19.2'
)
End {
    # Progress Bar
    $TempProgress = [PSCustomObject]@{
        Current = 0
        Total = 1
        Params = @{
            Activity = $Activity
            Status = 'In progress'
            CurrentOperation = 'Initializing'
            Id = 1
        }
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name Reset -Value {
        param([ValidateScript({$_ -gt 0 })][int]$Max) 
        $this.Total = $Max
        $this.Current = 0
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name SetStatus -Value {
        param([string]$Status) 
        $this.Params.Status = $Status
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name SetCurrentOperation -Value {
        param([string]$CurrentOperation) 
        $this.Params.CurrentOperation = $CurrentOperation
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name Refresh -Value {
        $Params = $this.Params
        Write-Progress @Params -PercentComplete ($this.Current / $this.Total * 100)
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name Step -Value {
        param($CurrentOperation)
        $this.Current ++
        $this.Params.CurrentOperation = $CurrentOperation
        $Params = $this.Params
        Write-Progress @Params -PercentComplete ($this.Current / $this.Total * 100)
    }
    $TempProgress | Add-Member -MemberType ScriptMethod -Name Done -Value {
        $Params = $this.Params
        Write-Progress @Params -Completed
    }
    return $TempProgress
}