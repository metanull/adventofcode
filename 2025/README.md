# Advent of Code 2025

Using Node.js this year :-)

## Structure

```
2025/
├── package.json         # Node.js project configuration
├── lib/                 # Common utilities
│   ├── index.js         # Main export point
│   ├── fileReader.js    # File reading utilities
│   ├── logger.js        # Logging and output formatting
│   └── utils.js         # Common helper functions
├── Input/               # Input files for each day
│   └── Day1.txt
├── Day1/                # Day 1 solutions
│   ├── Day1.1.js        # Part 1
│   └── Day1.2.js        # Part 2
└── DayN/                # Additional days...
```

## Setup/Run

1. Install dependencies:
```powershell
cd 2025
npm install
```

2. Running 

```powershell
# invoking using node
node 2025\Day1\Day1.1.js    # Run Day 1, Part 1

# or invoking using npm (using scripts defined in the package.json):
npm run day1.2    # Run Day 1, Part 2
```

## Common Utilities

### Logger

The logger provides centralized logging with color-coded output:

```javascript
import { logger } from '../lib/index.js';

logger.setDebug(true);           // Enable debug mode
logger.debug('Debug message');    // Gray - only shown in debug mode
logger.info('Information');       // Blue
logger.success('Success!');       // Green
logger.warn('Warning');           // Yellow
logger.error('Error');            // Red
logger.result('Answer', 42);      // Cyan - for final answers
logger.section('Section Title');  // Magenta - section headers
logger.progress(50, 100);         // Progress indicator
```

### File Reader

Read and process input files easily:

```javascript
import { readLines, readFile, processLines, aggregateLines } from '../lib/index.js';

// Read all lines as array
const lines = readLines('path/to/file.txt');

// Skip empty lines
const lines = readLines('path/to/file.txt', { skipEmpty: true });

// Process each line with callback
const results = processLines('path/to/file.txt', (line, index) => {
    return parseInt(line) * 2;
});

// Aggregate lines with reducer
const sum = aggregateLines('path/to/file.txt', (acc, line) => {
    return acc + parseInt(line);
}, 0);
```

### Utility Functions

Common helper functions:

```javascript
import { sum, product, int, extractIntegers, range, unique } from '../lib/index.js';

sum([1, 2, 3, 4]);              // 10
product([2, 3, 4]);             // 24
int("42");                       // 42 (safe parsing)
extractIntegers("a1b2c3");      // [1, 2, 3]
range(0, 5);                     // [0, 1, 2, 3, 4]
unique([1, 2, 2, 3, 3]);        // [1, 2, 3]
```

## Template node.js app

```javascript
import { readLines, logger, sum, int } from '../lib/index.js';
import { resolve } from 'path';

logger.setDebug(true);

// 1. Decode/parse input line
function decodeLine(line) {
    // Parse the line into a usable format
    return line;
}

// 2. Process each line
function processLine(data, lineIndex) {
    // Compute value for this line
    return 0;
}

// 3. Aggregate results
function aggregate(results) {
    // Combine all results (usually sum)
    return sum(results);
}

// 4. Main solution
function solve() {
    const lines = readLines('Input/DayN.txt', { skipEmpty: true });
    const results = lines.map((line, i) => processLine(decodeLine(line), i));
    return aggregate(results);
}

// Run
const answer = solve();
logger.result('Final Answer', answer);
```
