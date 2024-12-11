# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    
    <#
    $Parser = [regex]::new('(?<digit>\d)')
    $InputData | ForEach-Object {
        Write-Warning $_
        $IntArray = $Parser.Matches($_)|?{$_.Success}|%{[int]$_.Value}
        ($IntArray) -join ','
        (Defrag -DiskImage $IntArray) -join ','
        # Write-Warning ((Expand -Line $_ ) -join '')
        # Write-Warning ((ExpandDefrag -Line $_ ) -join '')
    }#>
    $InputData | ForEach-Object {
        Write-Warning ((Expand -Line $_ ) -join '')
        (ExpandDefrag -Line $_ ) -join ''
    } | Tee-Object -Variable Defragmented
    Write-Warning ((Expand -Line $Defragmented ) -join '')
    $Parser = [regex]::new('(?<digit>\d)')
    $Parser.Matches($Defragmented)|% { $_.Index * $_.Value } | Measure-Object -sum
}
Begin {

    Function Defrag {
        [CmdletBinding()]
        param(
            [Parameter(ValueFromPipeline)]
            [object[]]$DiskImage
        )
        Begin {
            $Array = @()
        }
        Process {
            $Array += ,($_)
        }
        End {
            $freeIndex = $null
            $lastBlock = $DiskImage.Length - ($DiskImage.Length % 2)
            for($blockIndex = $lastBlock; $blockIndex -ge 0 -and $blockIndex -gt $freeIndex; $blockIndex -=2) {
                if([int]($DiskImage[$blockIndex]) -gt 0) {
                    $blockSize = [int]"$($DiskImage[$blockIndex])"
                    if($blockSize -gt 0) {
                        for($freeIndex = 1; $freeIndex -lt $DiskImage.Length; $freeIndex ++ ) {
                            $freeSize = [int]"$($DiskImage[$freeIndex])"
                            if($freeSize -eq 0) {
                                continue
                            } else {
                                if($freeIndex -le 0) {
                                    throw 'Invalid index for free block'
                                }
                                $DiskImage[$freeIndex - 1] = (($DiskImage[$freeIndex - 1]) + 1)
                                $DiskImage[$freeIndex] = (($DiskImage[$freeIndex - 1]) - 1)
                                $DiskImage[$blockIndex] = (($DiskImage[$blockIndex]) - 1)
                            }
                        }
                    }
                }
            }
            return $DiskImage
        }
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

    $Year = 2024
    $Day = 9
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