Process {
    $DiskImage = $InputData

    $DiskImage | Write-Warning

    ExpandImage -InputData $DiskImage | Write-Warning
    '00992111777.44.333....5555.6666.....8888..'|write-warning

    $DiskImageInt = ([char[]]$DiskImage)|%{CtoI($_) }
    $ClonedImage = $DiskImageInt.PSObject.Copy()
    $DiskImageInt -join ','
    $Moved = ([pscustomobject]@{List=@()})
    Defrag -Input ([ref]$DiskImageInt) -Moved ([ref]$Moved)
    $DiskImageInt -join ','

    $Index = 0
    $Position = 0
    ($DiskImageInt |% {
        # pour les paies (car 1 sur deux est un data)
        for($i=0; $i -lt $_; $i ++) {
            $Position | Write-Output
            $Index ++
        }
        $Moved | Where-Object { $_.to -eq $Position } {
            for($i=0; $i -lt $_; $i ++) {
                $_.from | Write-Output
                $Index ++
            }
        }
        #Pour l'élément suivant (car 1 sur deux est un free space)
        for($i=0; $i -lt $_; $i ++) {
            $Position | Write-Output
            $Index ++
        }


        $Position ++
    }) -join ''

    $Index = 0
    $Position = 0
    $DiskImageInt |% {
        for($i=0; $i -lt $_; $i ++) {
            $Position * $Index | Write-Output
            $Index ++
        }
        $Moved | Where-Object { $_.to -eq $Position } {
            for($i=0; $i -lt $_; $i ++) {
                $_.from * $Index | Write-Output
                $Index ++
            }
        }
        $Position ++
    }
<#    $p = 0
    for($x = 0; $x -lt $ClonedImage.Count; $x++) {
        $Defrag = $DiskImageInt[$x]
        $Original = $ClonedImage[$x]

        if($x %2 -eq 0) {
            # Data
            for($n = 0; $n -lt $Defrag; $n++) {
                $Defrag * ($p++)    # Checksum      pas just
            }
        } else {
            # Free space
            $Moved | Where-Object {
                $_.to -eq $p
            } | ForEach-Object {
                $oldIndex = $_.from
                # Count blocks before the "from", this was the "file index"
                $oldFileIndex = $ClonedImage|Where-Object{$_ -lt $oldIndex}|Measure-Object -Sum|Select-Object -ExpandProperty Sum
                $oldLength = $ClonedImage[$oldIndex]
                for($n = 0; $n -lt $oldLength; $n++) {
                    $Defrag * ($p++)    # Checksum      pas juste
                }
            }
            $p ++
        }
    }
    #>
}
Begin {
    Function Defrag {
        [CmdletBinding()]
        [OutputType([object[]])]
        param([ref]$InputData,[ref]$Moved)

        $MovedNodes = @()
        $LastVal = ([Math]::Truncate(($InputData.Value.Count)/2)) * 2
        # Browse all value blocks form the end
        for($z=$LastVal; $z -ge 0; $z -= 2) {
            $DataLength = $InputData.Value[$z]
            # Browse all free blocks form the start
            for($a=1; $a -lt $InputData.Value.Count; $a += 2) {
                $FreeLength = [Math]::Abs($InputData.Value[$a])
                if($FreeLength -ge $DataLength) {
                    # Found free space, move block
                    $MovedNodes += [pscustomobject]@{from=$z;to=$a}
                    $InputData.Value[$z] = $FreeLength - $DataLength
                    $InputData.Value[$a] = -($FreeLength - $DataLength)
                    break
                }
            }
        }
        $Moved.Value = $MovedNodes
        #return $InputData
    }

    Function CtoI {
        param([parameter(Position=0,Mandatory)][ValidateRange('0','9')][char]$c)
        return ([int]$c) - ([int]([char]'0'))
    }
    Function ItoC {
        param([parameter(Position=0,Mandatory)][ValidateRange(0,9)][int]$i)
        return (""+$i)[0]
    }
    Function ExpandImage {
        [CmdletBinding()]
        [OutputType([string])]
        param([char[]]$InputData)
        $s = [string]::empty
        for($i=0;$i -lt $InputData.Count; $i++) {
            for($j=0;$j -lt (CtoI($InputData[$i])); $j ++) {
                if($i % 2 -eq 0) {
                    $s += "$([Math]::Truncate($i/2))"
                } else {
                    $s += '.'
                }
            }
        }
        return $s
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