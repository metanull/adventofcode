# https://adventofcode.com/{YEAR}/day/{DAY}[#part2]
Process {
    $InputData | ForEach-Object {
        Write-Warning $_
        Write-Warning ((Expand -Line $_ ) -join '')
        Write-Warning ((ExpandDefrag -Line $_ ) -join '')
    }
}
Begin {

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