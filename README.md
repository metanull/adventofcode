# Purpose

This repository holds my own solutions to the puzzles provided on [adventofcode.com](https://adventofcode.com/2024/about) by [Eric Wastl](https://was.tl/).

## Structure

- Solutions are organized by Year. Each year has its own folder.
- Inside each year's folder, scripts are named following this convention: `Day X[.Y].ext`; where:
  - `X` is the number of the day (e.g. A number between 1 and 24)
  - `Y` is the number of the part of the challenge (e.g. 1, 2 or more)
  - `ext` is the file extension corresponding to the chosen language (e.g. `php`,`ps1`,`sh`,`c`,`js`)

By Example:
`/2024/Day3.1.ps1` is my solution for to the first (`1`) part of puzzle of the third (`3`) day of this year's advent of code issue (`2024`). This solution is written in powershell (`ps1`).

## Puzzles and Inputs

For the actual puzzles, and the input data, please refer to the [adventofcode.com](https://adventofcode.com) website.
According to their rules, it is not permitted to store the puzzles and the inputs in external repositories.
For more information, please refer to [adventofcode.com](https://adventofcode.com/2024/about).

In order to run the solutions without modifications, make sure to save your input files locally under ./`Year`/`Input`/Day`Number`.txt. By example: `./Year/Input/Day1.txt`

## Running Node.js Solutions

For Node.js solutions, use the universal launcher from the root directory:

```bash
# Install dependencies (first time only)
npm install

# Run any solution
npm run aoc <year> <day>.<part>

# Examples:
npm run aoc 2025 Day1.1    # or simply: npm run aoc 2025 1.1
npm run aoc 2024 Day21.2   # or simply: npm run aoc 2024 21.2
```

All Node.js solutions share common libraries located in `/lib`.

## Creating New Day Solutions

Generate skeleton files for a new day:

```bash
# Create files for a specific day (defaults to current year)
npm run new <day>

# Create files for a specific year and day
npm run new <day> <year>

# Examples:
npm run new 5           # Creates 2025/Day5/Day5.1.js and Day5.2.js
npm run new 12 2024     # Creates 2024/Day12/Day12.1.js and Day12.2.js
```

This automatically creates:
- Year directory (if it doesn't exist)
- Day directory (`DayX/`)
- Solution templates (`DayX.1.js` and `DayX.2.js`)
- Empty input file (`Input/DayX.txt`)

The generator will refuse to overwrite existing non-empty directories.