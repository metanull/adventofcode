Describe "Testing public module function Reset-DictionaryItem" -Tag "UnitTest" {
    BeforeEach {
        $abc_Items = [System.Collections.ArrayList]::new()
        $abc_Items.Add([System.Collections.ArrayList]::new(@('abc')))
        $abc_Items.Add([System.Collections.ArrayList]::new(@('ab','c')))
        $abc_Items.Add([System.Collections.ArrayList]::new(@('a','bc')))
        $abc_Items.Add([System.Collections.ArrayList]::new(@('a','b','c')))
        $def_Items = [System.Collections.ArrayList]::new()
        $def_Items.Add([System.Collections.ArrayList]::new(@('def')))
        $def_Items.Add([System.Collections.ArrayList]::new(@('de','f')))
        $def_Items.Add([System.Collections.ArrayList]::new(@('d','ef')))
        $def_Items.Add([System.Collections.ArrayList]::new(@('d','e','f')))
        $script:AOC_2024_19_DICTIONARY = (@{
            'abc' = [pscustomobject]@{
                Pattern = 'abc'
                Parts = $abc_Items
            }
            'def' = [pscustomobject]@{
                Pattern = 'def'
                Parts = $def_Items
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

        It "Should not throw on empty dictionnary" {
            $script:AOC_2024_19_DICTIONARY = (@{})
            {Invoke-ModuleFunctionStub -Key xyz} | Should -Not -Throw
        }

        It "Should not throw" {
            {Invoke-ModuleFunctionStub -Key xyz} | Should -Not -Throw
        }

        It "Should not output" {
            $Result = Invoke-ModuleFunctionStub -Key abc
            $Result | Should -BeNullOrEmpty
        }

        It "Should empty the dictionary" {
            Invoke-ModuleFunctionStub -Key abc
            Should -ActualValue $script:AOC_2024_19_DICTIONARY -BeOfType 'hashtable'
            $script:AOC_2024_19_DICTIONARY.Keys -contains 'abc' | Should -BeTrue
            $script:AOC_2024_19_DICTIONARY['abc'].Pattern | Should -Be 'abc'
            Should -ActualValue $script:AOC_2024_19_DICTIONARY['abc'].Parts -BeOfType 'System.Collections.ArrayList'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts.Count | Should -Be 0

            $script:AOC_2024_19_DICTIONARY.Keys -contains 'def' | Should -BeTrue
            $script:AOC_2024_19_DICTIONARY['def'].Pattern | Should -Be 'def'
            Should -ActualValue $script:AOC_2024_19_DICTIONARY['def'].Parts -BeOfType 'System.Collections.ArrayList'
            $script:AOC_2024_19_DICTIONARY['def'].Parts.Count | Should -Be 4
        }
    }
}
