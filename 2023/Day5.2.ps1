# https://adventofcode.com/2023/day/5#part2
[CmdletBinding()]
param ()
Process {
    $Data = @{
        Seeds = @()
        Order = @()
        Maps = $null
    }
    Read-InputMaps -InputPath $InputPath -Data ([ref]$Data) | Out-Null
    $Map = Get-SimplifiedMap -Maps $Data.Maps -Order $Data.Order -Max ([Math]::pow(2,32))
    
    # Part TWO: seeds is not a simple list, but a list of ranges
    $Locations = for($s = 0; $s -lt $Data.Seeds.Count; $s += 2) {
        $Range = @{start = $Data.Seeds[$s]; end = $Data.Seeds[$s] + $Data.Seeds[$s+1]}
        while($null -ne $Range) {
            $SeedInRange = $Range.start
            for($i = 0; $i -lt $Map.Count; $i ++) {
                if($SeedInRange -ge $Map[$i].start -and $SeedInRange -lt $Map[$i].end) {
                    # Found the first seed in range
                    # Send it's location down to the pipeline (we care only about the "smallest location")
                    # thus no need to send the whole range
                    $Location = $Map[$i].offstart + ($SeedInRange - $Map[$i].start)
                    $Location | write-Output

                    # Does the map entry cover the whole range of seeds?
                    if(($Range.end -lt $Map[$i].end)) {
                        # Yes, we can skip the rest of the seeds in the range, as we have the smallest already
                        $Range = $null
                        break
                    } else {
                        # No, we need to lookup for other entries in the map for the remaining of the range
                        $Range.start = $($Map[$i].end)
                        break
                    }
                }
            }
        }
    }
    $Locations | Measure-Object -Minimum -Maximum
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

    

    ##
    
    Function Get-SimplifiedMap {
        <#
        .SYNOPSIS
            Get the simplified map
        .DESCRIPTION
            Get the simplified map by combining all the maps
        .PARAMETER Maps
            The hashtable containing the maps (Coming from the INPUT file)
        .PARAMETER Order
            The order of the maps
        .PARAMETER Max  
            The maximum number of values of the map (2^32 by default)
        .EXAMPLE
            Get-SimplifiedMap
        #>
        [CmdletBinding()]
        param(
            $Maps,
            $Order,
            $Max = [Math]::pow(2,32)
        )
        if($Order.Count -lt 2) {
            throw "Not enough maps to combine"
        }
        # Get the left most map
        $Map = Get-MappedRangesByName -Maps $Maps -SourceName ($Order[0]) -DestinationName ($Order[1]) | Sort-Object start
        $Left = Repair-ContinuousRange -Map $Map -Max $Max | Sort-Object offstart
        for($x=0; $x -lt $Order.Count -2; $x ++ ) {
            # Combine the left map with the right map
            Write-Debug "$($Order[0])-$($Order[$x+2])"
            $Map = Get-MappedRangesByName -Maps $Maps -SourceName ($Order[$x + 1]) -DestinationName ($Order[$x + 2]) | Sort-Object start
            $Right = Repair-ContinuousRange -Map $Map -Max $Max | Sort-Object start
            $Combined = Join-AdjacentMaps -Left $Left -Right $Right | Sort-Object offstart
            # Replace the left map by the combined map created from left and right maps
            $Left = $Combined
        }
        # Send the combined map to the output
        $Left | Write-Output
    }

    Function Join-AdjacentMaps {
        <#
        .SYNOPSIS
            Combine two adjacent maps into a single one
        .DESCRIPTION
            Combine two adjacent maps into a single one
        .PARAMETER Left
            The left map
        .PARAMETER Right
            The right map
        .EXAMPLE
            Join-AdjacentMaps -Left $Left -Right $Right
        #>
        param(
            [object[]]$Left,
            [object[]]$Right
        )
        # Join the two maps
        $kl,$kr = 0,0
        while($kl -lt $Left.Count -and $kr -lt $Right.Count) {
            Write-Debug "($kl,$kr)`t$($Left[$kl].start)-$($Left[$kl].end)`t => $($Left[$kl].offstart)-$($Left[$kl].offend)`t <=> $($Right[$kr].start)-$($Right[$kr].end)`t => $($Right[$kr].offstart)-$($Right[$kr].offend)"
            if($Left[$kl].offend -lt $Right[$kr].end) {
                Write-Debug "Left has less than Right"
                # Send the map entry to the pipeline
                [pscustomobject]@{
                    start = $Left[$kl].start
                    end = $Left[$kl].start + ($Left[$kl].end - $Left[$kl].start)
                    offset = $Right[$kr].offset
                    offstart = $Right[$kr].offstart
                    offend = $Right[$kr].offstart + ($Left[$kl].end - $Left[$kl].start)
                } | Write-Output
                # Adjust the right map entry, and proceed to the next left map entry
                $Right[$kr].start += ($Left[$kl].end - $Left[$kl].start)
                $Right[$kr].offstart += ($Left[$kl].end - $Left[$kl].start)
                $kl++
                Write-Debug "NEW ($kl,$kr)`t$($Left[$kl].start)-$($Left[$kl].end)`t => $($Left[$kl].offstart)-$($Left[$kl].offend)`t <=> $($Right[$kr].start)-$($Right[$kr].end)`t => $($Right[$kr].offstart)-$($Right[$kr].offend)"
            }elseif($Left[$kl].offend -gt $Right[$kr].end) {
                Write-Debug "Left has more than Right"
                # Send the map entry to the pipeline
                [pscustomobject]@{
                    start = $Left[$kl].start
                    end = $Left[$kl].start + ($Right[$kr].end - $Right[$kr].start)
                    offset = $Right[$kr].offset
                    offstart = $Right[$kr].offstart
                    offend = $Right[$kr].offstart + ($Right[$kr].end - $Right[$kr].start)
                } | Write-Output
                # Adjust the left map entry, and proceed to the next right map entry
                $Left[$kl].start += ($Right[$kr].end - $Right[$kr].start)
                $Left[$kl].offstart += ($Right[$kr].end - $Right[$kr].start)
                $kr++
                Write-Debug "NEW ($kl,$kr)`t$($Left[$kl].start)-$($Left[$kl].end)`t => $($Left[$kl].offstart)-$($Left[$kl].offend)`t <=> $($Right[$kr].start)-$($Right[$kr].end)`t => $($Right[$kr].offstart)-$($Right[$kr].offend)"
            } else {
                Write-Debug "Left has the same as Right"
                # Send the map entry to the pipeline
                [pscustomobject]@{
                    start = $Left[$kl].start
                    end = $Left[$kl].start + ($Left[$kl].end - $Left[$kl].start)
                    offset = $Right[$kr].offset
                    offstart = $Right[$kr].offstart
                    offend = $Right[$kr].offstart + ($Right[$kr].end - $Right[$kr].start)
                } | Write-Output
                # Proceed to the next left and right map entries
                $kl++
                $kr++
                Write-Debug "NEW ($kl,$kr)`t$($Left[$kl].start)-$($Left[$kl].end)`t => $($Left[$kl].offstart)-$($Left[$kl].offend)`t <=> $($Right[$kr].start)-$($Right[$kr].end)`t => $($Right[$kr].offstart)-$($Right[$kr].offend)"
            }
        }
    }
    Function Repair-ContinuousRange {
        <#
        .SYNOPSIS
            Fill the gaps in the map
        .DESCRIPTION
            Fill the gaps in the map (e.g. makes sure that we have a continuous map from 0 to $Max)
        .PARAMETER Map
            The map to fill the gaps
        .PARAMETER Max
            The maximum value of the map (2^32 by default)
        .EXAMPLE
            Repair-ContinuousRange -Map $Map
        #>
        param(
            [object[]]$Map,
            [long]$Max = [Math]::pow(2,32)
        )
        $prev = 0
        for($i=0;$i -lt $Map.Count;$i++) {
            Write-Debug "$i Prev:$Prev; Start:$($Map[$i].start); End:$($Map[$i].end)"
            if($Map[$i].start -ne $prev) {
                Write-Debug "$i Filling GAP:  Start:$($prev); End:$($Map[$i].start); Offset:0)"
                # Send the map entry to the pipeline
                [pscustomobject]@{start = $prev;end = $Map[$i].start;offset = $null;offstart = $prev;offend = $Map[$i].start} | Write-Output 
                $prev = $Map[$i].start
                if($Prev -ne 0) {
                    # Filling the gap didn't reach the current's start, so we need to repeat the current
                    $i --
                }
            } else {
                Write-Debug "$i Adding AS IS: Start:$($Map[$i].start); End:$($Map[$i].end); Offset:$($Map[$i].offset)"
                $Map[$i] | Write-Output
                $prev = $Map[$i].end
            }
            if($i -eq ($Map.Count -1) -and $Map[$i].end -lt $Max) {
                Write-Debug "$i Adding LAST:  Start:$($Map[$i].end); End:$($Max); Offset:0)"
                # Send the map entry to the pipeline
                [pscustomobject]@{start = $Map[$i].end;end = $Max;offset = $null;offstart = $Map[$i].end;offend = $Max} | Write-Output
                $prev = $Map[$i].start
            }
        }
    }

    Function Get-MappedRangesByName {
        <#
        .SYNOPSIS
            Get the mapped ranges from the maps
        .DESCRIPTION
            Get the mapped ranges from the maps by their names "{source}-{destination}"
        .PARAMETER Maps
            The hashtable containing the maps (Coming from the INPUT file)
        .PARAMETER SourceName
            The name of the source 
        .PARAMETER DestinationName
            The name of the destination 
        .EXAMPLE
            Get-MappedRangesByName -Maps $Maps -SourceName 'seed' -DestinationName 'soil'
        #>
        param(
            [hashtable]$Maps,
            [string]$SourceName,
            [string]$DestinationName
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
                offset=$Maps.$Index[$i].destination - $Maps.$Index[$i].source
                offstart=$Maps.$Index[$i].destination
                offend=$Maps.$Index[$i].destination + $Maps.$Index[$i].length
            }
        }
        return $MappedRanges
    }

    Function Read-InputMaps {
        <#
        .SYNOPSIS
            Read the input maps
        .DESCRIPTION
            Read the input maps from the file
        .PARAMETER InputPath
            The path to the input file
        .PARAMETER Data
            The reference to an object or hashtable that receives: Seeds, Order and Maps
        .PARAMETER Order
            The reference to the order array, receives the order of the maps
        #>
        [CmdletBinding()]
        [OutputType([hashtable])]
        param (
            [Parameter(Mandatory)]
            [ValidateScript({Test-Path $_ -PathType Leaf})]
            [string]$InputPath,

            [ref]$Data
        )
        $InputData = Get-Content $InputPath -ErrorAction Stop

        #Load the list of seeds
        $SeedList = [long[]]($InputData[0] -replace '^seeds: ','' -split ' ' | ForEach-Object { [long]$_ })
        $Data.Value.Seeds = $SeedList
        Write-Debug "Seeds: $($Data.Value.Seeds)"

        # Load the Maps from the Input file
        $Data.Value.Order = @()
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
                $Data.Value.Order += ,($Label -replace '-to-.*$','')
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
        $Data.Value.Order += ,($Label -replace '^.*-to-','')
        Write-Debug "Order: $($Data.Value.Order)"

        $Data.Value.Maps = $Maps
        $Maps | Write-Output
    }
  
}