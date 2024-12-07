# https://adventofcode.com/2023/day/5
[CmdletBinding()]
param ()
Process {
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

    # Merge the two maps
    for($l = 0; $l -lt $PairOfMaps.Left.Count; $l ++) {
        # Look for the start of the right map
        $RightStartIndex = $null
        for($r = 0; $r -lt $PairOfMaps.Right.Count; $r ++) {
            if(     $PairOfMaps.Left[$l].offstart -ge $PairOfMaps.Right[$r].start `
                -and $PairOfMaps.Left[$l].offstart -le $PairOfMaps.Right[$r].end 
            ) {
                $RightStartIndex = $r
                break;
            }
        }
        if($null -eq $RightStartIndex) {
            throw "No right map found for $($PairOfMaps.Left[$l].offstart)"
        }
        "Right map found for $($PairOfMaps.Left[$l].offstart) at $RightStartIndex" | Write-Warning

        "Right map can provide $($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) items" | Write-Warning
        "Left map requires $($PairOfMaps.Left[$l].end - $PairOfMaps.Left[$l].start) items" | Write-Warning
        
        # Take what we have
        $Slice = $PairOfMaps.Left[$l].PSObject.Copy()
        $Slice.start = $PairOfMaps.Left[$l].start
        $Slice.end = $Slice.start + [Math]::Min(
            ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start),
            ($PairOfMaps.Left[$l].end - $PairOfMaps.Left[$l].start)
        )

        $Slice.offset = $PairOfMaps.Right[$RightStartIndex].offset
        $Slice.offstart = $Slice.start + $Slice.Offset
        $Slice.offend = $Slice.end + $Slice.Offset
        $Slice | Write-Output

        # Print what we miss
        "We miss $($($PairOfMaps.Left[$l].end - $PairOfMaps.Left[$l].start) - $Slice.end)" | Write-Warning

        if( ($PairOfMaps.Right[$RightStartIndex].end - $PairOfMaps.Right[$RightStartIndex].start) - ($PairOfMaps.Left[$l].end - $PairOfMaps.Left[$l].start) -ge 0) {
            "Right map can provide enough items" | Write-Warning
            # nothing else to do here, continue with left map
        } else {
            "Right map can't provide enough items" | Write-Warning
            # Take what we have...
            # = repeat mor eor less what we did above
            # ...Then Look for more
            # to do so, it would be interrest to save the current item of the left map in a temp variable
            # so that it can be updated before continuing the loop
        }
    }
<#
    # Merge the two maps
    for($l = 0; $l -lt $PairOfMaps.Left.Count; $l ++) {
        for($r = 0; $r -lt $PairOfMaps.Right.Count; $r ++) {
            if($PairOfMaps.Left[$l].end -le $PairOfMaps.Right[$l].start ) {
                # Direct translation, use the offset from right, with start and end from left
                $Merged += @{
                    start = $PairOfMaps.Left[$l].start
                    end = $PairOfMaps.Left[$l].end
                    offset = $PairOfMaps.Right[$r].offset
                    offstart = $PairOfMaps.Left[$r].offstart + $PairOfMaps.Right[$r].offset
                    offend = $PairOfMaps.Left[$r].offend + $PairOfMaps.Right[$r].offset
                }
            } elseif($PairOfMaps.Left[$l].end -le $PairOfMaps.Right[$l].end) {
                # Section, use a slice of left with a slice of right
                $Merged += @{
                    start = $PairOfMaps.Left[$l].start
                    end = $PairOfMaps.Left[$l].end
                    offset = $PairOfMaps.Right[$r].offset
                    offstart = $PairOfMaps.Left[$r].offstart + $PairOfMaps.Right[$r].offset
                    offend = $PairOfMaps.Left[$r].offend + $PairOfMaps.Right[$r].offset
                }
            }

            if($PairOfMaps.Left[$l].start -le $PairOfMaps.Right[$r].start -and $PairOfMaps.Left[$l].end -ge $PairOfMaps.Right[$r].end) {
                $PairOfMaps.Left[$l].offset = $PairOfMaps.Right[$r].start - $PairOfMaps.Left[$l].start
                $PairOfMaps.Left[$l].offstart = $PairOfMaps.Right[$r].start
                $PairOfMaps.Left[$l].offend = $PairOfMaps.Right[$r].end
            }
            if($PairOfMaps.Right[$r].start -le $PairOfMaps.Left[$l].start -and $PairOfMaps.Right[$r].end -ge $PairOfMaps.Left[$l].end) {
                $PairOfMaps.Right[$r].offset = $PairOfMaps.Left[$l].start - $PairOfMaps.Right[$r].start
                $PairOfMaps.Right[$r].offstart = $PairOfMaps.Left[$l].start
                $PairOfMaps.Right[$r].offend = $PairOfMaps.Left[$l].end
            }
        }
    }
#>
<#
        $BiMap = for($n = 0; $n -lt [Math]::Max($MapLeft.Count, $MapRight.Count); $n++) {
            $obj = [pscustomobject]@{}
            if($n -lt $MapLeft.Count) {
                $obj | Add-Member -NotePropertyName 'leftstart' -NotePropertyValue $MapLeft[$n].start
                $obj | Add-Member -NotePropertyName 'leftend' -NotePropertyValue $MapLeft[$n].end
                $obj | Add-Member -NotePropertyName 'leftoffset' -NotePropertyValue $MapLeft[$n].offset
                $obj | Add-Member -NotePropertyName 'leftoffstart' -NotePropertyValue $MapLeft[$n].offstart
                $obj | Add-Member -NotePropertyName 'leftoffend' -NotePropertyValue $MapLeft[$n].offend
            }
            if($n -lt $MapRight.Count) {
                $obj | Add-Member -NotePropertyName 'rightstart' -NotePropertyValue $MapRight[$n].start
                $obj | Add-Member -NotePropertyName 'rightend' -NotePropertyValue $MapRight[$n].end
                $obj | Add-Member -NotePropertyName 'rightoffset' -NotePropertyValue $MapRight[$n].offset
                $obj | Add-Member -NotePropertyName 'rightoffstart' -NotePropertyValue $MapRight[$n].offstart
                $obj | Add-Member -NotePropertyName 'rightoffend' -NotePropertyValue $MapRight[$n].offend
            }
            $obj | Write-Output
        }
        $BiMap | Write-Output
    #>
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
                [pscustomobject]@{start = $prev;end = $TempMap[$i].start;offset = 0;offstart = $prev;offend = $TempMap[$i].start} | Write-Output 
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
                [pscustomobject]@{start = $TempMap[$i].end;end = $Max;offset = 0;offstart = $TempMap[$i].end;offend = $Max} | Write-Output
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