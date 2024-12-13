Process {
    $DiskImage = $InputData

    $DiskImage | Write-Warning

    ExpandImage -InputData $DiskImage | Write-Warning
    '00992111777.44.333....5555.6666.....8888..'|write-warning

    $DiskImageInt = ([char[]]$DiskImage)|%{CtoI($_) }
    $ClonedImage = $DiskImageInt.PSObject.Copy()
    # $DiskImageInt -join ',' | Write-Warning
    $Moved = ([pscustomobject]@{List=@()})

    $DebugFile = New-Item -Force (Join-Path $PSScriptRoot 'Output\Day9.2.Debug.txt') -ItemType File
    "ORIGINAL    " | Add-Content $DebugFile -NoNewline
    for($_x = 0; $_x -lt $ClonedImage.Count; $_x ++) {
        "$($ClonedImage[$_x]) ".PadLeft(8,' ') | Add-Content $DebugFile -NoNewline
    }
    '---' | Add-Content $DebugFile
    Defrag -Input ([ref]$DiskImageInt) -Moved ([ref]$Moved)
    # $DiskImageInt -join ';' | Write-Warning
    "DEFRAGMENTED" | Add-Content $DebugFile -NoNewline
    for($_x = 0; $_x -lt $DiskImageInt.Count; $_x ++) {
        "$($DiskImageInt[$_x]) ".PadLeft(8,' ') | Add-Content $DebugFile -NoNewline
    }
    '---' | Add-Content $DebugFile

    $Index = 0
    $Position = 0
    $DiskImageInt | Foreach-Object {
        $OldLength = $ClonedImage[$Position]
        $NewLength = $_

        if($Position % 2 -eq 0) {
            $fgtype = [System.ConsoleColor]::DarkGreen
        } else {
            $fgtype = [System.ConsoleColor]::DarkYellow
        }
        if( $ClonedImage[$Position] -eq 0) {
            $fgold = [System.ConsoleColor]::Yellow
        } elseif( $ClonedImage[$Position] -gt 0) {
            $fgold = [System.ConsoleColor]::Green
        } else {
            $fgold = [System.ConsoleColor]::Red
        }
        if( $DiskImageInt[$Position] -eq 0) {
            $fgnew = [System.ConsoleColor]::Yellow
        } elseif( $DiskImageInt[$Position] -gt 0) {
            $fgnew = [System.ConsoleColor]::Green
        } else {
            $fgnew = [System.ConsoleColor]::Red
        }
        if($ClonedImage[$Position] -eq $DiskImageInt[$Position]) {
            $bgChanged = [System.ConsoleColor]::Black
        } else {
            $bgChanged = [System.ConsoleColor]::Red
        }
        
        Write-Host -NoNewline "Position: "
        Write-Host -NoNewline -BackgroundColor $fgtype "$Position"
        Write-Host -NoNewline "`tValue: "
        Write-Host -NoNewline -ForegroundColor Green "$(($Position - ($Position % 2)) / 2)"
        Write-Host -NoNewline "`tType: "
        Write-Host -NoNewline -BackgroundColor $fgtype "$(if($Position % 2 -eq 0) { 'Data' } else { 'Free' })"
        Write-Host -NoNewline "`tBefore: "
        Write-Host -NoNewline -ForegroundColor $fgold "$($ClonedImage[$Position])"
        Write-Host -NoNewline "`t"
        Write-Host -NoNewline -BackgroundColor $bgChanged "After"
        Write-Host -NoNewline ":  "
        Write-Host -NoNewline -ForegroundColor $fgnew "$($DiskImageInt[$Position])"
        Write-Host -NoNewline "`tIndex: "
        Write-Host -NoNewline -ForegroundColor DarkCyan "$Index"
        Write-Host -NoNewline "`tBefore::Value:"
        Write-Host -NoNewline -ForegroundColor $fgold "$(if($Position % 2 -eq 0){("$($Position/2)" * $ClonedImage[$Position]).PadRight(8,' ')} else {('.' * $ClonedImage[$Position]).PadRight(8,' ')})"
        
        Write-Host -NoNewline "`tMoved: "
        Write-Host -NoNewline -ForegroundColor ($Host.UI.RawUI.ForegroundColor) "$(($Moved|Where-Object{$_.to -eq $Position}|%{"$($_.from) -> $($_.to)"}) -join ', ')"
        Write-Host ''
        '' | Add-Content $DebugFile
        
        if($Position % 2 -eq 0) {
            # Length is the length of a block of data
            if($NewLength -eq 0) {
                Write-Warning "Data removed, adding $($OldLength) spaces"
                for($i = 0; $i -lt $OldLength; $i ++) {
                    '.' | Add-Content $DebugFile -NoNewline
                    $null | Write-Output
                }
            } else {
                Write-Warning "Data present, copying '$($Position/2)' $($OldLength) time(s)"
                for($i = 0; $i -lt $OldLength; $i ++) {
                    "$($Position/2)" | Add-Content $DebugFile -NoNewline
                    $Position / 2 | Write-Output    
                }
            }
            $Index += $OldLength
        } else {
            # Length is the length of a block of free space
            # 1 original data left
            if($NewLength -gt 0) {
                Write-Warning "Space filled, adding $($OldLength) space(s)"
                for($i = 0; $i -lt $NewLength; $i ++) {
                    '.' | Add-Content $DebugFile -NoNewline
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
                Write-Warning "Space filled, copying '$($ValueBeforeDefrag)' $($LengthBeforeDefrag) time(s), from $($_.from)"
                for($i=0; $i -lt $LengthBeforeDefrag; $i ++) {
                    "$ValueBeforeDefrag" | Add-Content $DebugFile
                    $ValueBeforeDefrag | Write-Output
                }
                $Index += $LengthBeforeDefrag
            }
            # 3 free space left
            if($NewLength -lt 0 -and ($AbsNewLength = [Math]::Abs($NewLength)) -gt 0) {
                Write-Warning "Space left, adding $($AbsNewLength) space(s)"
                for($i = 0; $i -lt $AbsNewLength; $i ++) {
                    '.' | Add-Content $DebugFile
                    $null | Write-Output
                }
                $Index += $NewLength
            }
        }

        $Position ++
    } | Out-NUll #| Tee-Object -Variable Defragmented | Out-Null

    $DebugFile.FullName | Write-Warning
    # ($Defragmented |% {"'$_'"} ) -join ','
}
Begin {
    Function Defrag {
        [CmdletBinding()]
        [OutputType([object[]])]
        param([ref]$InputData,[ref]$Moved)

        $DebugFile = Get-Item (Join-Path $PSScriptRoot 'Output\Day9.2.Debug.txt')

        $MovedNodes = @()
        $LastVal = ([Math]::Truncate(($InputData.Value.Count)/2)) * 2

        # Browse all value blocks form the end
        for($z=$LastVal; $z -ge 0; $z -= 2) {
            $DataLength = $InputData.Value[$z]

<# ------------------------------------------------------------------- 
Write-Host -NoNewline "Z: "
Write-Host -NoNewline -ForegroundColor Magenta "$z"
Write-Host -NoNewline "`tValue: "
Write-Host -NoNewline -ForegroundColor Green "($($z / 2))"
Write-Host -NoNewline "`tLength: "
if($DataLength -le 0) {
    Write-Host -NoNewline -ForegroundColor Red "$DataLength"
} else {
    Write-Host -NoNewline -ForegroundColor Green "$DataLength"
}
Write-Host ''
   ------------------------------------------------------------------- #>
<# ------------------------------------------------------------------- #>
<# ------------------------------------------------------------------- #>

            # Browse all free blocks form the start
            for($a=1; $a -lt $InputData.Value.Count; $a += 2) {
                $FreeLength = [Math]::Abs($InputData.Value[$a])

<# ------------------------------------------------------------------- 
Write-Host -NoNewline "A: "
Write-Host -NoNewline -ForegroundColor Yellow "$a"
Write-Host -NoNewline "`tValue: "
Write-Host -NoNewline -ForegroundColor Green "($(($a - 1) / 2))"
Write-Host -NoNewline "`tLength: "
if($FreeLength -le 0) {
    Write-Host -NoNewline -ForegroundColor Red "$FreeLength" 
} else {
    Write-Host -NoNewline -ForegroundColor Green "$FreeLength"
}
Write-Host ''
   ------------------------------------------------------------------- #>

                if($DataLength -gt 0 -and $FreeLength -ge $DataLength) {
                    # Found free space, move block
                    $MovedNodes += [pscustomobject]@{from=$z;to=$a}
                    $InputData.Value[$z] = 0
                    $InputData.Value[$a] = -($FreeLength - $DataLength)
<# ------------------------------------------------------------------- #>
Write-Host -NoNewline -ForegroundColor Cyan "Moving from: "
Write-Host -NoNewline -ForegroundColor Yellow "$z"
Write-Host -NoNewline -ForegroundColor Magenta " ($($DataLength))"
Write-Host -NoNewline -ForegroundColor Cyan "`tto: "
Write-Host -NoNewline -ForegroundColor Yellow "$a"
Write-Host -NoNewline -ForegroundColor MAgenta " ($($FreeLength))"
Write-Host -NoNewline "`tLength: "
Write-Host -NoNewline -ForegroundColor Cyan "$($FreeLength - $DataLength)"  # PAS JUSTE cas du 4
Write-Host ''
for($_x = 0; $_x -lt $InputData.Value.Count; $_x ++) {
    if($_x -eq $z -or $_x -eq $a) {
        Write-Host -NoNewline -ForegroundColor Magenta "$($InputData.Value[$_x]) ".PadLeft(4,' ')
    } else {
        if($_x % 2 -eq 0) {
            Write-Host -NoNewline -ForegroundColor DarkGreen  "$($InputData.Value[$_x]) ".PadLeft(4,' ')
        } else {
            Write-Host -NoNewline -ForegroundColor DarkYellow "$($InputData.Value[$_x]) ".PadLeft(4,' ')
        }
    }
}
Write-Host ''
<# ------------------------------------------------------------------- #>

                    break
                }
            }
            
            
            $InputData.Value |% { " $_ ".PadLeft(8,' ') } | Add-Content $DebugFile.FullName -NoNewline
            '' | Add-Content $DebugFile.FullName
        }
        $Moved.Value = $MovedNodes

        $InputData.Value |% { " $_ ".PadLeft(8,' ') } | Add-Content $DebugFile.FullName -NoNewline
        '' | Add-Content $DebugFile.FullName
        $Moved.Value |% { "$($_.from) -> $($_.to)`t" } | Add-Content $DebugFile.FullName -NoNewline
        '' | Add-Content $DebugFile.FullName

        Write-Warning "DEFRAG: $( ($InputData.Value) -join ',' )"
        Write-Warning "MOVED:  $( ($Moved.Value |% {"$($_.from) -> $($_.to)"}) -join ', ' )"
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
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\Day9.txt"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
}