Describe "Testing public module function New-DictionaryItemValue" -Tag "UnitTest" {
    BeforeEach {
        $script:AOC_2024_19_DICTIONARY = (@{
            'abc' = [pscustomobject]@{
                Pattern = 'abc'
                Parts = [System.Collections.ArrayList]::new()
            }
        })
    }
    AfterEach {
        $script:AOC_2024_19_DICTIONARY = (@{})
    }
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

        It "Should throw on invalid key" {
            { Invoke-ModuleFunctionStub -Key xyz -ValueItem @('abc')} | Should -Throw
        }

        It "Should not output data" {
            $Result = Invoke-ModuleFunctionStub -Key abc -ValueItem @('abc')
            $Result | Should -BeNullOrEmpty
        }

        It "Should add the value to the dictionary key" {
            Invoke-ModuleFunctionStub -Key abc -ValueItem @('abc')
            Invoke-ModuleFunctionStub -Key abc -ValueItem @('ab','c')
            Should -ActualValue $script:AOC_2024_19_DICTIONARY['abc'].Parts -BeOfType 'System.Collections.ArrayList'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts.Count | Should -Be 2
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[0].Count | Should -Be 1
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1].Count | Should -Be 2
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[0][0] | Should -Be 'abc'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1][0] | Should -Be 'ab'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1][1] | Should -Be 'c'
        }
    }
}
