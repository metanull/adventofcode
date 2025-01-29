# https://adventofcode.com/2024/day/17
[CmdletBinding()]
[OutputType([object[]])]
param()
Process {
    Write-Warning "P: $Script:P A: $Script:A B: $Script:B C: $Script:C"
    while($Script:P -lt $Script:Program.Length) {
        Write-Verbose "P: $Script:P A: $Script:A B: $Script:B C: $Script:C"
        $OpCode = $Script:Program[$Script:P]
        $Operand = $Script:Program[$Script:P + 1]
        OpCode -OpCode $OpCode -Operand $Operand
    } 
}
Begin {
    $Year = 2024
    $Day = 17
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "..\Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    #$InputData = Get-Content $InputPath -ErrorAction Stop

    $Script:A = 0
    $Script:B = 0
    $Script:C = 0
    $Script:P = 0
    $Script:Program = @()

    Get-Content $InputPath -ErrorAction Stop | Foreach-Object -Begin {
        $Parser = [regex]::new('^(?<Label>.*): (?<Values>.*)$')
        $Buffer = @()
        $Object = $null
    } -Process {
        if($Buffer.Count -le 5) {
            $Buffer += ,$_
        }
        if($Buffer.Count -eq 5) {
            $Script:A = $Parser.Match($Buffer[0]).Groups|Where-Object{$_.Name -eq 'Values'}|Select-Object -ExpandProperty Value
            $Script:B = $Parser.Match($Buffer[1]).Groups|Where-Object{$_.Name -eq 'Values'}|Select-Object -ExpandProperty Value
            $Script:C = $Parser.Match($Buffer[2]).Groups|Where-Object{$_.Name -eq 'Values'}|Select-Object -ExpandProperty Value
            $Program = $Parser.Match($Buffer[4]).Groups|Where-Object{$_.Name -eq 'Values'}|Select-Object -ExpandProperty Value
            $Script:Program = ([regex]::new('\d+')).Matches($Program).Value
            $Script:P = 0
        }
    }

    Function Operand {
        [CmdletBinding()]
        [OutputType([int])]
        param(
            [Parameter(Mandatory)]
            [string]
            $Value,

            [switch]
            $print
        )
        $r = $null;
        switch($Value) {
            0 { $r = 0 }
            1 { $r = 1 }
            2 { $r = 2 }
            3 { $r = 3 }
            4 { $r = $A }
            5 { $r = $B }
            6 { $r = $C }
            default { throw "Invalid operand: $Value" }
        }
        Write-Debug "Operand: $Value -> $r"
        $r | Write-Output
    }
    Function OpCode {
        [CmdletBinding()]
        [OutputType([int])]
        param(
            [Parameter(Mandatory)]
            [int]
            $OpCode,

            [Parameter(Mandatory)]
            [int]
            $Operand
        )
        Begin {
            
        }
        Process {
            Write-Verbose "OpCode: $OpCode, Operand: $Operand"
            switch($OpCode) {
                0 { 
                    # ADV 
                    Write-Verbose "ADV $(Operand -Value $Operand)"
                    $Script:A = [math]::Truncate($Script:A / [math]::Pow(2, (Operand -Value $Operand)))
                    $Script:P += 2
                }
                1 { 
                    # BXL
                    Write-Verbose "BXL $($Operand)"
                    $Script:B = $Script:B -bxor $Operand
                    $Script:P += 2
                }
                2 { 
                    # BST
                    Write-Verbose "BST $(Operand -Value $Operand)"
                    $Script:B = (Operand -Value $Operand) % 8
                    $Script:P += 2
                }
                3 { 
                    # JNZ
                    Write-Verbose "JNZ $($Operand)"
                    if($Script:A -ne 0) {
                        $Script:P = $Operand
                    } else {
                        $Script:P += 2
                    }
                }
                4 { 
                    # BXC
                    Write-Verbose "BXC N/A"
                    $Script:B = $Script:B -bxor $Script:C
                    $Script:P += 2
                }
                5 { 
                    # OUT
                    Write-Verbose "OUT $(Operand -Value $Operand)"
                    (Operand -Value $Operand) % 8 | Write-Output
                    $Script:P += 2
                }
                6 { 
                    # BDV
                    Write-Verbose "BDV $(Operand -Value $Operand)"
                    $Script:B = [math]::Truncate($Script:A / [math]::Pow(2, (Operand -Value $Operand)))
                    $Script:P += 2
                }
                7 { 
                    # CDV
                    Write-Verbose "CDV $(Operand -Value $Operand)"
                    $Script:C = [math]::Truncate($Script:A / [math]::Pow(2, (Operand -Value $Operand)))
                    $Script:P += 2
                }
                default { throw "Invalid opcode: $Value" }
            }
        }
    }
}