<#
    .SYNOPSIS
    A dummy function, that doesn't not do anything
#>
[CmdletBinding()]
[OutputType([PSCustomObject])]
param(
    [Parameter(Mandatory, Position = 0, ValueFromPipeline)]
    [PSCustomObject]$Item
)
Begin {
    $ANSI = "$([char]27)"
    $Colors = @{
        Red = "$ANSI[31;1m"
        Green = "$ANSI[32;1m"
        Yellow = "$ANSI[33;1m"
        Blue = "$ANSI[34;1m"
        Magenta = "$ANSI[35;1m"
        Cyan = "$ANSI[36;1m"
        White = "$ANSI[37;1m"
        None = "$ANSI[0m"
    }
    Function ColoredString {
        param([string]$Color,[string]$Message,[switch]$Quoted)
        $QuoteString = ''
        if($Quoted.IsPresent -and $Quoted) {
            $QuoteString = "$($Colors.Yellow)""$($Colors.None)"
        }
        "$($QuoteString)$($Colors[$Color])$($Message)$($Colors.None)$($QuoteString)"
    }
}
Process {
    if(-not ($Item|Get-Member -Name Pattern)) {
        "Item is not of the expected type" | Write-Error
    }
    if(-not ($Item|Get-Member -Name Parts)) {
        "Item is not of the expected type" | Write-Error
    }

    $stringBuilder = New-Object -TypeName System.Text.StringBuilder

    $stringBuilder.Append("Key: $(ColoredString -Color Cyan -Message $Item.Pattern -Quoted)") | Out-Null
    $stringBuilder.AppendLine(" | Items: $(ColoredString -Color Yellow -Message $Item.Parts.Count)") | Out-Null

    for($k = 0; $k -lt $Item.Parts.Count; $k ++) {
        $stringBuilder.Append("- [$(ColoredString -Color Blue -Message $k.ToString())]: ") | Out-Null
        for($l = 0; $l -lt $Item.Parts[$k].Count; $l ++) {
            if($l -ne 0) {
                $stringBuilder.Append(', ') | Out-Null
            }
            $stringBuilder.Append((ColoredString -Color Green -Message $Item.Parts[$k][$l] -Quoted)) | Out-Null
        }
        $stringBuilder.AppendLine(". Count: $(ColoredString -Color Yellow -Message $Item.Parts[$k].Count).") | Out-Null
    }
    return $stringBuilder.ToString()
}