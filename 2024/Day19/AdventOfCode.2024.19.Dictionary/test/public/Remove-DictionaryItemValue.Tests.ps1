Describe "Testing public module function Remove-DictionaryItemValue" -Tag "UnitTest" {
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

        It "Should not throw on empty dictionnary" {
            $script:AOC_2024_19_DICTIONARY = (@{})
            {Invoke-ModuleFunctionStub -Key xyz -ValueItem @('xyz')} | Should -Not -Throw
        }

        It "Should not throw" {
            {Invoke-ModuleFunctionStub -Key xyz -ValueItem @('xyz')} | Should -Not -Throw
        }

        It "Should not output" {
            $Result = Invoke-ModuleFunctionStub -Key abc  -ValueItem @('ab','c')
            $Result | Should -BeNullOrEmpty
        }

        It "Should remove the value from the dictionary key" {
            Invoke-ModuleFunctionStub -Key abc -ValueItem @('ab','c')
            $script:AOC_2024_19_DICTIONARY['abc'].Parts.Count | Should -Be 3
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[0].Count | Should -Be 1
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1].Count | Should -Be 2
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[2].Count | Should -Be 3
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[0][0] | Should -Be 'abc'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1][0] | Should -Be 'a'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[1][1] | Should -Be 'bc'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[2][0] | Should -Be 'a'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[2][1] | Should -Be 'b'
            $script:AOC_2024_19_DICTIONARY['abc'].Parts[2][2] | Should -Be 'c'
        }
    }
}
