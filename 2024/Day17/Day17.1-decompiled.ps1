<#
//        "DECOMPILING" the input PROGRAM:
//          
//              Register A: 64751475
//              Register B: 0
//              Register C: 0
//              Program: 2,4,1,2,7,5,4,5,1,3,5,5,0,3,3,0
//
//        /* 0 */ A = 64751475
//
//        /* 1 */ 2, 4,   /* BST: B = A & 7 */
//        /* 2 */ 1, 2,   /* BXL: B = B ^ 2 */
//        /* 3 */ 7, 5,   /* CDV: C = A >>B */
//        /* 4 */ 4, 5,   /* BXC: B = B ^ C */
//        /* 5 */ 1, 3,   /* BXL: B = B ^ 3 */
//        /* 6 */ 5, 5,   /* OUT: B & 7     */
//        /* 7 */ 0, 3,   /* ADV: A = A >>3 */
//        /* 8 */ 3, 0,   /* JNZ: A == 0 ? GOTO 1 ELSE EXIT */
//
//        /* Z */ OUT = 3,1,4,3,1,7,1,6,3.
#>

function obi {
[CmdletBinding()]
    param(
        [UInt64] $A,
        [UInt64] $B,
        [UInt64] $R,
        [String] $T
    )
    "OP| $($T.PadRight(25,' ')) $($R.ToString().PadLeft(12,' ')) $(([Convert]::ToString($R,2)).PadLeft(40,'0'))" | Write-Warning
    " L: $(([Convert]::ToString($A,2)).PadLeft(40,'0'))" | Write-Debug
    " R: $(([Convert]::ToString($B,2)).PadLeft(40,'0'))" | Write-Debug
    "=>: $(([Convert]::ToString($R,2)).PadLeft(40,'0'))" | Write-Debug

}
function pgm {
    [CmdletBinding(DefaultParametersetName='First')]
    param(
        [Parameter(Mandatory,ParameterSetName='First')]
        [UInt64] $A,
        [Parameter(Mandatory,ParameterSetName='Other')]
        [object] $R
    )
    Process {
        if($PSCmdlet.ParameterSetName -eq 'First') {
            $R = [PSCustomObject]@{A = [UInt64]$A;B = [UInt64]0;C = [UInt64]0}
        }
        # "A: $(([Convert]::ToString($R.A,2)).PadLeft(40,'0'))"
        "+---------------------------------------------------------------------------------+" | Write-Warning
        "INIT:| A |                    $($R.A.ToString().PadLeft(12,' ')) $(([Convert]::ToString($R.A,2)).PadLeft(40,'0'))" | Write-Warning
        "     | B |                    $($R.B.ToString().PadLeft(12,' ')) $(([Convert]::ToString($R.B,2)).PadLeft(40,'0'))" | Write-Warning
        "     | C |                    $($R.C.ToString().PadLeft(12,' ')) $(([Convert]::ToString($R.C,2)).PadLeft(40,'0'))" | Write-Warning
        "+---------------------------------------------------------------------------------+" | Write-Warning

        obi -A 0 -B 0 -R $R.A -T "A = A ".PadRight(22,' ')

        obi -A $R.A -B 7 -R ($R.B -band $R.A) -T "2,4| BST A| B = A   & 7".PadRight(22,' ')
        $R.B = $R.A -band 7

        obi -A $R.B -B 2 -R ($R.B -bxor 2) -T "1,2| BXL 2| B = B XOR 2".PadRight(22,' ')
        $R.B = $R.B -bxor 2

        obi -A $R.A -B $R.B -R ($R.A -shr $R.B) -T "7,5| CDV B| C = A  >> B".PadRight(22,' ')
        $R.C = $R.A -shr $R.B

        obi -A $R.B -B $R.C -R ($R.B -bxor $R.C) -T "4,5| BXC  | B = B XOR C".PadRight(22,' ')
        $R.B = $R.B -bxor $R.C

        obi -A $R.B -B 3 -R ($R.B -bxor 3) -T "1,3| BXL 3| B = B XOR 3".PadRight(22,' ')
        $R.B = $R.B -bxor 3

        obi -A $R.B -B 7 -R ($R.B -band 7) -T "5,5| OUT B| OUT B   & 7".PadRight(22,' ')
        $V = $R.B -band 7

        obi -A $R.A -B 3 -R ($R.A -shr 3) -T "0,3| ADV 3| A = A  >> 3".PadRight(22,' ')
        $R.A = $R.A -shr 3

        $V | Write-Output

        if($R.A) {
            obi -A $R.A -B 0 -R ($R.A -eq 0) -T "3,0| JNZ 0| PTR = 0".PadRight(22,' ')
            pgm -R $R
        } else {
            obi -A $R.A -B 0 -R ($R.A -eq 0) -T "3,0| JNZ 0| EXIT".PadRight(22,' ')
        }
    }
}

clear-host
$final = pgm -A 64751475
$final -join ','
