#!/usr/bin/env node
/**
 * Universal launcher for Advent of Code solutions
 * Usage: node run.js <year> <day>.<part>
 * Example: node run.js 2025 Day1.1
 *          node run.js 2024 Day21.1
 */

import { spawn } from 'child_process';
import { resolve, dirname } from 'path';
import { fileURLToPath } from 'url';
import { existsSync } from 'fs';

const __dirname = dirname(fileURLToPath(import.meta.url));

// Parse arguments
const [,, year, dayPart] = process.argv;

if (!year || !dayPart) {
    console.error('Usage: node run.js <year> <day>.<part>');
    console.error('Example: node run.js 2025 Day1.1');
    console.error('         node run.js 2024 Day21.1');
    process.exit(1);
}

// Normalize day format (support both "Day1.1" and "1.1")
const normalizedDay = dayPart.startsWith('Day') ? dayPart : `Day${dayPart}`;
const [day, part] = normalizedDay.match(/Day(\d+)\.(\d+)/).slice(1);

// Construct paths to try
const paths = [
    resolve(__dirname, year, `Day${day}`, `Day${day}.${part}.js`),
    resolve(__dirname, year, `Day${day}-nodejs`, `Day${day}.${part}.js`)
];

// Find the first existing file
let scriptPath = null;
for (const path of paths) {
    if (existsSync(path)) {
        scriptPath = path;
        break;
    }
}

if (!scriptPath) {
    console.error(`Error: Could not find script for ${year} Day ${day} Part ${part}`);
    console.error('Tried paths:');
    paths.forEach(p => console.error(`  - ${p}`));
    process.exit(1);
}

// Run the script
console.log(`Running: ${year}/Day${day}.${part}\n`);
const child = spawn('node', [scriptPath], {
    stdio: 'inherit',
    cwd: dirname(scriptPath)
});

child.on('exit', (code) => {
    process.exit(code);
});
