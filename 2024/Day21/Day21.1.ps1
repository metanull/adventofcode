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

$Keypads = Import-PowerShellDataFile -Path (Join-Path -Path $PSScriptRoot -ChildPath Keypads.psd1)

$Code = @('0','2','9','A')
$Keypads.Numeric.Printable

$CurrentKey = 'A'
$Level1 = foreach($TargetKey in $Code) {
    Write-Warning $TargetKey
    Find-KeyPadPath -UpFirst -KeyPad $Keypads.Numeric.Moves -TargetKey $TargetKey -CurrentKey $CurrentKey | Foreach-Object {
        $_ | Write-Host -ForegroundColor Magenta
        $_ | Write-Output
    }
    'A' | Write-Host -ForegroundColor Cyan
    'A' | Write-Output
    $CurrentKey = $TargetKey
}

$Keypads.Directional.Printable

$CurrentKey = 'A'
$Level2 = foreach($TargetKey in $Level1) {
    Write-Warning $TargetKey
    Find-KeyPadPath -KeyPad $Keypads.Directional.Moves -TargetKey $TargetKey -CurrentKey $CurrentKey| Foreach-Object {
        $_ | Write-Host -ForegroundColor Magenta
        $_ | Write-Output
    }
    'A' | Write-Host -ForegroundColor Cyan
    'A' | Write-Output
    $CurrentKey = $TargetKey
}
