Describe "Testing public module function ConvertTo-Pattern" -Tag "UnitTest" {
    Context "Default context" {
        BeforeAll {
            $ModuleRoot = $PSCommandPath | Split-Path -Parent | Split-Path -Parent | Split-Path -Parent
            $ScriptName = $PSCommandPath | Split-Path -Leaf
            $Visibility = $PSCommandPath | Split-Path -Parent | Split-Path -Leaf
            $SourceDirectory = Resolve-Path (Join-Path $ModuleRoot "source\$Visibility")
            $TestDirectory = Resolve-Path (Join-Path $ModuleRoot "test\$Visibility")

            $FunctionPath = Join-Path $SourceDirectory ($ScriptName -replace '\.Tests\.ps1$', '.ps1')
    
            # Create a Stub for the module function to test
            Function Invoke-ModuleFunctionStub {
                . $FunctionPath @args | write-Output
            }
        }

        It "Should not throw when no parameters are passed" {
            {$Result = Invoke-ModuleFunctionStub} | Should -Not -Throw
        }
        It "Should not throw when an empty string or null is passed" {
            {$Result = Invoke-ModuleFunctionStub -PatternList ""} | Should -Not -Throw
            {$Result = Invoke-ModuleFunctionStub -PatternList $null } | Should -Not -Throw
        }
        It "Should not throw when a string is passed" {
            {$Result = Invoke-ModuleFunctionStub -PatternList "test"} | Should -Not -Throw
        }
        It "Should not throw when a string array is passed" {
            {$Result = Invoke-ModuleFunctionStub -PatternList @("test1", "test2")} | Should -Not -Throw
        }

        It "Should return a PSCustomObject" {
            $Result = Invoke-ModuleFunctionStub -PatternList "test"
            $Result | Should -BeOfType [PSCustomObject]
        }
        It "Should return a PSCustomObject with the correct property types" {
            $Result = Invoke-ModuleFunctionStub -PatternList @("test1", "test22", "test33")
            Should -ActualValue $Result.Count -BeOfType [int]
            Should -ActualValue $Result.Patterns -BeOfType [System.Collections.Generic.HashSet[string]]
            Should -ActualValue $Result.SortedPatterns -BeOfType [System.Object[]]
            Should -ActualValue $Result.MinLength -BeOfType [int]
            Should -ActualValue $Result.MaxLength -BeOfType [int]
            Should -ActualValue $Result.ValuesByLength(6) -BeOfType [System.Object[]]
            Should -ActualValue $Result.CountByLength(6) -BeOfType [int]
        }
        
        It "Should return a PSCustomObject with the correct property values when multiple patterns are passed" {
            $Result = Invoke-ModuleFunctionStub -PatternList @("test1", "test22", "test33")
            $Result.Count | Should -Be 3
            $Result.Patterns | Should -Be @("test1", "test22", "test33")
            $Result.SortedPatterns | Should -Be @("test1", "test22", "test33")
            $Result.MinLength | Should -Be 5
            $Result.MaxLength | Should -Be 6
            $Result.ValuesByLength(6) | Should -Be @("test22", "test33")
            $Result.CountByLength(1) | Should -Be 0
            $Result.CountByLength(4) | Should -Be 0
            $Result.CountByLength(5) | Should -Be 1
            $Result.CountByLength(6) | Should -Be 2
        }
        It "Should return a PSCustomObject with the correct property values when a string is passed" {
            $Result = Invoke-ModuleFunctionStub -PatternList "test"
            $Result.Count | Should -Be 1
            $Result.Patterns | Should -Be @("test")
            $Result.SortedPatterns | Should -Be @("test")
            $Result.MinLength | Should -Be 4
            $Result.MaxLength | Should -Be 4
            $Result.ValuesByLength(4) | Should -Be @("test")
            $Result.CountByLength(4) | Should -Be 1
        }
        It "Should return a PSCustomObject with the correct property values when an empty string or null is passed" {
            $Result = Invoke-ModuleFunctionStub -PatternList ""
            $Result.Count | Should -Be 0
            $Result.Patterns | Should -Be @()
            $Result.SortedPatterns | Should -Be @()
            $Result.MinLength | Should -Be 0
            $Result.MaxLength | Should -Be 0
            $Result.ValuesByLength(0) | Should -Be @()
            $Result.CountByLength(0) | Should -Be 0
        }
        It "Should return a PSCustomObject with the correct property values when no parameters are passed" {
            $Result = Invoke-ModuleFunctionStub
            $Result.Count | Should -Be 0
            $Result.Patterns | Should -Be @()
            $Result.SortedPatterns | Should -Be @()
            $Result.MinLength | Should -Be 0
            $Result.MaxLength | Should -Be 0
            $Result.ValuesByLength(0) | Should -Be @()
            $Result.CountByLength(0) | Should -Be 0
        }
        It "Should return a PSCustomObject with the correct unique property values when multiple patterns are passed" {
            $Result = Invoke-ModuleFunctionStub -PatternList @("test1", "test2", "test1")
            $Result.Count | Should -Be 2
            $Result.Patterns | Should -Be @("test1", "test2")
            $Result.SortedPatterns | Should -Be @("test1", "test2")
            $Result.MinLength | Should -Be 5
            $Result.MaxLength | Should -Be 5
            $Result.ValuesByLength(5) | Should -Be @("test1", "test2")
            $Result.CountByLength(5) | Should -Be 2
        }
    }
}
