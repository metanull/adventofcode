Process {
    $DiskImage = $InputData | ExpandImage
    $MirrorImage = $DiskImage.PSObject.Copy()
    [array]::Reverse($MirrorImage)
    $BlockCount = ($DiskImage|?{$null -ne $_}).count

    $PercentMax = $BlockCount
    Write-Progress -Activity 'Defragmenting' -PercentComplete ([Math]::Truncate($BlockCount*$PercentMax) * 100)
    $CursorIndex = 0
    $FillerIndex = 0
    $DiskImage | ForEach-Object {
        Write-Progress -Activity 'Defragmenting' -PercentComplete ([Math]::Truncate($BlockCount*$PercentMax) * 100)
        if($BlockCount -le 0) {
            $null | Write-Output
        } elseif($null -ne $_) {
            $_ | Write-Output
            $BlockCount --
        } else {
            for($n = $FillerIndex; $n -lt $MirrorImage.Count; $n++) {
                if($null -ne $MirrorImage[$n]) {
                    $MirrorImage[$n] | Write-Output
                    $BlockCount --
                    $FillerIndex = $n + 1
                    break
                }
            }
        }
        $CursorIndex ++
    }
    Write-Progress -Activity 'Defragmenting' -Completed
}
Begin {
    Function ExpandImage {
        [OutputType([int[]])]
        [CmdletBinding()]
        param(
            [OutputType([int[]])]
            [Parameter(Mandatory,ValueFromPipeline)]
            [String]$DiskImage
        )
        Begin {
            $Parser = [regex]::new('(?<digit>\d)')
        }
        Process {
            $CursorIndex = 0
            $FileIndex = 0
            $DiskImage | ForEach-Object {
                $Parser.Matches($_) | Where-Object {
                    $_.Success
                } | ForEach-Object {
                    [int]($_.Value) | Write-Output
                }
            } | Foreach-Object {
                $BlockSize = $_
                for($n = 0; $n -lt $BlockSize; $n ++ ) {
                    if($CursorIndex % 2 -eq 0) {
                        $FileIndex | Write-Output
                    } else {
                        $null | Write-Output
                    }
                }
                if($CursorIndex % 2 -ne 0) {
                    $FileIndex ++
                }
                $CursorIndex ++
            }
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