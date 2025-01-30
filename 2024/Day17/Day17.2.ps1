$A = 64751475
$R = 3,1,4,3,1,7,1,6,3

Function p([Uint64]$v) {
    return [Convert]::ToString($v,2).PadLeft(40,'0')
}
Function o($A=0) {
    $B = $A -band 7
    $B = $B -bxor 2
    $C = $A -shr $B
    $B = $B -bxor $C
    $B = $B -bxor 3
    $A = $A -shr 3
    $R = $B -band 7 
    $R | Write-Output
    if($A -ne 0) {
        o($A)
    } else {
        return
    }
}
Function ov($A=0) {
    Write-Host -ForegroundColor Yellow "".PadLeft(48,'-')
    Write-Host -ForegroundColor Yellow "     A: $(p($A))`t$A"
    Write-Host -ForegroundColor Yellow "".PadLeft(48,'-')

    $B = 0; $C = 0; $R = 0;
    Write-Host -ForegroundColor Red    "     A: $(p($A))`t$A"
    Write-Host -ForegroundColor White  "AND  7: $(p(7))"
    Write-Host -ForegroundColor Green  " > B =  $(p($A -band 7))`t$($A -band 7)`n"
    $B = $A -band 7

    Write-Host -ForegroundColor Green  "     B: $(p($B))`t$B"
    Write-Host -ForegroundColor White  "XOR  2: $(p(2))"
    Write-Host -ForegroundColor Green  " > B =  $(p($B -bxor 2))`t$($B -bxor 2)`n"
    $B = $B -bxor 2

    Write-Host -ForegroundColor Red    "     A: $(p($A))`t$A"
    Write-Host -ForegroundColor Green  "SHR  B: $(p($B))`t$B"
    Write-Host -ForegroundColor Blue   " > C =  $(p($A -shr $B))`t$($A -shr $B)`n"
    $C = $A -shr $B

    Write-Host -ForegroundColor Green  "     B: $(p($B))`t$B"
    Write-Host -ForegroundColor Blue   "XOR  C: $(p($C))`t$C"
    Write-Host -ForegroundColor Green  " > B =  $(p($B -bxor $C))`t$($B -bxor $C)`n"
    $B = $B -bxor $C

    Write-Host -ForegroundColor Green  "     B: $(p($B))`t$B"
    Write-Host -ForegroundColor White  "XOR  3: $(p(3))"
    Write-Host -ForegroundColor Green  " > B =  $(p($B -bxor 3))`t$($B -bxor 3)`n"
    $B = $B -bxor 3

    Write-Host -ForegroundColor Red    "     A: $(p($A))`t$A"
    Write-Host -ForegroundColor White  "SHR  3: $(p(3))"
    Write-Host -ForegroundColor Red    " > A =  $(p($A -shr 3))`t$($A -shr 3)`n"
    $A = $A -shr 3

    Write-Host -ForegroundColor Yellow "".PadLeft(48,'-')
    Write-Host -ForegroundColor Green  "     B: $(p($B))`t$B"
    Write-Host -ForegroundColor White  "AND  7: $(p(7))"
    Write-Host -ForegroundColor Yellow   "PRINT>  $(p($R -band 7))`t$($R -band 7)`n"
    $R = $B -band 7 
    $R | Write-Output
    if($A -ne 0) {
        ov($A)
    } else {
        return
    }
}

Clear-Host
o($A) -join ','

ov([Uint64]64751475)

<#

$r = 2,1,7,4,1,5,0,3
$i = 0
$r |% {
    0..7|% {
        $c = $_
        0..7|% {
            $b = $_
            if(((($b -bxor $c) -bxor 3) -band 7) -eq ($r|Select-Object -Last 1 -Skip $i)) {
                "C: $c, B: $b, R: $($r|Select-Object -Last 1 -Skip $i)"
                0..7|%{
                    $a = $_
                    if((($c -bxor 2) -bxor $b) -eq $a) {
                        "Inverse A: $a, B: $b, C: $c"
                    }
                }
            }
        }
    }
}
#>