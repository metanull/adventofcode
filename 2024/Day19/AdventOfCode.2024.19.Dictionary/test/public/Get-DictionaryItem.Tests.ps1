Describe "Testing public module function Get-DictionaryItem" -Tag "UnitTest" {
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
            $Item = $null
            {Invoke-ModuleFunctionStub -RefItem ([ref]$Item) -Key xyz } | Should -Not -Throw
        }

        It "Should return null on not found" {
            $Item = $null
            $Result = Invoke-ModuleFunctionStub -RefItem ([ref]$Item) -Key xyz
            $Item | Should -BeNullOrEmpty
            $Result | Should -BeNullOrEmpty
        }

        It "Should return the Key on found" {
            $Item = $null
            $Result = Invoke-ModuleFunctionStub -RefItem ([ref]$Item) -Key abc
            
            # Test the returned arraylist object
            $Item | Should -Not -BeNullOrEmpty
            Should -ActualValue $Item -BeOfType [System.Collections.ArrayList]
            $Item.Count | Should -Be 4
            $Item[0].Count | Should -Be 1
            $Item[1].Count | Should -Be 2
            $Item[2].Count | Should -Be 2
            $Item[3].Count | Should -Be 3

            # Test the returned flatten array
            $Result.Count | Should -Be 8
        }
    }
}
