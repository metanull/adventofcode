Describe "Testing public module function Get-Dictionary" -Tag "UnitTest" {
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

        It "Should not throw on empty dictionary" {
            $script:AOC_2024_19_DICTIONARY = (@{})
            {Invoke-ModuleFunctionStub} | Should -Not -Throw
        }

        It "Should return empty hashtable on empty dictionary" {
            $script:AOC_2024_19_DICTIONARY = (@{})
            $Item = Invoke-ModuleFunctionStub
            Should -ActualValue $Item -BeOfType 'hashtable'
            $Item.Keys | Should -BeNullOrEmpty
            $Item.Values | Should -BeNullOrEmpty
        }

        It "Should return the dictionary" {
            $Item = Invoke-ModuleFunctionStub

            Should -ActualValue $Item -BeOfType 'hashtable'
            $Item.Keys.Count | Should -Be 2
            $Item.Keys | Should -Contain 'abc'
            $Item.Keys | Should -Contain 'def'
            $Item['abc'].Pattern | Should -Be 'abc'
            $Item['def'].Pattern | Should -Be 'def'
            Should -ActualValue $Item['abc'].Parts -BeOfType 'System.Collections.ArrayList'
            $Item['abc'].Parts.Count | Should -Be 4
            $Item['abc'].Parts[0].Count | Should -Be 1
            $Item['abc'].Parts[1].Count | Should -Be 2
            $Item['abc'].Parts[2].Count | Should -Be 2
            $Item['abc'].Parts[3].Count | Should -Be 3
            Should -ActualValue $Item['def'].Parts -BeOfType 'System.Collections.ArrayList'
            $Item['def'].Parts.Count | Should -Be 4
            $Item['def'].Parts[0].Count | Should -Be 1
            $Item['def'].Parts[1].Count | Should -Be 2
            $Item['def'].Parts[2].Count | Should -Be 2
            $Item['def'].Parts[3].Count | Should -Be 3
        }
    }
}
