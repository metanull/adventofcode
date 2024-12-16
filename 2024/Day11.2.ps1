# https://adventofcode.com/2024/day/11
[CmdletBinding()]
[OutputType([long])]
param(
        [Parameter(Mandatory = $false)]
        $blink = 6
)
Process {
    #Write-Warning "Blink 0 : $($Stones.Count)"
    #for($i = 0; $i -lt $blink; $i++) {
    #    $Stones = Blink -Stones $Stones
    #    Write-Warning "Blink $($i +1) : $($Stones.Count)"
    #}
    #$Stones.Count

    $script:Cache = @{}

    Write-Warning "Init : $($Stones.Count)"
    $Stones.Count
    ($Stones |% { "$_" }) -join ', ' | Write-Warning

    $Groups = BlinkGroup -Stones $Stones
    Write-Warning "Blink 0 : $($Stones.Groups)"
    $Groups | measure-object Count -sum|select-object -ExpandProperty Sum
    #($Groups |% { for($k=0;$k -lt $_.Count; $k++) {$_.Name} }) -join ', ' | Write-Warning

    for($i = 1; $i -lt $blink; $i++) {
        # Write-Warning "Blink $i : $($Groups.Count)"
        $BlinkTime = Measure-Command {$Groups = BlinkGroup -Groups $Groups}
        # ($Groups |% { for($k=0;$k -lt $_.Count; $k++) {$_.Name} }) -join ', ' | Write-Warning
        $CountTime = Measure-Command {$CountStones = $Groups | measure-object Count -sum|select-object -ExpandProperty Sum}
        Write-Warning ("Blink {2,2} : {3,9}`t=>`t{4,2} = {5,12} `tin {0,12:n6} min`t(count: {1,12:n6} ms)" -f ($BlinkTime.TotalMinutes),($CountTime.TotalMinutes),$i,($Groups.Count),($i+1),$CountStones)
        $CountStones | Write-Output
    }
    
}
Begin {
    $Year = 2024
    $Day = 11
    $PuzzleUrl = "https://adventofcode.com/$Year/$Day"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop
    $Stones = [regex]::new('(?<digit>\d+)') | ForEach-Object {
        $_.Matches($InputData) | ForEach-Object {
            [long]::parse($_.Groups['digit'].Value)
        }
    }

    Function BlinkGroup {
        [CmdletBinding(DefaultParameterSetName='Stones')]
        [OutputType([object[]])]
        param(
            [Parameter(ParameterSetName = 'Stones', Mandatory,Position = 0)]
            [long[]] $Stones,

            [Parameter(ParameterSetName = 'Groups', Mandatory,Position = 0)]
            [object[]] $Groups
        )
        if($null -ne $Stones) {
            $Stones |% { $_ | Blink } | Group-Object
        }
        elseif($null -ne $Groups) {
            $Groups |% { 
                $Blinked = $_.Group | Blink
                for($z=0;$z -lt $_.Count;$z++) {
                    $Blinked | Write-Output
                }
            } | Group-Object
        }
    }
    Function Blink {
        [CmdletBinding()]
        [OutputType([long])]
        param(
            [parameter(Mandatory,Position = 0,ValueFromPipeline)]
            [long] $Stone
        )
        
        #if($null -ne $script:Cache[$Stone]) {
        #    $ret = $script:Cache[$Stone]
        #} else {
            $ret = $null
            if($Stone -eq 0) {
                $ret = 1
            } elseif(($Str = "$Stone").length % 2 -eq 0) {
                $Div = [math]::Pow(10, $Str.length / 2)
                $ret = @([math]::truncate($Stone / $Div), [math]::truncate($Stone % $Div))
            } else {
                $ret = $Stone * 2024
            }
        #   $script:Cache[$Stone] = $ret
        #}
        $ret | Write-Output
    }
}