$A = 64751475
while($A) { 
    $i1 = (($A -band 7) -bxor 2)
    $i2 = $A -shr $i1
    $i1 = (($i1 -bxor $i2) -bxor 3) -band 7
    $i1 | Write-Output
    
    $A = $A -shr 3
}