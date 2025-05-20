Describe "Testing public module function Format-DictionaryItem" -Tag "UnitTest" {
    BeforeAll {
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
    AfterAll {
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

        It "Should not throw" {
            {Invoke-ModuleFunctionStub -Item $script:AOC_2024_19_DICTIONARY.abc} | Should -Not -Throw
        }

        It "Should return a string" {
            $Result = Invoke-ModuleFunctionStub -Item $script:AOC_2024_19_DICTIONARY.abc
            $Result | Should -BeOfType [string]
        }

        It "Should return a multiline string representing the item" {
            $Result = Invoke-ModuleFunctionStub -Item $script:AOC_2024_19_DICTIONARY.abc
            $Lines = $Result -split "`n"
            $Lines.Count | Should -Be 6
            $Lines[0] -match 'Key.*abc.*4' | Should -BeTrue
            $Lines[1] -match '0.*abc.*1' | Should -BeTrue
            $Lines[2] -match '1.*ab.+c.*2' | Should -BeTrue
            $Lines[3] -match '2.*a.+bc.*2' | Should -BeTrue
            $Lines[4] -match '3.*a.+b.+c.*3' | Should -BeTrue
            $Lines[5] | Should -BeNullOrEmpty
        }
    }
}
