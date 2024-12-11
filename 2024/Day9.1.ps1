Process {
    $DiskImage = $InputData | ExpandImage
    $MirrorImage = $DiskImage.PSObject.Copy()
    [array]::Reverse($MirrorImage)
    $BlockCount = ($DiskImage|?{$null -ne $_}).count

    $CursorIndex = 0
    $FillerIndex = 0
    $ChecksumIndex = 0
    $DiskImage | ForEach-Object {
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
    } | ForEach-Object {
        ($ChecksumIndex ++) * $_
    } | Measure-Object -Sum
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