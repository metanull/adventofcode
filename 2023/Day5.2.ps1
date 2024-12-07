# https://adventofcode.com/2023/day/5
Process {
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