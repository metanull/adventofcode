# https://adventofcode.com/2024/day/3#part2

Process {
    dodont -Mem $Data | Where-Object {
        $_.Do
    } | Foreach-Object {
        mul -Mem ($Data.Substring($_.Index,$_.Length))
    } | Measure-Object -Sum
}

Begin {
    Function mul {
        param([string]$Mem)
        $Reg = [regex]::new('mul\(\d+,\d+\)')
        $Reg.Matches($Mem) | Foreach-Object {
            $x = [int]($matches[1])
            $y = [int]($matches[2])
            "$($_.value)`t$x * $y = $($x * $y)" | Write-Verbose
            ($x * $y) | Write-Output
        }
    }
    Function dodont {
        param([string]$Mem)
        $Reg = @{
            Do = [regex]::new("(^|do\(\)|don't\(\))")
            Mul = [regex]::new('mul\(\d+,\d+\)')
        }
        $Prev = $null
        $Cur = $null
        $Reg.Do.Matches($Mem) | Foreach-Object { 
            $Cur = [pscustomobject]@{
                Do = ($_.Groups[1].value -in '','do()')
                Index = $_.Index
                End = $null
                Length = $null
            }
            if($Prev -ne $null) {
                $Prev.End = $_.Index
                $Prev.Length = $Prev.End - $Prev.Index
                $Prev | Write-Output
            }
            $Prev = $Cur.PSObject.Copy()
        }
        $Prev.End = $Mem.Length
        $Prev.Length = $Prev.End - $Prev.Index
        $Prev | Write-Output
    } 

    $Data = Get-Content -Raw (Join-Path -Resolve $PSScriptRoot Input\Day3.txt)
}