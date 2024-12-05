# https://adventofcode.com/2024/day/5
Process {
    $Data | Foreach-Object {
        # Verify line compliance
        try {
            # Reset the list of forbidden items
            $Forbidden = @()
            for($i=0; $i -lt $_.Values.Length; $i ++) {
                $Current = $_.Values[$i]
                # Verify if the current item is not forbidden by the current set of rules
                if($Current -in $Forbidden) {
                    throw "Item #$($i+1) ($($Current)) is not compliant with the rules in [$($_.Values -join ',')]"
                }
                # Add rules imposed by the current item to the set of rules
                $Forbidden += ($Rules | Where-Object{$_.Key -eq $Current} | Select-Object -ExpandProperty Values)
            }
            # Compliant, send down to the pipeline
            $_ | Write-Output
        } catch {
            # Non compliant, discard
            $_ | Write-Verbose
        }
    } | Foreach-Object {
        # Preserve only the Middle Value
        if($_.Values.Count % 2 -eq 0) {
            throw "Invalid number of elements in input, can't get the middle value from: [$($_.Values -join ',')]"
        }
        $_.Values[($_.Values.Count -1)/2] | Write-Output
    } | Measure-Object -Sum
}
Begin {
    $PuzzleUrl = "https://adventofcode.com/$(Get-Date -Format 'yyyy')/day/$([int](Get-Date -Format 'dd'))"
    $InputPath = Join-Path -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')"
    if(-Not (Test-Path $InputPath)) {
        $InputUrl = "$PuzzleUrl/input"
        Start-Process $InputUrl
        throw "Input file is missing. Please download the Input file at $InputUrl, and save it as $InputPath"
    }
    "Instructions: $PuzzleUrl" | Write-Warning
    $InputData = Get-Content $InputPath -ErrorAction Stop

    # Reading the list of Rules and Data from the input file
    $RuleHashtable = @{}
    $RuleExpression = [RegEx]::new('^(\d+)\|(\d+)$')
    $ReadingRules = $true
    #"Reading Rules" | Write-Warning
    $Data = $InputData | ForEach-Object {
        if($ReadingRules -and $_.Length -eq 0) {
            # Finished processing Rules, now starts the data
            #"Reading Data" | Write-Warning
            $ReadingRules = $false
        } elseif($ReadingRules) {
            # Process one line of rule, format of each line is xx|yy
            if(-not ($_ -match $RuleExpression)) {
                throw "Input line '$_' does not match $($Expr.ToString()))"
            }
            $RuleExpression.Matches($_) | Foreach-Object {
                $Key = [string]($_.Groups[2].Value)
                $Value = [int]($_.Groups[1].Value)
                if(-not ($RuleHashtable.ContainsKey($Key))) {
                    # No rule for that key yet, add it
                    $RuleHashtable.($Key) = @($Value)
                } else {
                    # A rule exists, appends the new value
                    $RuleHashtable.($Key) += ,($Value)
                }
            }
        } else {
            # Process one line of data, format of each line is xx,y,zzz,...
            [pscustomobject]@{Values=[int[]]($_ -split ',' | Foreach-Object { [int]$_ })} | Write-Output
        }
    }
    #"Convert Rules to objects" | Write-Warning
    $Rules = $RuleHashtable.GetEnumerator() | Foreach-Object {
        [pscustomobject]@{Key = $_.key; Values = $_.value}
    }
}