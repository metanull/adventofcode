Process {
    
    # Process all the Input
    $Result = $needles | ForEach-Object -begin { 
        $percent_current = 0
        $percent_total = $needles.Count
        $progress = @{
            Status = 'Testing inputs'
            Activity = 'AdventOfCode 2024-19.1' 
        } 
    } -end { 
        Write-Progress @progress -Completed 
    } -process {
        Write-Progress @progress -CurrentOperation $_ -PercentComplete ($percent_current / $percent_total * 100)
        $needle = $_
        [pscustomobject]@{
            N = $percent_current++
            Needle = $needle 
            Status = (aoc2024_19_1 -needle $needle -layers $layers)
        } | Write-Output
    }
    
    # Display the figures:
    $Result | Group-Object Status

}
Begin {
    function aoc2024_19_1 {
        param(
            $needle,
            $layers,
            $level = $null
        )
        # compute some valuable values 
        $max = ($layers.ItemLength | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum )
        $min = ($layers.ItemLength | Measure-Object -Minimum | Select-Object -ExpandProperty Minimum )
        if ($null -eq $level) {
            $level = $max   # if not set, set l to the max allowed value
        }

        # out of bounds => fail
        if ($level -lt ($min)) { 
            return $false
        }
        # not enough chars, reduce $level
        if ($needle.Length -lt ($level)) {  
            return (aoc2024_19_1 -needle $needle -layers $layers -level $needle.Length)
        }

        # Find substring in available patterns
        $ss_end = $needle.Substring($needle.Length - $level)
        $ss_match = $layers | Where-Object { $_.ItemLength -eq $level } | ForEach-Object {
            $_.items | Where-Object {
                $_ -eq $ss_end
            }
        }
        # match found
        if ($ss_match) {    
            if ($ss_match.Length -eq $needle.Length) {  # the whole input was processeed, done for good
                return $true
            }
            # there is more to process, move next
            if (aoc2024_19_1 -needle $needle.Substring(0, $needle.Length - $level) -layers $layers) {
                return $true    # Analysis of the remaining string = success, done for good
            }
            # analysis of remaining string failed
            # let it try something else...
        }
        # no match, or analysis of remaining failed, try something else
        return (aoc2024_19_1 -needle $needle -layers $layers -level ($level - 1))
    }
    
    # COPY PASTE YOUR INPUT's FIRST LINE HERE:
    $patterns = @"
r, wr, b, g, bwu, rb, gb, br
"@
    # COPY PASTE YOUR INPUT HERE:
    $needles = @"
brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb
"@

    $patterns = [System.Collections.ArrayList]($patterns -split ', ' | where-object { -not [string]::IsNullOrEmpty($_) } | sort-object { $_.Length })
    $needles = [System.Collections.ArrayList]($needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })
    $min = $patterns | Select-Object -first 1 | ForEach-Object { $_.Length }
    $max = $patterns | Select-Object -last 1 | ForEach-Object { $_.Length }
    $layers = $min..$max | foreach-object {
        $length = $_
        $layer = [pscustomobject]@{
            ItemLength   = $length
            Items        = $patterns | Where-Object { $_.Length -eq $length } | Sort-Object
        }
        $layer | Write-Output
    }
}

