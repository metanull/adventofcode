Describe "Testing public module function New-ProgressBarObject" -Tag "UnitTest" {
    Context "A dummy unit test" {
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
            {Invoke-ModuleFunctionStub} | Should -Not -Throw
        }
        It "Should not throw when a string is passed" {
            {Invoke-ModuleFunctionStub -Activity "Activity"} | Should -Not -Throw
        }
        It "Should return a PSCustomObject" {
            $Result = Invoke-ModuleFunctionStub -Activity "Activity"
            $Result | Should -BeOfType [PSCustomObject]
        }
        It "Should return a PSCustomObject with the correct property types" {
            $Result = Invoke-ModuleFunctionStub -Activity "Activity"
            Should -ActualValue $Result.Current -BeOfType [int]
            Should -ActualValue $Result.Total -BeOfType [int]
            Should -ActualValue $Result.Params -BeOfType [hashtable]
        }
    }
}
