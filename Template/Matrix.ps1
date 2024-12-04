function Test-MatrixSubstring {
    <#
        .SYNOPSIS
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point in a given direction

        .DESCRIPTION
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point in a given direction

        .PARAMETER X
        The X coordinate of the starting point. It is assumed that this point is the first character of the word

        .PARAMETER Y
        The Y coordinate of the starting point. It is assumed that this point is the first character of the word

        .PARAMETER XInc
        The increment in the X direction (eg: -1, 0 or 1)

        .PARAMETER YInc
        The increment in the Y direction (eg: -1, 0 or 1)

        .PARAMETER MatrixSize
        The size of the matrix. It is assumed that the matrix is square

        .PARAMETER Length
        The length of the word to be obtained.
    #>
    [CmdletBinding()]
    [OutputType([bool])]
    param(
        [ValidateScript({$_ -ge 0})]
        [long]$X,
        [ValidateScript({$_ -ge 0})]
        [long]$Y,
        [ValidateRange(-1,1)]
        [long]$XInc,
        [ValidateRange(-1,1)]
        [long]$YInc,
        [ValidateScript({$_ -ge 0})]
        [long]$MatrixSize,
        [ValidateScript({$_ -ge 0})]
        [long]$Length
    )
    if(      ((($X) + ($Length * $XInc)) -le ($MatrixSize)) `
        -and ((($Y) + ($Length * $YInc)) -le ($MatrixSize)) `
        -and ((($X+1) + ($Length * $XInc)) -ge 0) `
        -and ((($Y+1) + ($Length * $YInc)) -ge 0) ) {
        return $true
    }
    return $false
}
function Get-MatrixSubstring {
    <#
        .SYNOPSIS
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point in a given direction

        .DESCRIPTION
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point in a given direction

        .PARAMETER Matrix
        The matrix from which the word is to be obtained. It is assumed that the matrix is square

        .PARAMETER X
        The X coordinate of the starting point. It is assumed that this point is the first character of the word

        .PARAMETER Y
        The Y coordinate of the starting point. It is assumed that this point is the first character of the word

        .PARAMETER XInc
        The increment in the X direction (eg: -1, 0 or 1)

        .PARAMETER YInc
        The increment in the Y direction (eg: -1, 0 or 1)

        .PARAMETER Length
        The length of the word to be obtained.

        .EXAMPLE
        $Matrix = @(
    '.......'
    '.A.v.Z.'
    '..B.Y..'
    '.>.C.<.'
    '..W.D..'
    '.V.^.E.'
    '.......'

        # Get the word from the top-left corner of the matrix, in the direction from the top-left corner to the bottom-right corner
        Get-MatrixSubstring -Matrix $Matrix -X 1 -Y 1 -Length 5 -XInc 1 -YInc 1
        # Output: 'ABCDE'

        # Get the word from the top-right corner of the matrix, in the direction from the top-right corner to the bottom-left corner
        Get-MatrixSubstring -Matrix $Matrix -X 5 -Y 1 -Length 5 -XInc -1 -YInc 1
        # Output: 'ZYCWV'

        # Get the word from the bottom-left corner of the matrix, in the direction from the bottom-left corner to the top-right corner
        Get-MatrixSubstring -Matrix $Matrix -X 1 -Y 5 -Length 5 -XInc 1 -YInc -1
        # Output: 'VWCYZ'

        # Get the word from the bottom-right corner of the matrix, in the direction from the bottom-right corner to the top-left corner
        Get-MatrixSubstring -Matrix $Matrix -X 5 -Y 5 -Length 5 -XInc -1 -YInc -1
        # Output: 'EDCBA'

        # Get the word from the middle of the matrix, in the direction from the middle-left to the middle-right
        Get-MatrixSubstring -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 1 -YInc 0
        # Output: '>.C.<'

        # Get the word from the middle of the matrix, in the direction from the middle-right to the middle-left
        Get-MatrixSubstring -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc -1 -YInc 0
        # Output: '<.C.>'

        # Get the word from the middle of the matrix, in the direction from the top-middle to the bottom-middle
        Get-MatrixSubstring -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 0 -YInc 1
        # Output: 'v.C.^'
        
    #>
    [CmdletBinding()]
    [OutputType([string])]
    param(
        [String[]]$Matrix,
        [ValidateScript({$_ -ge 0})]
        [long]$X,
        [ValidateScript({$_ -ge 0})]
        [long]$Y,
        [ValidateRange(-1,1)]
        [long]$XInc,
        [ValidateRange(-1,1)]
        [long]$YInc,
        [ValidateScript({$_ -ge 0})]
        [long]$Length
    )
    $MatrixSize = $Matrix.Count
    if($MatrixSize -eq 0) {
        throw 'Empty matrix!'
    }
    if($MatrixSize -ne $Matrix[0].Length) {
        throw 'Input matrix not a square!'
    }
    if(-not (Test-MatrixSubstring -X $X -Y $Y -XInc $XInc -YInc $YInc -MatrixSize $MatrixSize -Length $Length)) {
        return $null
    }
    $Str = ''
    for($i=0;$i -lt $Length;$i++) {
        $Str += $Matrix[$Y+($i*$YInc)][$X+($i*$XInc)]
    }
    $Str | Write-Output
}

function Test-MatrixSubstringFromMiddle {
    <#
        .SYNOPSIS
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point - which is to be at the center of that word - in a given direction

        .DESCRIPTION
        Test if it is possible to obtain a word of a given length from a matrix, starting from a given point - which is to be at the center of that word - in a given direction

        .PARAMETER X
        The X coordinate of the starting point. It is assumed that this point is the center of the word

        .PARAMETER Y
        The Y coordinate of the starting point. It is assumed that this point is the center of the word

        .PARAMETER XInc
        The increment in the X direction (eg: -1, 0 or 1)

        .PARAMETER YInc
        The increment in the Y direction (eg: -1, 0 or 1)

        .PARAMETER MatrixSize
        The size of the matrix. It is assumed that the matrix is square

        .PARAMETER Length
        The length of the word to be obtained. It is assumed that the length is an odd number
    #>
    [CmdletBinding()]
    [OutputType([bool])]
    param(
        [ValidateScript({$_ -ge 0})]
        [long]$X,
        [ValidateScript({$_ -ge 0})]
        [long]$Y,
        [ValidateRange(-1,1)]
        [long]$XInc,
        [ValidateRange(-1,1)]
        [long]$YInc,
        [ValidateScript({$_ -ge 0})]
        [long]$MatrixSize,
        [ValidateScript({$_ -ge 0 -and ($_%2)})]
        [long]$Length
    )
    $OffsetLength = ($Length -1)/2
    if(     ($X - ($OffsetLength * [Math]::Abs($XInc))) -ge 0 `
       -and ($Y - ($OffsetLength * [Math]::Abs($YInc))) -ge 0 `
       -and ($X + ($OffsetLength * [Math]::Abs($XInc))) -lt $MatrixSize `
       -and ($Y + ($OffsetLength * [Math]::Abs($YInc))) -lt $MatrixSize ) {
        return $true
    }
    return $false
}
function Get-MatrixSubstringFromMiddle {
    <#
        .SYNOPSIS
        Get a word from a matrix, starting from a given point - which is to be at the center of that word - in a given direction

        .DESCRIPTION
        Get a word from a matrix, starting from a given point - which is to be at the center of that word - in a given direction

        .PARAMETER Matrix
        The matrix from which the word is to be obtained. It is assumed that the matrix is square

        .PARAMETER X
        The X coordinate of the starting point. It is assumed that this point is the center of the word

        .PARAMETER Y
        The Y coordinate of the starting point. It is assumed that this point is the center of the word

        .PARAMETER XInc
        The increment in the X direction (eg: -1, 0 or 1)

        .PARAMETER YInc
        The increment in the Y direction (eg: -1, 0 or 1)

        .PARAMETER Length
        The length of the word to be obtained. It is assumed that the length is an odd number

        .EXAMPLE
        $Matrix = @(
    '.......'
    '.A.v.Z.'
    '..B.Y..'
    '.>.C.<.'
    '..W.D..'
    '.V.^.E.'
    '.......'
)
        # Get the word from the center of the matrix, in the direction from the top-left corner to the bottom-right corner
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 1 -YInc 1 
        # Output: 'ABCDE'

        # Get the word from the center of the matrix, in the direction from the top-right corner to the bottom-left corner
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc -1 -YInc 1 
        # Output: 'ZYCWV'

        # Get the word from the center of the matrix, in the direction from the middle-left to the middle-right
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 1 -YInc 0
        # Output: '>.C.<'

        # Get the word from the center of the matrix, in the direction from the middle-right to the middle-left
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc -1 -YInc 0 
        # Output: '<.C.>'

        # Get the word from the center of the matrix, in the direction from the bottom-left corner to the top-right corner
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 1 -YInc -1 
        # Output: 'VWCYZ'

        # Get the word from the center of the matrix, in the direction from the bottom-right corner to the top-left corner
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc -1 -YInc -1 
        # Output: 'EDCBA'

        # Get the word from the center of the matrix, in the direction from the top-middle to the bottom-middle
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 0 -YInc 1 
        # Output: 'v.C.^'

        # Get the word from the center of the matrix, in the direction from the bottom-middle to the top-middle
        Get-MatrixSubstringFromMiddle -Matrix $Matrix -X 3 -Y 3 -Length 5 -XInc 0 -YInc -1 
        # Output: '^.C.v'
    #>
    [CmdletBinding()]
    [OutputType([string])]
    param(
        [String[]]$Matrix,
        [ValidateScript({$_ -ge 0})]
        [long]$X,
        [ValidateScript({$_ -ge 0})]
        [long]$Y,
        [ValidateRange(-1,1)]
        [long]$XInc,
        [ValidateRange(-1,1)]
        [long]$YInc,
        [ValidateScript({$_ -ge 0 -and ($_%2)})]
        [long]$Length
    )
    $MatrixSize = $Matrix.Count
    if($MatrixSize -eq 0) {
        throw 'Empty matrix!'
    }
    if($MatrixSize -ne $Matrix[0].Length) {
        throw 'Input matrix not a square!'
    }
    if(-not (Test-MatrixSubstringFromMiddle -X $X -Y $Y -XInc $XInc -YInc $YInc -MatrixSize $MatrixSize -Length $Length)) {
        return $null
    }
    $Offset = (($Length -1)/2)
    $NewX = $X - ($Offset * $XInc)
    $NewY = $Y - ($Offset * $YInc)

    $Str = ''
    for($i=0;$i -lt $Length;$i++) {
        $Str += $Matrix[$NewY+($i*$YInc)][$NewX+($i*$XInc)]
    }
    $Str | Write-Output
}
