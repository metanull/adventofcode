<#
M*A + N*B = Z
=>
M*A.X + N*B.X = Z.X
M*A.Y + N*B.Y = Z.Y

Button A: X+19, Y+11
Button B: X+26, Y+50
Prize: X=11223, Y=12479

M*19 + N*26 = 11223
M*11 + N*50 = 12479
#>
<#
($M * $A.X) + ($N * $B.X) = $Prize.X
($M * $A.Y) + ($N * $B.Y) = $Prize.Y
=>
$M * $A.X = $Prize.X - ($N * $B.X)  =>  $M = ($Prize.X - ($N * $B.X)) / $A.X
$M * $A.Y = $Prize.Y - ($N * $B.Y)  =>  $M = ($Prize.Y - ($N * $B.Y)) / $A.Y
=>
($Prize.X - ($N * $B.X)) / $A.X = ($Prize.Y - ($N * $B.Y)) / $A.Y
=>
$Prize.X / $A.X - ($N * $B.X) / $A.X = $Prize.Y / $A.Y - ($N * $B.Y) / $A.Y
=>
$Prize.X / $A.X - $Prize.Y / $A.Y = ($N * $B.X) / $A.X - ($N * $B.Y) / $A.Y
=>
$Prize.X / $A.X - $Prize.Y / $A.Y = $N * ($B.X / $A.X - $B.Y / $A.Y)
=>
$N = ($Prize.X / $A.X - $Prize.Y / $A.Y) / ($B.X / $A.X - $B.Y / $A.Y)
=>
$M = ($Prize.X - ($N * $B.X)) / $A.X

$N * $B.X = $Prize.X - ($M * $A.X)v => $N = ($Prize.X - ($M * $A.X)) / $B.X
$N * $B.Y = $Prize.Y - ($M * $A.Y) => $N = ($Prize.Y - ($M * $A.Y)) / $B.Y
=>
($Prize.X - ($M * $A.X)) / $B.X = ($Prize.Y - ($M * $A.Y)) / $B.Y
=>
$Prize.X / $B.X - ($M * $A.X) / $B.X = $Prize.Y / $B.Y - ($M * $A.Y) / $B.Y
=>
$Prize.X / $B.X - $Prize.Y / $B.Y = ($M * $A.X) / $B.X - ($M * $A.Y) / $B.Y
=>
$Prize.X / $B.X - $Prize.Y / $B.Y = $M * ($A.X / $B.X - $A.Y / $B.Y)
=>
$M = ($Prize.X / $B.X - $Prize.Y / $B.Y) / ($A.X / $B.X - $A.Y / $B.Y)
=>
$N = ($Prize.Y - ($M * $A.Y)) / $B.Y
#>


$A = New-Object PSObject -Property @{X=[long]82;Y=[long]39}
$B = New-Object PSObject -Property @{X=[long]37;Y=[long]87}
$P = New-Object PSObject -Property @{X=[long]4611;Y=[long]6843}

$Offset = @(0,10000000000000)
$Offset | Foreach-Object {
    $Prize = New-Object PSObject -Property @{X=$P.X+$_;Y=$P.Y+$_}

    $N1 = ($Prize.X / $A.X - $Prize.Y / $A.Y) / ($B.X / $A.X - $B.Y / $A.Y)
    $M1 = ($Prize.X - ($N1 * $B.X)) / $A.X

    $M2 = ($Prize.X / $B.X - $Prize.Y / $B.Y) / ($A.X / $B.X - $A.Y / $B.Y);
    $N2 = ($Prize.Y - ($M2 * $A.Y)) / $B.Y

    "$_ / N: $N1, M: $M1" | Write-Output
    "$_ / N: $N2, M: $M2" | Write-Output

}