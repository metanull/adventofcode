# Day 2 - Invalid Product IDs

## Part 1

Find all product IDs with a "doubled pattern" (sequence repeated twice) within given ranges and sum them.

### Examples
- `55` (5 twice) - invalid
- `6464` (64 twice) - invalid  
- `123123` (123 twice) - invalid
- `0101` - valid (leading zeros not allowed)

Sample input: `11-22,95-115,998-1012,...`

Expected answer: `1227775554`

### Solution
- **Answer:** TBD
- **File:** `Day2.1.js`
- **Logic:** Parse comma-separated ranges, find doubled patterns in each range, sum all invalid IDs

## Part 2

[Description of part 2 TBD]

### Solution
- **Answer:** TBD
- **File:** `Day2.2.js`

## Notes

- Ranges are separated by commas
- Each range format: `start-end`
- Check each ID in range for doubled pattern
- Doubled pattern = even-length number where first half equals second half
