Process {
    $InputData | ForEach-Object {
        $Line = $_
    }
}
Begin {
    $InputPath = Join-Path -Resolve -Path $PSScriptRoot -ChildPath "Input\$(($PSCommandPath | Split-Path -Leaf) -replace '\.\d+\.ps1','.txt')" -ErrorAction Stop
    $InputData = Get-Content $InputPath -ErrorAction Stop
}