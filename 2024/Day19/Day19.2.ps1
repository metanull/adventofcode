[CmdletBinding()]
param()
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
        Write-Verbose "------ $_"
        Write-Progress @progress -CurrentOperation $_ -PercentComplete ($percent_current / $percent_total * 100)
        $needle = $_
        $ret = [pscustomobject]@{
            N = $percent_current++
            Needle = $needle 
            Status = $null
            Buf = [System.Collections.ArrayList]@()
            All = [System.Collections.ArrayList]@()
        }
        $ret.Status = (aoc2024_19_2 -needle $needle -layers $layers -buf ([ref]$ret.Buf) -all ([ref]$ret.All))
        $ret | Write-Output
    }
    
    # Count the solutions per needle; and calculate there Sum
    $Result | Foreach-Object {
        $_ | Select-Object N,Needle | Add-Member -PassThru -MemberType NoteProperty -Name Solutions -Value $($_.All.Count)
    } | Tee-Object -Variable CountResult
    $CountResult | Measure-Object -Property Solutions -Sum | Select-Object -ExpandProperty Sum | ForEach-Object {
        "Total solutions: $_"
    }

}
Begin {
    function aoc2024_19_2 {
        param(
            $needle,
            $layers,
            $level = $null,
            [ref] $buf,
            [ref] $all
        )
        # compute some valuable values 
        $max = ($layers.ItemLength | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum )
        $min = ($layers.ItemLength | Measure-Object -Minimum | Select-Object -ExpandProperty Minimum )
        if ($null -eq $level) {
            $level = $max   # if not set, set l to the max allowed value
        }

        # out of bounds => fail
        if ($level -lt ($min)) {
            $buf.Value.Clear()
            return $false
        }
        # not enough chars, reduce $level
        if ($needle.Length -lt ($level)) {
            Write-Warning "Needle too short: $needle ($($needle.Length)) < ($($level))"
            return (aoc2024_19_2 -needle $needle -layers $layers -level $needle.Length -buf $buf -all $all)
        }

        # Find substring in available patterns
        $ss_end = $needle.Substring($needle.Length - $level)
        $ss_match = $layers | Where-Object { $_.ItemLength -eq $level } | ForEach-Object {
            $_.items | Where-Object {
                $_ -eq $ss_end
            }
        }
        # match found
        $bufLen = $buf.Count
        if ($ss_match) {
            $bufClone = $buf.Value.Clone()
            $buf.Value.Add($ss_match)

            # If the whole input was processeed...
            if ($ss_match.Length -eq $needle.Length) {
                # we have a solution -> done for good
                $Solution = $buf.Value.Clone()
                $Solution.Reverse()

                #Avoid infinite loop, by testing if $Solution is already in $all
                if ($all.Value | Where-Object { Compare-Object -ReferenceObject ($_ -join ',') -DifferenceObject ($Solution -join ',') -IncludeEqual -ExcludeDifferent }) {
                    Write-Warning "Already found: $($Solution -join ', ')"
                    return $false
                }
                "SOLUTION:      $($Solution -join ', ')" | Write-Warning
                $all.Value.Add($Solution)
                # Find other positive alternatives, using smaller chunks
                for($altLevel = $level - 1; $altLevel -ge $min; $altLevel --) {
                    $altbuf = $bufClone.Clone()
                    $altres = aoc2024_19_2 -needle $needle -layers $layers -buf ([ref]$altbuf) -level $altLevel -all $all
                }
                # /Find ---

                return $true
            }
            # Else, if there is more to process, recurse
            if (aoc2024_19_2 -needle $needle.Substring(0, $needle.Length - $level) -layers $layers -buf $buf -all $all) {
                # Recursion resulted in a solution -> done for good

                # Find other positive alternatives, using smaller chunks
                for($altLevel = $level - 1; $altLevel -ge $min; $altLevel --) {
                    $altbuf = $bufClone.Clone()
                    $altres = aoc2024_19_2 -needle $needle -layers $layers -buf ([ref]$altbuf) -level $altLevel -all $all
                }
                # /Find ---

                return $true    
            }
            # Else, if recursion couldn't find a solution, we have to try something else

            # Remove items form the buffer, as they are not relevant anymore, and continue...
            if($buf.Value.Count -gt $bufLen) {
                $buf.Value.RemoveRange($bufLen, $buf.Value.Count - $bufLen )
            }
        }
        
        # No match, or recursion was not conclusive, look for another option by recursing using a smaller chunk
        return (aoc2024_19_2 -needle $needle -layers $layers -level ($level - 1) -buf $buf -all $all)
    }
    
    if($true) {
        # TEST DATA
        $patterns = @"
r, wr, b, g, bwu, rb, gb, br
"@
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
    } else {
        # REAL DATA
        $patterns = @"
# COPY PASTE YOUR INPUT's FIRST LINE HERE:
"@
        $needles = @"
# COPY PASTE YOUR INPUT HERE:
"@
    }

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

