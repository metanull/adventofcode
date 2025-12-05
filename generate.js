#!/usr/bin/env node
/**
 * Advent of Code Day Generator
 * Creates skeleton structure for a new day's solutions
 * 
 * Usage: node generate.js <day> [year]
 * Example: node generate.js 5
 *          node generate.js 12 2025
 */

import { mkdirSync, existsSync, readdirSync, writeFileSync } from 'fs';
import { resolve, dirname } from 'path';
import { fileURLToPath } from 'url';

const __dirname = dirname(fileURLToPath(import.meta.url));

// Parse arguments
const [,, dayArg, yearArg] = process.argv;

if (!dayArg) {
    console.error('Usage: node generate.js <day> [year]');
    console.error('Example: node generate.js 5');
    console.error('         node generate.js 12 2025');
    process.exit(1);
}

const day = parseInt(dayArg, 10);
const year = yearArg ? parseInt(yearArg, 10) : new Date().getFullYear();

// Validate inputs
if (isNaN(day) || day < 1 || day > 25) {
    console.error('Error: Day must be an integer between 1 and 25');
    process.exit(1);
}

if (isNaN(year) || year < 2015) {
    console.error('Error: Invalid year');
    process.exit(1);
}

// Paths
const yearDir = resolve(__dirname, String(year));
const dayDir = resolve(yearDir, `Day${day}`);
const inputDir = resolve(yearDir, 'Input');

// Create template for part 1
const createPart1Template = (year, day) => `/**
 * Advent of Code ${year} - Day ${day}, Part 1
 * 
 * Description: https://adventofcode.com/${year}/day/${day}
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day ${day}, Part 1';

logger.setDebug(true);

/**
 * Decode/parse a single line of input
 * @param {string} line - Raw input line
 * @returns {any} Parsed data
 */
function decodeLine(line) {
    // TODO: Implement line parsing
    return line;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day${day}.txt');
    
    logger.info(\`Reading input from: \${inputPath}\`);
    
    // Read all lines
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(\`Total lines: \${lines.length}\`);
    
    // Process each line
    let result = 0;
    
    for (let index = 0; index < lines.length; index++) {
        logger.progress(index + 1, lines.length);
        
        const line = lines[index];
        const data = decodeLine(line);
        
        // TODO: Implement solution logic for each line
        logger.debug(\`Line \${index + 1}:\`, data);
        
        // TODO: Update result based on processed line
        // result += ...;
    }
    
    return result;
}

try {
    logger.section(title);
    const startTime = performance.now();
    const result = solve();
    const endTime = performance.now();
    
    logger.result('Final Answer', result);
    logger.success(\`Completed in \${(endTime - startTime).toFixed(2)}ms\`);
} catch (error) {
    logger.error('Failed to solve:', error.message);
    console.error(error);
    process.exit(1);
}
`;

// Create template for part 2
const createPart2Template = (year, day) => `/**
 * Advent of Code ${year} - Day ${day}, Part 2
 * 
 * Description: https://adventofcode.com/${year}/day/${day}#part2
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day ${day}, Part 2';

logger.setDebug(true);

/**
 * Decode/parse a single line of input
 * @param {string} line - Raw input line
 * @returns {any} Parsed data
 */
function decodeLine(line) {
    // TODO: Implement line parsing
    return line;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day${day}.txt');
    
    logger.info(\`Reading input from: \${inputPath}\`);
    
    // Read all lines
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(\`Total lines: \${lines.length}\`);
    
    // Process each line
    let result = 0;
    
    for (let index = 0; index < lines.length; index++) {
        logger.progress(index + 1, lines.length);
        
        const line = lines[index];
        const data = decodeLine(line);
        
        // TODO: Implement solution logic for each line (Part 2)
        logger.debug(\`Line \${index + 1}:\`, data);
        
        // TODO: Update result based on processed line
        // result += ...;
    }
    
    return result;
}

try {
    logger.section(title);
    const startTime = performance.now();
    const result = solve();
    const endTime = performance.now();
    
    logger.result('Final Answer', result);
    logger.success(\`Completed in \${(endTime - startTime).toFixed(2)}ms\`);
} catch (error) {
    logger.error('Failed to solve:', error.message);
    console.error(error);
    process.exit(1);
}
`;

// Check and create year directory
if (!existsSync(yearDir)) {
    console.log(`Creating year directory: ${year}/`);
    mkdirSync(yearDir, { recursive: true });
}

// Check if day directory exists and is not empty
if (existsSync(dayDir)) {
    const files = readdirSync(dayDir);
    if (files.length > 0) {
        console.error(`Error: Directory ${year}/Day${day}/ already exists and is not empty`);
        console.error('Existing files:', files.join(', '));
        process.exit(1);
    }
} else {
    console.log(`Creating day directory: ${year}/Day${day}/`);
    mkdirSync(dayDir, { recursive: true });
}

// Create Input directory if it doesn't exist
if (!existsSync(inputDir)) {
    console.log(`Creating input directory: ${year}/Input/`);
    mkdirSync(inputDir, { recursive: true });
}

// Create the solution files
const part1Path = resolve(dayDir, `Day${day}.1.js`);
const part2Path = resolve(dayDir, `Day${day}.2.js`);
const inputPath = resolve(inputDir, `Day${day}.txt`);

console.log(`Creating ${year}/Day${day}/Day${day}.1.js`);
writeFileSync(part1Path, createPart1Template(year, day), 'utf8');

console.log(`Creating ${year}/Day${day}/Day${day}.2.js`);
writeFileSync(part2Path, createPart2Template(year, day), 'utf8');

// Create empty input file if it doesn't exist
if (!existsSync(inputPath)) {
    console.log(`Creating empty input file: ${year}/Input/Day${day}.txt`);
    writeFileSync(inputPath, '', 'utf8');
}

console.log('\n✅ Success! Day structure created:');
console.log(`   ${year}/Day${day}/Day${day}.1.js`);
console.log(`   ${year}/Day${day}/Day${day}.2.js`);
console.log(`   ${year}/Input/Day${day}.txt`);
console.log(`\nRun with: npm run aoc ${year} ${day}.1`);
