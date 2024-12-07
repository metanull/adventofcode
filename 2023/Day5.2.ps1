# https://adventofcode.com/2023/day/5
[CmdletBinding()]
param ()
Process {
    "`n`n`n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`n`n`n" | Write-Host -ForegroundColor Magenta

    $Order = @('seed','soil','fertilizer','water','light','temperature','humidity','location')
    
    # Get two maps, filling blanks
    $Max = [Math]::pow(2,32) # ([int32]::MaxValue+1)*2 # 4294967296 # [int64]::MaxValue
    $PairOfMaps = [pscustomobject]@{Left=@{};Right=@()}
    $Index = 0
    for($n=0; $n -lt 2; $n++) {
        $Index = $Index + $n
        $TempMap = Get-MappedRanges -Maps $Maps -SourceName ($Order[$Index]) -DestinationName ($Order[$Index + 1]) | Sort-Object start
        if($n -eq 0) {
            $PairOfMaps.Left = Fill-Gaps -TempMap $TempMap -Index $Index -Max $Max
        } else {
            $PairOfMaps.Right = Fill-Gaps -TempMap $TempMap -Index $Index -Max $Max
        }
    }

    "LEFT" | Write-Warning
    $PairOfMaps.Left | ft
    "RIGHT" | Write-Warning
    $PairOfMaps.Right | ft

    # Merge the two maps
    $CurrentLeftMapEntry = $null
    for($l = 0; $l -lt $PairOfMaps.Left.Count; $l ++) {

        if($null -eq $CurrentLeftMapEntry) {
            "Taking a new entry from the left map" | Write-Warning
            $CurrentLeftMapEntry = $PairOfMaps.Left[$l]
        } else {
            "Keep going with the existing entry from the left map" | Write-Warning
        }

        @("----------------------------------"
        "- CURRENT LEFT MAP ENTRY (Index: $l):"
        "- Start: $($CurrentLeftMapEntry.start)"
        "- End: $($CurrentLeftMapEntry.end)"
        "- Offset: $($CurrentLeftMapEntry.offset)"
        "- OffStart: $($CurrentLeftMapEntry.offstart)"
        "- OffEnd: $($CurrentLeftMapEntry.offend)"
        "----------------------------------") -join "`n" | Write-Warning

        # Look for the start of the right map
        $RightStartIndex = $null
        for($r = 0; $r -lt $PairOfMaps.Right.Count; $r ++) {
            if(     $CurrentLeftMapEntry.offstart -ge $PairOfMaps.Right[$r].start `
                -and $CurrentLeftMapEntry.offstart -lt $PairOfMaps.Right[$r].end 
            ) {
                $RightStartIndex = $r
                break;
            }
        }
        if($null -eq $RightStartIndex) {
            throw "No right map entry found for $($CurrentLeftMapEntry.offstart)"
        }
        "Right map entry found for $($CurrentLeftMapEntry.offstart) at $RightStartIndex" | Write-Warning
        "Left map entry requires $($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start) items" | Write-Warning
        "Right map entry can provide $($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) items" | Write-Warning

        @("   ----------------------------------"
        "   - CURRENT RIGHT MAP ENTRY (Index: $r):"
        "   - Start: $($PairOfMaps.Right[$RightStartIndex].start)"
        "   - End: $($PairOfMaps.Right[$RightStartIndex].end)"
        "   - Offset: $($PairOfMaps.Right[$RightStartIndex].offset)"
        "   - OffStart: $($PairOfMaps.Right[$RightStartIndex].offstart)"
        "   - OffEnd: $($PairOfMaps.Right[$RightStartIndex].offend)"
        "   ----------------------------------") -join "`n" | Write-Warning
        
        # Take what we have
        # "Take what we have" | Write-Warning
        $Slice = $CurrentLeftMapEntry.PSObject.Copy()
        $Slice.start = $CurrentLeftMapEntry.start
        $Slice.end = $Slice.start + [Math]::Min(
            ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start),
            ($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start)
        )
        $Slice.offset = $PairOfMaps.Right[$RightStartIndex].offset
        $Slice.offstart = $Slice.start + $Slice.Offset
        $Slice.offend = $Slice.end + $Slice.Offset
        $Slice | Write-Output

        @("#  ----------------------------------"
        "#  - NEW SLICE:"
        "#  - Start: $($Slice.start)"
        "#  - End: $($Slice.end)"
        "#  - Offset: $($Slice.offset)"
        "#  - OffStart: $($Slice.offstart)"
        "#  - OffEnd: $($Slice.offend)"
        "#  ----------------------------------") -join "`n" | Write-Host -ForegroundColor Cyan

        if( ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) - ($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start) -ge 0) {
            if( ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) - ($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start) -gt 0) {
                "`n`nWe have in EXCESS $(($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) - ($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start))`n`n" | Write-Warning

                #Not a problem, move next
                $CurrentLeftMapEntry = $null
                continue
            } else {
                "Right map can provide just enough items" | Write-Warning
                # nothing else to do here, continue with the next left map entry
                $CurrentLeftMapEntry = $null
                continue
            }
        } else {
            "Right map can't provide enough items" | Write-Warning
            "We miss $($($CurrentLeftMapEntry.end - $CurrentLeftMapEntry.start) - ($Slice.end - $Slice.start))" | Write-Warning
            # we need to take more from the right map, starting from where we were.
            $l = $l - 1     # we need to keep on with the current left map entry
            
            # Adjust the values to continue from where we were

            "Continuing from where we were, for the next $($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) items" | Write-Host -ForegroundColor Cyan
            $CurrentLeftMapEntry.start += (($Slice.end - $Slice.start)) 
            $CurrentLeftMapEntry.end =  $CurrentLeftMapEntry.start + ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start)
            $CurrentLeftMapEntry.offstart += (($Slice.end - $Slice.start)) 
            $CurrentLeftMapEntry.offend =  $CurrentLeftMapEntry.offstart + ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start)
# PROGRES MAIS PAS ENCOR EBON
            continue
        }
    }

    <#
    $Lowest = $null
    $n = $Seeds.Length
    for($ks = 0; $ks -lt $n; $ks+=2) {
        "#$($ks/2) * $($Seeds[$ks +1])" | Write-Host
        # "$ks, $($Seeds[$ks]) $($Seeds[$ks+1])" | Write-Warning
        for($t = 0; $t -lt ($Seeds[$ks +1]); $t++) {
            # "$ks $t $($Seeds[$ks] + $t)" | Write-Warning
            $Seed = $Seeds[$ks] + $t
            $Location = ConvertFrom-SeedToLocation -Maps $Maps -SeedValue $Seed
            # "Seed: $Seed -> Location: $Location" | Write-Output
            if($Lowest -eq $null -or $Location -lt $Lowest) {
                $Lowest = $Location
                "New Lowest Location: $Lowest" | Write-Output
            }
        }
    }
        "Lowest Location: $Lowest" | Write-Output
    #>
}
Begin {
    $Year = 2023
    $Day = 5
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    $Seeds = [long[]]($InputData[0] -replace '^seeds: ','' -split ' ' | ForEach-Object { [long]$_ })
    $Label = $null
    $Maps = @{}
    for($y=2;$y -lt $InputData.Length;$y++) {
        $Line = $InputData[$y]
        if($Line -eq '') {
            $Label = $null
            continue
        }
        if($Line -match '^(.*)\s+map:$') {
            $Label = $Matches[1]
            $Maps += @{$Label = @()}
            continue
        }
        if($Line -match '^(\d+)\s+(\d+)\s+(\d+)$') {
            $Maps.$Label += ,[PSCustomObject]@{
                destination = [long]$Matches[1]
                source = [long]$Matches[2]
                length = [long]$Matches[3]
            }
        } else {
            throw "Unexpected line: $Line"
        }
    }

    Function Fill-Gaps {
        param(
            [Alias('Map')]
            [object[]]$TempMap,
            [long]$Index,
            [long]$Max = [Math]::pow(2,32)
        )
        $prev = 0
        for($i=0;$i -lt $TempMap.Count;$i++) {
            # Write-Warning "$i Prev:$Prev; Start:$($TempMap[$i].start); End:$($TempMap[$i].end)"
            if($TempMap[$i].start -ne $prev) {
                # Write-Warning "$i Filling GAP:  Start:$($prev); End:$($TempMap[$i].start); Offset:0)"
                [pscustomobject]@{start = $prev;end = $TempMap[$i].start;offset = $null;offstart = $prev;offend = $TempMap[$i].start} | Write-Output 
                $prev = $TempMap[$i].start
                if($Prev -ne 0) {
                    # Filling the gap didn't reach the current's start, so we need to repeat the current
                    # Write-Warning "REPEAT"
                    $i --
                }
            } else {
                # Write-Warning "$i Adding AS IS: Start:$($TempMap[$i].start); End:$($TempMap[$i].end); Offset:$($TempMap[$i].offset)"
                $TempMap[$i] | Write-Output
                $prev = $TempMap[$i].end
            }
            if($i -eq ($TempMap.Count -1) -and $TempMap[$i].end -lt $Max) {
                # Write-Warning "$i Adding LAST:  Start:$($TempMap[$i].end); End:$($Max); Offset:0)"
                [pscustomobject]@{start = $TempMap[$i].end;end = $Max;offset = $null;offstart = $TempMap[$i].end;offend = $Max} | Write-Output
                $prev = $TempMap[$i].start
            }
        }
    }

    Function Get-MappedRanges {
        param(
            [hashtable]$Maps,
            [string]$SourceName,
            [string]$DestinationName,
            [switch]$Unmapped
        )
        $Index = "$($SourceName)-to-$($DestinationName)"
        if(-not ($Maps.ContainsKey($Index))) {
            throw "No map found for $Index"
        }
        $MappedRanges = @()
        for($i = 0; $i -lt $Maps.$Index.Count ; $i++) {
            $MappedRanges += ,[pscustomobject]@{
                start=$Maps.$Index[$i].source
                end=$Maps.$Index[$i].source + $Maps.$Index[$i].length
                offset=$Maps.$Index[$i].destination-$Maps.$Index[$i].source
                offstart=$Maps.$Index[$i].destination
                offend=$Maps.$Index[$i].destination + $Maps.$Index[$i].length
            }
        }
        return $MappedRanges
    }

    Function ConvertFrom-MapIndex {
        param(
            [hashtable]$Maps,
            [string]$SourceName,
            [string]$DestinationName,
            [long]$SourceValue
        )
        $Index = "$($SourceName)-to-$($DestinationName)"
        if(-not ($Maps.ContainsKey($Index))) {
            throw "No map found for $Index"
        }
        for($i = 0; $i -lt $Maps.$Index.Count ; $i++) {
            if( $SourceValue -ge $Maps.$Index[$i].source -and $SourceValue -lt ($Maps.$Index[$i].source + $Maps.$Index[$i].length)) {
                return $Maps.$Index[$i].destination + $SourceValue - $Maps.$Index[$i].source
            }
        }
        return $SourceValue
    }

    Function ConvertFrom-SeedToLocation {
        param(
            [hashtable]$Maps,
            [long]$SeedValue
        )
        $SoilValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'seed' -DestinationName 'soil' -SourceValue $SeedValue
        $FertilizerValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'soil' -DestinationName 'fertilizer' -SourceValue $SoilValue
        $WaterValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'fertilizer' -DestinationName 'water' -SourceValue $FertilizerValue
        $LightValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'water' -DestinationName 'light' -SourceValue $WaterValue
        $TemperatureValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'light' -DestinationName 'temperature' -SourceValue $LightValue
        $HumidityValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'temperature' -DestinationName 'humidity' -SourceValue $TemperatureValue
        $LocationValue = ConvertFrom-MapIndex -Maps $Maps -SourceName 'humidity' -DestinationName 'location' -SourceValue $HumidityValue
        return $LocationValue
    }
    
}