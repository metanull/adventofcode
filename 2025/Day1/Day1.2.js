/**
 * Advent of Code 2025 - Day 1, Part 2
 * 
 * Description: https://adventofcode.com/2025/day/1
 */

import { readLines, logger, int, getCallerDir } from '../lib/index.js';
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
        logger.progress(index, lines.length);
        const offset = decodeLine(lines[index]);
        const oldResult = result;
        result += offset;

        if(result == oldResult) {
            // No change
            continue;
        }

        const oldHitsHundreds = oldResult % 100 === 0 ? 1 : 0;
        const newHitsHundreds = result % 100 === 0 ? 1 : 0;
        let crossedHundreds = Math.abs(Math.floor(oldResult/100) - Math.floor(result/100));
        if( 
            (oldHitsHundreds && newHitsHundreds)
            || ( oldHitsHundreds && (result <= oldResult))
            || ( newHitsHundreds && (result > oldResult ))
        ) {
            // Adjust for double counting (as we add 1 if ending on a hundred)
            crossedHundreds -= 1;
        }
        if(crossedHundreds > 0) {
            logger.debug(`Line ${index + 1}: Crossed ${crossedHundreds} hundreds from ${oldResult} to ${result}`);
        }
        if(newHitsHundreds) {
            logger.debug(`Line ${index + 1}: Hit hundred at ${result}`);
        }
        aggregatedResult += crossedHundreds;
        aggregatedResult += newHitsHundreds;
    }
    
    return aggregatedResult;
}

// Run solution
try {
    logger.section('Day 1, Part 2');

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
