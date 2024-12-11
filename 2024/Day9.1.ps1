# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    $InputData | ForEach-Object {
        Write-Warning $_
        # Write-Warning ((Expand -Line $_ ) -join '')
        # Write-Warning ((ExpandDefrag -Line $_ ) -join '')

        $CharArray = $_.ToCharArray()
        Write-Warning ((Defrag -CharArray $CharArray) -join '')
        # Write-Warning ((Expand -Line $_ ) -join '')
    }
}
Begin {

    Function Defrag {
        [CmdletBinding()]
        param([object[]]$CharArray)

        "---" | Write-Warning
        $CharArray -join ''| Write-Warning

        $freeIndex = $null
        $lastBlock = $CharArray.Length - ($CharArray.Length % 2)
        for($blockIndex = $lastBlock; $blockIndex -ge 0 -and $blockIndex -gt $freeIndex; $blockIndex -=2) {
            Write-Warning "BLOCK $BlockIndex $($CharArray[$blockIndex])"
            if([int]($CharArray[$blockIndex]) -gt 0) {
                $blockSize = [int]"$($CharArray[$blockIndex])"
                Write-Warning "BLOCKSIZE $BlockSize"
                if($blockSize -gt 0) {
                    for($freeIndex = 1; $freeIndex -lt $CharArray.Length; $freeIndex ++ ) {
                        Write-Warning "FREE $FreeIndex  $($CharArray[$FreeIndex])"
                        $freeSize = [int]"$($CharArray[$freeIndex])"
                        Write-Warning "FREESIZE $freeSize"
                        if($freeSize -eq 0) {
                            Write-Warning "Empty free space"
                            continue
                        } else {
                            if($freeIndex -le 0) {
                                throw 'Invalid index for free block'
                            }
                            Write-Warning "Defrag"

                            #$CharArray -join ':'| Write-Warning
                            $CharArray[$freeIndex - 1] = [char]([int]($CharArray[$freeIndex - 1]) + 1)
                            $CharArray[$freeIndex] = [char]([int]($CharArray[$freeIndex - 1]) - 1)
                            $CharArray[$blockIndex] = [char]([int]($CharArray[$blockIndex]) - 1)
                            #$CharArray | Write-Warning
                            # Write-Warning (($CharArray|%{"$([int]($_))"})  -Join '-')
                            $CharArray -join ''| Write-Warning
                            # return
                        }
                    }
                }
            }
        }

        return $CharArray
    }
    # REDO: using recursive (fix the first, then recurse)
    Function ExpandDefrag {
        [CmdletBinding()]
        param([String]$Line)

        $Expanded = ((Expand -Line $Line) -join '').ToCharArray()
        $end = $Expanded.Length - 1
        $k = $end
        for($j = 0; $j -lt $Expanded.Length -and $j -lt $k; $j++) {
            write-debug "J: $j = $($Expanded[$j])"
            if($Expanded[$j] -eq '.') {
                for($k = $end; $k -gt 0; $k --) {
                    write-debug "K: $k = $($Expanded[$k])"
                    $end = $k
                    if($Expanded[$k] -ne '.') {
                        write-debug "SWAP: $j $k"
                        $Expanded[$j],$Expanded[$k] = $Expanded[$k],$Expanded[$j]
                        Write-Debug ($Expanded -join '')
                        break
                    }
                }
            }
        }
        return $Expanded
    }
    Function Expand {
        [CmdletBinding()]
        param([String]$Line)
        for($i=0;$i -lt $Line.Length; $i+=2) {
            $FileN = ($i/2)
            $Block = [int]("$($Line[$i])")
            Write-Debug "$FileN,  $Block"
            for($k=0; $k -lt $Block; $k++) {
                $FileN | Write-Output
            }
            if($i -lt ($Line.Length +1) ){
                $Empty = [int]("$($Line[$i+1])")
                Write-Debug ".,  $Empty"
                for($k=0; $k -lt $Empty; $k++) {
                    '.' | Write-Output
                }
            }
        }
    }

    $Year = [int]$(Get-Date -Format 'yyyy')
    $Day = [int](Get-Date -Format 'dd')
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
}