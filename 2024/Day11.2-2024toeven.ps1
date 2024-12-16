<#
* 2024
ODD:  0
EVEN: 1         -> 4
ODD:  5         -> 49
EVEN: 50        -> 494
ODD:  495       -> 4940
EVEN: 4941      -> 49407
ODD:  49408     -> 494071
EVEN: 494072    -> 4940711
ODD:  4940712   -> 49407114
EVEN: 49407115  -> 494071149
ODD:  494071150 -> 4940711490
...
#>
for($i=4940711490; $i -lt 49407114900; $i ++) {
    $r = $i * 2024
    if("$r".length % 2 -eq 0 ) {
        Write-Host -ForegroundColor Green "$i => $r"
        break
    } else {
        #Write-Host -ForegroundColor Red "$i => $r"
        #break
    }
}