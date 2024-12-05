# https://adventofcode.com/2023/day/1#part2
Process {
    $RegEx = [Regex]::new('([0-9]|one|two|three|four|five|six|seven|eight|nine)')
    $InputData | ForEach-Object {
        #$_ | Write-Warning
        if(-not $_ -match $RegEx) {
            $_ | Write-Error
            return
        }
        $Line = ($RegEx.Matches($_) | ForEach-Object {
                switch ($_.Groups[1].Value) {
                    'one' { 1 }
                    'two' { 2 }
                    'three' { 3 }
                    'four' { 4 }
                    'five' { 5 }
                    'six' { 6 }
                    'seven' { 7 }
                    'eight' { 8 }
                    'nine' { 9 }
                    default { [int]$_ }
                }
            }) -join ''
        # $Line | Write-Warning
        if($Line -match '^\D*(\d).*(\d)\D*$') {
            $V = "$([int]$matches[1])$([int]$matches[2])"
            $V | Write-Output
        } elseif($Line -match '(\d)') {
            $V = "$([int]$matches[1])$([int]$matches[1])"
            $V | Write-Output
        } else {
            $Line | Write-Error
        }
    } | Measure-Object -Sum
}
Begin {
    $InputPath = Join-Path -Resolve -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')" -ErrorAction Stop
    $InputData = Get-Content $InputPath -ErrorAction Stop
<#
    $InputData = @('two1nine'
'eightwothree'
'abcone2threexyz'
'xtwone3four'
'4nineeightseven2'
'zoneight234'
'7pqrstsixteen'
    )
#>
}