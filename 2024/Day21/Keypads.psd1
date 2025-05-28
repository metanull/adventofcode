@{
    Numeric = @{
        Printable = @"
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
  ! | 0 | A |
    +---+---+
"@
        Condensed = @"
789
456
123
!0A
"@
        Moves = @{
            '!' = @{Horizontal = 2; Vertical = 0}
            'A' = @{Horizontal = 0; Vertical = 0}
            '0' = @{Horizontal = 1; Vertical = 0}
            '1' = @{Horizontal = 2; Vertical = 1}
            '2' = @{Horizontal = 1; Vertical = 1}
            '3' = @{Horizontal = 0; Vertical = 1}
            '4' = @{Horizontal = 2; Vertical = 2}
            '5' = @{Horizontal = 1; Vertical = 2}
            '6' = @{Horizontal = 0; Vertical = 2}
            '7' = @{Horizontal = 2; Vertical = 3}
            '8' = @{Horizontal = 1; Vertical = 3}
            '9' = @{Horizontal = 0; Vertical = 3}
        }
    }
    Directional = @{
        Printable = @"
    +---+---+
  ! | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+
"@
        Condensed = @"
!^A
<v>
"@
        Moves = @{
            '!' = @{Horizontal = 2; Vertical = 0}
            'A' = @{Horizontal = 0; Vertical = 0}
            'Up' = @{Horizontal = 1; Vertical = 0}
            'Left' = @{Horizontal = 2; Vertical = -1}
            'Down' = @{Horizontal = 1; Vertical = -1}
            'Right' = @{Horizontal = 0; Vertical = -1}
        }
    }
}
