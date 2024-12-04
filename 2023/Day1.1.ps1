Process {
    $RegEx = [Regex]::new('^\D*(\d).*(\d)\D*$')
    $InputData | ForEach-Object {
        $Line = $_
        if($Line -match '^\D*(\d).*(\d)\D*$') {
            $V = "$([int]$matches[1])$([int]$matches[2])"
            $V | Write-Output
        } elseif($Line -match '^\D*(\d)\D*$') {
            $V = "$([int]$matches[1])$([int]$matches[1])"
            $V | Write-Output
        } else {
            $Line | Write-Warning
        }
    } | Measure-Object -Sum
}
Begin {
    $InputPath = Join-Path -Resolve -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')" -ErrorAction Stop
    $InputData = Get-Content $InputPath -ErrorAction Stop
<#
    $InputData = @('1abc2'
    'pqr3stu8vwx'
    'a1b2c3d4e5f'
    'treb7uchet'
    )
#>
}