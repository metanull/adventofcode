Process {
    $DiskImage = ($InputData | ExpandImage | %{if($null -eq $_){'.'}else{$_}}) -join ''

    $d = $DiskImage | DefragOneBlock
    $b = BlockToImage -BlockList $d
    $p = $b
    while($true){
        $d = $b | DefragOneBlock
        $b = BlockToImage -BlockList $d
        $b
        if($b -eq $p) {
            break
        }
        $p = $b.PSObject.Copy()
    }
}
Begin {
    Function BlockToImage {
        [OutputType([string])]
        [CmdletBinding()]
        param(
            [Parameter(Mandatory)]
            [object[]]$BlockList
        )
        ($BlockList |% {$_.Value}) -join ''
    }
    Function DefragOneBlock {
        [OutputType([object[]])]
        [CmdletBinding()]
        param(
            [Parameter(Mandatory,ValueFromPipeline)]
            [String]$DiskImage
        )

        $BlockExpr = [regex]::new('(?<block>(\d|\.)(\1*))')
        $BlockList = $BlockExpr.Matches($DiskImage) | Select-Object Success,Index,Value,Length
        for($z = $BlockList.count; $z -gt 0; $z --) {
            if($BlockList[$z].Value -notmatch '^\d+$') {
                # Not a File block, skip
                continue
            }
            for($a = 0; $a -lt $BlockList.count; $a ++) {
                if($BlockList[$a].Value -notmatch '^\.+$') {
                    # Not a Free block, skip
                    continue
                }
                if($BlockList[$a].Length -lt $BlockList[$z].Length) {
                    # Free block is not large enough
                    continue
                }
                # Move the block
                $RoomLeft = 0
                for($m = 0; $m -lt $BlockList.count; $m ++) {
                    if($m -eq $a) {
                        # Handle the ex-empty block
                        $RoomLeft = $BlockList[$a].Length - $BlockList[$z].Length
                        $Block = $BlockList[$a] | Select-Object Success,Index,Value,Length
                        $Block.Length = $BlockList[$z].Length
                        $Block.Value = ''.PadLeft($BlockList[$z].Length,($BlockList[$z].Value[0]))
                        $Block | Write-Output
                        if($RoomLeft -gt 0) {
                            $Block = $BlockList[$a] | Select-Object Success,Index,Value,Length
                            $Block.Length = $RoomLeft
                            $Block.Value = ''.PadLeft($RoomLeft,'.')
                            $Block | Write-Output
                        }
                    } elseif($m -eq $z) {
                        # Handle the ex-file block
                        $Block = $BlockList[$z] | Select-Object Success,Index,Value,Length
                        $Block.Value = ''.PadLeft($Block.Length,'.')
                        $Block | Write-Output
                        return
                    } else {
                        $Block = $BlockList[$m] | Select-Object Success,Index,Value,Length
                        $Block | Write-Output
                    }
                }
                return
            }
        }

    }
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