# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    # Extract the '*'' from the input
    $Gears = @()
    $x = 0; $y = 0
    for( $y = 0; $y -lt $InputData.Count; $y++) {
        for( $x = 0; $x -lt $InputData[$y].Length; $x++) {
            if($InputData[$y][$x] -match '\*') {
                # Extract the 7x3 matrix around the '*'
                $Matrix = @(
                    ($InputData[$y-1][($x-3)..($x+3)] -join ''),
                    ($InputData[$y][($x-3)..($x+3)] -join ''),
                    ($InputData[$y+1][($x-3)..($x+3)] -join '')
                )
                $Gears += [pscustomobject]@{X=$x;Y=$Y;Matrix=$Matrix}
            }
        }
    }
    $Gears | Foreach-Object {
        $Gear = $_
        $Values = @()
        # Find all horizontal numbers in the matrix
        $rx = [regex]::new('(\d+)')
        0..2 | Foreach-Object {
            $Line = $_
            $rx.Matches($Gear.Matrix[$Line]) | Foreach-Object {
                $Capture = $_
                $Capture | Where-Object {
                    # For each found number, verify that it is adjacent with the center '*' (e.g check that any of its digits are in the position 2,3 or 4)
                    (2..4 | Foreach-Object { ($Capture.Index)..($Capture.Index + $Capture.Length-1) -contains $_ } | Where-Object {$_}).Length -gt 0
                } | Foreach-Object {
                    # If the number is adjacent, add it to the list of values
                    $Values += ,([pscustomobject]@{X=($Gear.X -3 + $_.Index); Y=($Gear.Y -1 + $Line); Value=[int]$_.Value})
                }
            }
        }
        if($Values.Count -gt 1) {
            # If the 'Gear' has than one adjacent number, pass it over to the next stage in the pipeline
            $Gear.PSObject.Copy() | Add-Member -MemberType NoteProperty -Name Values -Value $Values -PassThru | Write-Output
        }
    } | Foreach-Object {
        if($_.Values.Count -ne 2) {
            throw "Expected 2 adjacent numbers, but found $($_.Values.Count)"
        }
        # If we found more than one number, calculate the product of the two numbers
        $_ | Add-Member -MemberType NoteProperty -Name Product -Value ($_.Values[0].Value * $_.Values[1].Value) -PassThru | Write-Output
    } | Measure-Object -Property Product -Sum   # Finally return the sum of the products

}
Begin {
    $PuzzleUrl = "https://adventofcode.com/$(Get-Date -Format 'yyyy')/day/$([int](Get-Date -Format 'dd'))"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    $Width = $InputData[0].Length
    $Height = $InputData.Count

    $Spacer = (0..($Width+5) | Foreach-Object { '.' }) -join ''
    $SpacerData = @()
    $SpacerData += ,$Spacer
    $InputData | ForEach-Object {
        $SpacerData += ,"...$_..." | Write-Output
    }
    $SpacerData += ,$Spacer
    $InputData = $SpacerData
}