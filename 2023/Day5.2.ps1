# https://adventofcode.com/2023/day/5
[CmdletBinding()]
param ()
Process {
    $Order = @('seed','soil','fertilizer','water','light','temperature','humidity','location')
    
        $Index = 0
        $TempMap = Get-MappedRanges -Maps $Maps -SourceName ($Order[$Index]) -DestinationName ($Order[$Index + 1]) | Sort-Object start
        # Fill the gaps
        $prev = 0
        $MapLeft = for($i=0;$i -lt $TempMap.Count;$i++) {
            if($TempMap[$i].start -ne $prev) {
                [pscustomobject]@{start = $prev;end = $TempMap[$i].start;offset = 0;offstart = $prev;offend = $TempMap[$i].start}
            } else {
                $TempMap[$i]
            }
            if($i -eq ($TempMap.Count -1) -and $TempMap[$i].end -lt 1000000000000000) {
                [pscustomobject]@{start = $TempMap[$i].end;end = 1000000000000000;offset = 0;offstart = $TempMap[$i].end;offend = 1000000000000000}
            }
            $prev = $TempMap[$i].end
        }
        

        $Index = 1
        $TempMap = Get-MappedRanges -Maps $Maps -SourceName ($Order[$Index]) -DestinationName ($Order[$Index + 1]) | Sort-Object start
        # Fill the gaps
        $prev = 0
        $MapRight = for($i=0;$i -lt $TempMap.Count;$i++) {
            if($TempMap[$i].start -ne $prev) {
                [pscustomobject]@{start = $prev;end = $TempMap[$i].start;offset = 0;offstart = $prev;offend = $TempMap[$i].start}
            } else {
                $TempMap[$i]
            }
            if($i -eq ($TempMap.Count -1) -and $TempMap[$i].end -lt 1000000000000000) {
                [pscustomobject]@{start = $TempMap[$i].end;end = 1000000000000000;offset = 0;offstart = $TempMap[$i].end;offend = 1000000000000000}
            }
            $prev = $TempMap[$i].end
        }

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