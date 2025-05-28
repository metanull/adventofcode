Function Find-KeyPadPath {
    [CmdletBinding()]
    param(
        [hashtable]$KeyPad,
        [string]$TargetKey,
        [string]$CurrentKey = 'A',
        [switch]$UpFirst
    )
    $Target = $KeyPad[$TargetKey]
    $Current = $KeyPad[$CurrentKey]

    $Vertical = $Target.Vertical - $Current.Vertical
    $VerticalMoves = for($i = 0; $i -lt [Math]::Abs($Vertical); $i++) {
        if($Vertical -gt 0) {
            'Up' | Write-Output
        } else {
            'Down' | Write-Output
        }
    }
    $Horizontal = $Target.Horizontal - $Current.Horizontal
    $HorizontalMoves = for($i = 0; $i -lt [Math]::Abs($Horizontal); $i++) {
        if($Horizontal -gt 0) {
            'Left' | Write-Output
        } else {
            'Right' | Write-Output
        }
    }
    if(($Vertical -ge 0 -and $UpFirst.IsPresent -and $UpFirst) -or ($Vertical -le 0 -and ((-not $UpFirst.IsPresent) -or (-not $UpFirst)))) {
        # Moving away from the deadchar: place vertical moves first (to avoid hitting deadchar)
        $VerticalMoves | ForEach-Object { $_ | Write-Output }
        $HorizontalMoves | ForEach-Object { $_ | Write-Output }
    } else {
        # Moving towards the deadchar: place horizontal moves first (to avoid hitting deadchar)
        $HorizontalMoves | ForEach-Object { $_ | Write-Output }
        $VerticalMoves | ForEach-Object { $_ | Write-Output }
    }
}

Function Invoke-RobotChain {
    [CmdletBinding()]
    Param(
        [System.Collections.ArrayList] $Code
    )
    Begin {
        $Keypads = Import-PowerShellDataFile -Path (Join-Path -Path $PSScriptRoot -ChildPath Keypads.psd1)
    } 
    Process {
        $CurrentKey = 'A'
        $Robot1 = foreach($TargetKey in $Code) {
            Find-KeyPadPath -UpFirst -KeyPad $Keypads.Numeric.Moves -TargetKey $TargetKey -CurrentKey $CurrentKey
            'A' | Write-Output
            $CurrentKey = $TargetKey
        }
        $CurrentKey = 'A'
        $Robot2 = foreach($TargetKey in $Robot1) {
            Find-KeyPadPath -KeyPad $Keypads.Directional.Moves -TargetKey $TargetKey -CurrentKey $CurrentKey
            'A' | Write-Output
            $CurrentKey = $TargetKey
        }
        $CurrentKey = 'A'
        $Robot3 = foreach($TargetKey in $Robot2) {
            Find-KeyPadPath -KeyPad $Keypads.Directional.Moves -TargetKey $TargetKey -CurrentKey $CurrentKey
            'A' | Write-Output
            $CurrentKey = $TargetKey
        }

        $Final = $Robot3
        $ShortestSolutionLength = [int]($Final | Measure-Object | Select-Object -ExpandProperty Count)
        $CodeFingerprint = [int](($Code -join '') -replace '^\D+|\D+$')
        $Solution = $ShortestSolutionLength * $CodeFingerprint

        [PSCustomObject]@{
            Code = $Code -join ''
            CodeValue = $CodeFingerprint
            Solution = ($Final | Foreach-Object {
                switch($_) {
                    'Up' {'^'}
                    'Down' {'v'}
                    'Left' {'<'}
                    'Right' {'>'}
                    default {$_}
                }
            }) -join ''
            SolutionLength = $ShortestSolutionLength
            Complexity = $Solution
        }
    }
}

$Codes = @('029A','980A','179A','456A','379A')
#$Codes = @('780A','539A','341A','189A','682A')
$Solutions = foreach($Code in $Codes) {
    Invoke-RobotChain -Code $Code.ToCharArray()
}
$Solutions | Select-Object -Property Code,CodeValue,SolutionLength,Solution
$Solutions | Measure-Object -Sum -Property Complexity | Select-Object -ExpandProperty Sum

