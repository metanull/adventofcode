Process {
    $DiskImage = $InputData

    #$DiskImage | Write-Warning
    #ExpandImage -InputData $DiskImage | Write-Warning
    #'00992111777.44.333....5555.6666.....8888..'|write-warning

    $DiskImageInt = ([char[]]$DiskImage)|%{CtoI($_) }
    $ClonedImage = $DiskImageInt.PSObject.Copy()
    # $DiskImageInt -join ',' | Write-Warning
    $Moved = ([pscustomobject]@{List=@()})

    Write-Warning "DEFRAG"

    ExpandDefragmentedImage -DiskImageInt $DiskImageInt -ClonedImage $ClonedImage -Moved $Moved -FilePath (Join-Path $PSScriptRoot 'Output\Day9.2.BEFORE.txt')
    Defrag -Input ([ref]$DiskImageInt) -Moved ([ref]$Moved)
    ExpandDefragmentedImage -DiskImageInt $DiskImageInt -ClonedImage $ClonedImage -Moved $Moved -FilePath (Join-Path $PSScriptRoot 'Output\Day9.2.AFTER.txt')
    # $DiskImageInt -join ';' | Write-Warning

    Write-Warning "COMPUTE"
    $Index = 0
    $Position = 0
    $DiskImageInt | Foreach-Object {
        $OldLength = $ClonedImage[$Position]
        $NewLength = $_
        if($Position % 2 -eq 0) {
            # Length is the length of a block of data
            if($NewLength -eq 0) {
                for($i = 0; $i -lt $OldLength; $i ++) {
                    $null | Write-Output
                }
            } else {
                for($i = 0; $i -lt $OldLength; $i ++) {
                    $Position / 2 | Write-Output    
                }
            }
            $Index += $OldLength
        } else {
            # Length is the length of a block of free space
            # 1 original data left
            if($NewLength -gt 0) {
                for($i = 0; $i -lt $NewLength; $i ++) {
                    $null | Write-Output
                }
                $Index += $NewLength
            }
            # 2 moved data
            $Moved | Where-Object {
                $_.to -eq $Position
            } | Foreach-Object {
                $LengthBeforeDefrag = $ClonedImage[$_.from]
                $ValueBeforeDefrag = $_.from / 2
                for($i=0; $i -lt $LengthBeforeDefrag; $i ++) {
                    $ValueBeforeDefrag | Write-Output
                }
                $Index += $LengthBeforeDefrag
            }
            # 3 free space left
            if($NewLength -lt 0 -and ($AbsNewLength = [Math]::Abs($NewLength)) -gt 0) {
                for($i = 0; $i -lt $AbsNewLength; $i ++) {
                    $null | Write-Output
                }
                $Index += $NewLength
            }
        }
        $Position ++
    } | Tee-Object -Variable Defragmented | Foreach-Object -Begin { 
            Write-Warning "CHECKSUM"
            $ChecksumIndex = 0
        } -Process {
            # Write-Warning "($_) * ($ChecksumIndex) = $($_ * $ChecksumIndex)"
            if($null -ne $_) {
                $_ * ($ChecksumIndex ++) | Write-Output
            }
        } -End {
            Write-Warning "SUM"
        } | Measure-Object -Sum | Select-Object -ExpandProperty Sum
}
Begin {

    Function ExpandDefragmentedImage {
        [CmdletBinding()]
        [OutputType([object[]])]
        param($DiskImageInt,$ClonedImage,$Moved,$FilePath = 'Output\Day9.2.Debug.txt')

        Begin {
            $DebugFile = New-Item -Force $FilePath -ItemType File
            '' | Set-Content $DebugFile

            $Width = 8
        }
        Process {
            $Index = 0
            $Position = 0
            $DiskImageInt | Foreach-Object {
                $OldLength = $ClonedImage[$Position]
                $NewLength = $_
                if($Position % 2 -eq 0) {
                    # Length is the length of a block of data
                    if($NewLength -eq 0) {
                        for($i = 0; $i -lt $OldLength; $i ++) {
                            '.'.PadLeft($Width,' ') | Write-Output
                        }
                    } else {
                        for($i = 0; $i -lt $OldLength; $i ++) {
                            "$($Position / 2)".PadLeft($Width,' ') | Write-Output
                        }
                    }
                    $Index += $OldLength
                } else {
                    # Length is the length of a block of free space
                    # 1 original data left
                    if($NewLength -gt 0) {
                        for($i = 0; $i -lt $NewLength; $i ++) {
                            '.'.PadLeft($Width,' ') | Write-Output
                        }
                        $Index += $NewLength
                    }
                    # 2 moved data
                    $Moved | Where-Object {
                        $_.to -eq $Position
                    } | Foreach-Object {
                        $LengthBeforeDefrag = $ClonedImage[$_.from]
                        $ValueBeforeDefrag = $_.from / 2
                        for($i=0; $i -lt $LengthBeforeDefrag; $i ++) {
                            "$($ValueBeforeDefrag)".PadLeft($Width,' ') | Write-Output
                        }
                        $Index += $LengthBeforeDefrag
                    }
                    # 3 free space left
                    if($NewLength -lt 0 -and ($AbsNewLength = [Math]::Abs($NewLength)) -gt 0) {
                        for($i = 0; $i -lt $AbsNewLength; $i ++) {
                            '.'.PadLeft($Width,' ') | Write-Output
                        }
                        $Index += $NewLength
                    }
                }
                $Position ++
            } | Add-Content $DebugFile -NoNewline
            '' | Add-Content $DebugFile
        }
    }
    Function Defrag {
        [CmdletBinding()]
        [OutputType([object[]])]
        param([ref]$InputData,[ref]$Moved)

        Begin {
            $DebugFile = New-Item -Force (Join-Path $PSScriptRoot 'Output\Day9.2.Debug.txt') -ItemType File
            '' | Set-Content $DebugFile
        }
        End {
            #return $InputData
        }
        Process {
            $MovedNodes = @()
            $LastVal = ([Math]::Truncate(($InputData.Value.Count)/2)) * 2

            "Indexes         " | Add-Content $DebugFile -NoNewline
            for($_x = 0; $_x -lt $InputData.Value.Count; $_x ++) {
                "$($_x) ".PadLeft(8,' ') | Add-Content $DebugFile -NoNewline
            }
            "" | Add-Content $DebugFile

            # Browse all value blocks form the end
            for($z=$LastVal; $z -ge 0; $z -= 2) {

                "?  ($z)`tA: (??)`t" | Add-Content $DebugFile -NoNewline
                for($_x = 0; $_x -lt $InputData.Value.Count; $_x ++) {
                    "$($InputData.Value[$_x]) ".PadLeft(8,' ') | Add-Content $DebugFile -NoNewline
                }
                "" | Add-Content $DebugFile
                "" | Add-Content $DebugFile

                $DataLength = $InputData.Value[$z]
                # Browse all free blocks form the start
                for($a=1; $a -lt $InputData.Value.Count; $a += 2) {
                    $FreeLength = [Math]::Abs($InputData.Value[$a])
                    if($DataLength -gt 0 -and $FreeLength -ge $DataLength) {

                        Write-Warning "Moving from $z to $a (in $($InputData.Value.Count))"
                        "  Moving from $("$z".PadLeft(6,' ')) ($($InputData.Value[$z]) -> 0)" | Add-Content $DebugFile
                        "           to $("$a".PadLeft(6,' ')) ($($InputData.Value[$a]) -> $(-($FreeLength - $DataLength)))" | Add-Content $DebugFile
                        
                        # Found free space, move block
                        $MovedNodes += [pscustomobject]@{from=$z;to=$a}
                        $InputData.Value[$z] = 0
                        $InputData.Value[$a] = -($FreeLength - $DataLength)

                        "-> ($z)`tA: ($a)`t" | Add-Content $DebugFile -NoNewline
                        for($_x = 0; $_x -lt $InputData.Value.Count; $_x ++) {
                            "$($InputData.Value[$_x]) ".PadLeft(8,' ') | Add-Content $DebugFile -NoNewline
                        }
                        "" | Add-Content $DebugFile

                        break
                    }
                }
                "" | Add-Content $DebugFile
            }
            $Moved.Value = $MovedNodes


        }
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