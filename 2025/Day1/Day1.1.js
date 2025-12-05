/**
 * Advent of Code 2025 - Day 1, Part 1
 * 
 * Description: https://adventofcode.com/2025/day/1
 */

import chalk from 'chalk';
import { readLines, logger, int, getCallerDir} from '../lib/index.js';
import { resolve } from 'path';

// Enable debug mode for detailed logging
logger.setDebug(false);

/**
 * Decode/parse a single line of input
 * @param {string} line - Raw input line (format: R123 or L123)
 * @returns {number} Positive for R, negative for L
 */
function decodeLine(line) {
    const direction = line[0];
    const value = int(line.slice(1));
    return direction === 'L' ? -value : value;
}

/**
 * Main solution
 */
function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day1.txt');
    
    logger.info(`Reading input from: ${inputPath}`);
    
    // Read and process all lines
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Total lines: ${lines.length}`);
    
    // Process each line with accumulating result
    let result = 50;
    let aggregatedResult = 0;
    
    for (let index = 0; index < lines.length; index++) {
        const decoded = decodeLine(lines[index]);
        const prevResult = result;
        result = result + decoded;
        if(result % 100 === 0) {
            aggregatedResult += 1;
        }
        
        // Print formatted line
        const indexStr = chalk.gray(`[${String(index + 1).padStart(6)}]`);
        const currentStr = chalk.cyan(`${prevResult}`.padStart(6));
        const operationStr = decoded < 0 ? chalk.hex('#FFA500')('-') : chalk.green('+');
        const operationValueStr = chalk.cyan(String(Math.abs(decoded)).padStart(6));
        const equalsStr = chalk.gray('=');
        const resultStr = result % 100 === 0 ? chalk.green(String(result).padStart(6)) : chalk.white(String(result).padStart(6));
        logger.debug(`${indexStr} ${currentStr} ${operationStr} ${operationValueStr} ${equalsStr} ${resultStr}`);
        
        if(result === 0) {
            logger.info(`Reached zero at line ${index + 1}`);
        }
    }
    
    return aggregatedResult;
}

// Run solution
try {
    logger.section('Day 1, Part 1');

    const startTime = performance.now();
    const result = solve();
    const endTime = performance.now();
    logger.result('Final Answer', result);
    
    logger.success(`Completed in ${(endTime - startTime).toFixed(2)}ms`);
} catch (error) {
    logger.error('Failed to solve:', error.message);
    console.error(error);
    process.exit(1);
}
