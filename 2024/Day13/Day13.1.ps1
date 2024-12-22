# https://adventofcode.com/2024/day/13
Process {
    $InputData | ForEach-Object {
        $_
    } | ft
}
Begin {
    $Year = 2024
    $Day = 13
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "..\Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning

    $InputData = Get-Content $InputPath -ErrorAction Stop | Foreach-Object -Begin {
        $Parser = [regex]::new('^(?<Label>.*?): X[=\+](?<X>\d+), Y[=\+](?<Y>\d+)$')
        $Buffer = @()
        $Object = $null
    } -End {
        [pscustomobject]@{
            A = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
            B = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
            Prize = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
        } | Write-Output
    } -Process {
        #Write-Warning $Buffer.Count
        if(-not $_) {
            $Buffer = @()
            return
        }
        if($Buffer.Count -le 3) {
            $Buffer += ,$_
        }
        if($buffer.Count -ne 3) {
            return
        }
        [pscustomobject]@{
            A = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
            B = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
            Prize = [pscustomobject]@{
                #Label = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'Label'}|Select-Object -ExpandProperty Value
                X = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'X'}|Select-Object -ExpandProperty Value
                Y = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'Y'}|Select-Object -ExpandProperty Value
            }
        }  | Write-Output

    }
}