Describe "Testing public module function Test-DictionaryItemValue" -Tag "UnitTest" {
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

        It "Should not throw on not found" {
            {Invoke-ModuleFunctionStub -Key 'xyz' -ValueItem @('x','y','z') } | Should -Not -Throw
        }

        It "Should return false on not found" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('x','y','z')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeFalse
        }
        It "Should return false on not found (order sensitive)" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('c','b','a')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeFalse
        }
        It "Should return false on not found (length sensitive)" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('a','b','c','d')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeFalse
        }

        It "Should return true on found" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('a','b','c')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeTrue
        }
        It "Should return true on found (2)" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('ab','c')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeTrue
        }
        It "Should return true on found (3)" {
            $Result = Invoke-ModuleFunctionStub -Key 'abc' -ValueItem @('abc')
            $Result | Should -Not -BeNullOrEmpty
            $Result | Should -BeTrue
        }
    }
}
