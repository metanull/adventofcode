# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    # Extract the numbers from the input
    $Parts = @()
    $x = 0; $y = 0
    for( $y = 0; $y -lt $InputData.Count; $y++) {
        for( $x = 0; $x -lt $InputData[$y].Length; $x++) {
            if($InputData[$y][$x] -match '\d') {
                if($InputData[$y].SubString($x) -match '^(?<Number>\d+)') {
                    $Parts += [pscustomobject]@{Number=$matches.Number;X=$x;Y=$Y}
                    $x += $matches.Number.Length
                } else {
                    throw 'sbeuh'
                }
            }
        }
    }

    # Get the rectangle surrounding the Number, looking for symbols other than '.'
    $Gears = @()
    $Gears = $Parts | ForEach-Object {
        $Part = $_
        $w = "$($Part.Number)".Length
        $x1 = $Part.X; $y1 = $Part.Y; $x2 = $Part.X + $w -1; $y2 = $Part.Y
        $sym = ''
        if($x1 -gt 0) {    $x1--}
        if($x2 -lt ($InputData[$Part.Y].Length -1)) { $x2++}
        if($y1 -gt 0) {    $y1--}
        if($y2 -lt ($InputData.Count - 1)) {          $y2++}
        if($y1 -ne $Part.Y) {
            $sym += $InputData[$y1].Substring($x1,$x2-$x1+1)
        }
        if($y2 -ne $Part.Y) {
            $sym += $InputData[$y2].Substring($x1,$x2-$x1+1)
        }
        if($x1 -ne $Part.X) {
            $sym += $InputData[$_.Y].Substring($x1,1)
        }
        if($x2 -ne $Part.X + $w -1) {
            $sym += $InputData[$Part.Y].Substring($x2,1)
        }
        # Add the identified symbols to the Part object
        $Part | Add-Member -NotePropertyName Symbols -NotePropertyValue ([RegEx]::new('([^\.])')).Matches($sym).Value

        # Add the coordinates of the points of the rectanle
        $Part | Add-Member -NotePropertyName Rectangle -NotePropertyValue (($y1)..($y2) | Foreach-Object {
            $y = $_
            ($x1)..($x2) | Foreach-Object {
                $x = $_
                "($x,$y)"
            }
        })
        #  return to the pipeline
        $Part | Write-Output

    } | Where-Object {
        # Keep only Part numbers with a symbol
        $_.Symbols.Count -gt 0
    } | Where-Object {
        # Keep only the "Gears"
        '*' -in $_.Symbols
    }
"Finding Pairs" | Write-Warning
    # Look for pair of gears
    $PairsOfGears = @()
    $Gears | Foreach-Object {
        # $_ | Write-Warning
        $CurrentGear = $_
        $Gears |? {
            # Don't match with self
            $_.X -ne $CurrentGear.X -and $_.Y -ne $CurrentGear.Y
        } |% {
            # $_ | Write-Warning
            $CandidateGear = $_
            # Compute the intersection
            $Intersection = $CurrentGear.Rectangle |? {
                $_ -in $CandidateGear.Rectangle
            } | Foreach-Object {
                #"Testing intersection $_" | Write-Warning
                # Only consider the pair if they have a "*" character in their intersection
                if(-not ($_ -match '^\((\d+),(\d+)\)$')) {
                    throw "Invalid data in intersection: $_"
                }
                $X = $matches[1]
                $Y = $matches[2]
                if($InputData[$Y][$X] -eq '*') {
                    "Gear found in $_" | Write-Warning
                    $_ | Write-Output
                }
            } | Foreach-Object {
                #"Testing pair for existance $($_ -join ',')" | Write-Warning

                # If there is an intersection, add the Pair to the list
                $PairExists = $PairsOfGears |? {
                    # Avoid repeating a pair that was already found
                    ($_.Gear2.X -eq $CurrentGear.X -and $_.Gear2.Y -eq $CurrentGear.Y) -and ($_.Gear1.X -eq $CandidateGear.X -and $_.Gear1.Y -eq $CandidateGear.Y)
                }
                if(-not $PairExists) {
                    # Add the pair
                    "Adding pair for Intersection = $($_ -join ',')" | Write-Warning
                    $Pair = [pscustomobject]@{
                        Gear1 = $CurrentGear
                        Gear2 = $CandidateGear
                        Intersection = $Intersection
                        Power = [int]$CurrentGear.Number * [int]$CandidateGear.Number
                    }
                    $PairsOfGears += ,($Pair)
                }
            }
        }
    }
    <#
    # Remove pairs that do not have a "*" character in their intersection
    $PairsOfGears |? {
        $_.Intersection | Foreach-Object {
            if(-not ($_ -match '^\((\d+),(\d+)\)$')) {
                throw "Invalid data in intersection: $_"
            }
            $X = $matches[1]
            $Y = $matches[2]
            if($InputData[$Y][$X] -eq '*') {
                $true
            }
        }
    } #| Measure-Object Power -Sum
    #>
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

    function Test-Gear {
        param(
            $Matrix,
            $X,$Y
        )
        if($Matrix[$Y][$X] -ne '*') {
            return $false
        }
        $X1=$X;$X2=$X;$Y1=$Y;$Y2=$Y
        if($X -gt 0) {$X1--}
        if($Y -gt 0) {$Y1--}
        if($X -lt ($Matrix[$Y].Length -1)) {$X2++}
        if($Y -lt ($Matrix.Count -1)) {$Y2++}

        if($X1 -eq $X) {
            # No room to the left
        } elseif($Y1 -eq $Y) {
            # No room to the top
        } elseif($X2 -eq $X) {
            # No room to the right
        } elseif($Y2 -eq $Y) {
            # No room to the bottom
        } else {
            <#
       TLLL TLL TL T TR TRR TRRR
        LLL  LL  L *  R  RR  RRR
       BLLL BLL BL B BR BRR BRRR
            #>

            # Look above
            if(<#TL#> $Y -gt 0 -and $X -gt 0 -and ($Matrix[$Y-1][$X-1] -match '\d')) {
                if(<#TLL+TL#> $X -gt 1 -and ($Matrix[$Y-1][$X-2] -match '\d')) {
                    if(<#TLLL+TLL+TL#> $X -gt 2 -and ($Matrix[$Y-1][$X-3] -match '\d')) {
                        $Number="$($Matrix[$Y-1][$X-3])$($Matrix[$Y-1][$X-2])$($Matrix[$Y-1][$X-1])"
                    } elseif(<#TLL+TL+T#>($Matrix[$Y-1][$X] -match '\d')) {
                        $Number="$($Matrix[$Y-1][$X-2])$($Matrix[$Y-1][$X-1])$($Matrix[$Y-1][$X])"
                    } else {
                        <#TLL+TL#>
                        $Number="$($Matrix[$Y-1][$X-2])$($Matrix[$Y-1][$X-1])"
                    }
                } elseif(<#TL+T#> ($Matrix[$Y-1][$X] -match '\d')) {
                    if(<#TL+T+TR#> $X -lt ($Matrix[$Y].Length -1) -and ($Matrix[$Y-1][$X+1] -match '\d')) {
                        $Number="$($Matrix[$Y-1][$X-1])$($Matrix[$Y-1][$X])$($Matrix[$Y-1][$X+1])"
                    } else {
                        <#TL+T#>
                        $Number="$($Matrix[$Y-1][$X-1])$($Matrix[$Y-1][$X])"
                    }
                } else {
                    <#TL#>
                    $Number="$($Matrix[$Y-1][$X-1])"
                }
            } elseif(<#T,NOT(TL)#> $Y -gt 0 -and ($Matrix[$Y-1][$X] -match '\d')) {

            } elseif(<#TR,NOT(T),NOT(TL)#> $Y -gt 0 -and $X -lt (($Matrix[$Y].Length -1)) -and ($Matrix[$Y-1][$X+1] -match '\d')) {

            }
            # Look Left
            if(<#L#> $X -gt 0 -and ($Matrix[$Y][$X-1] -match '\d')) {

            }
            # Look Right
            if(<#R#> $X -lt (($Matrix[$Y].Length -1)) -and ($Matrix[$Y][$X+1] -match '\d')) {

            }
            # Look Below
            if(<#BL#> $Y -lt ($Matrix.Count -1) -and $X -gt 0 -and ($Matrix[$Y+1][$X-1] -match '\d')) {

            } elseif(<#B,NOT(BL)#> $Y -lt ($Matrix.Count -1) -and ($Matrix[$Y+1][$X] -match '\d')) {

            } elseif(<#BR,NOT(B),NOT(BL)#> $Y -lt ($Matrix.Count -1) -and $X -lt (($Matrix[$Y].Length -1)) -and ($Matrix[$Y+1][$X+1] -match '\d')) {

            }

        }
    }
}