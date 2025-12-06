/**
 * Advent of Code 2025 - Day 6, Part 1
 * 
 * Description: https://adventofcode.com/2025/day/6
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day 6, Part 1';

logger.setDebug(true);

/**
 * Skip whitespace and find the start of the next token
 * @param {string} line - The line to scan
 * @param {number} startPos - Position to start scanning from
 * @returns {number} Position of next non-whitespace character, or -1 if none
 */
function skipWhitespace(line, startPos) {
    while (startPos < line.length && line[startPos] === ' ') {
        startPos++;
    }
    return startPos < line.length ? startPos : -1;
}

/**
 * Read the next integer from a line starting at a given position
 * @param {string} line - The line to read from
 * @param {number} startPos - Position to start reading from
 * @returns {{value: number|string, endPos: number} | null} Object with parsed value and end position, or null
 */
function readNextToken(line, startPos) {
    // Skip leading whitespace
    const tokenStart = skipWhitespace(line, startPos);
    if (tokenStart === -1) return null;
    
    // Find the end of the token (next whitespace or end of line)
    let tokenEnd = tokenStart;
    while (tokenEnd < line.length && line[tokenEnd] !== ' ') {
        tokenEnd++;
    }
    
    const token = line.substring(tokenStart, tokenEnd);
    
    // Try to parse as integer, otherwise return as string (for operators)
    const num = parseInt(token, 10);
    const value = isNaN(num) ? token : num;
    
    return { value, endPos: tokenEnd };
}

/**
 * Generator that yields columns one at a time
 * @param {string[]} lines - All input lines
 * @yields {{numbers: number[], operator: string, columnIndex: number}}
 */
function* columnReader(lines) {
    if (lines.length === 0) return;
    
    // The last line contains operators, others are data
    const operatorLine = lines[lines.length - 1];
    const dataLines = lines.slice(0, -1);
    
    // Track the current position in each line
    const linePositions = dataLines.map(() => 0);
    let operatorPos = 0;
    
    let columnIndex = 0;
    
    // Keep reading columns until all lines are exhausted
    while (true) {
        const numbers = [];
        let hasData = false;
        
        // Read the next number from each data line
        for (let i = 0; i < dataLines.length; i++) {
            const result = readNextToken(dataLines[i], linePositions[i]);
            if (result !== null) {
                hasData = true;
                linePositions[i] = result.endPos; // Update position for this line
                
                if (typeof result.value === 'number') {
                    numbers.push(result.value);
                }
            }
        }
        
        // Read the operator for this column
        const opResult = readNextToken(operatorLine, operatorPos);
        if (opResult !== null) {
            hasData = true;
            operatorPos = opResult.endPos;
        }
        
        // If no data was read from any line, we're done
        if (!hasData) break;
        
        const operator = (opResult && typeof opResult.value === 'string') ? opResult.value : '+';
        
        yield { numbers, operator, columnIndex };
        columnIndex++;
    }
}

/**
 * Process a single column based on its operator
 * @param {number[]} numbers - Array of numbers in the column
 * @param {string} operator - Operation to perform ('*' or '+')
 * @returns {number} Result of the operation
 */
function processColumn(numbers, operator) {
    if (numbers.length === 0) return 0;
    
    if (operator === '*') {
        return numbers.reduce((acc, num) => acc * num, 1);
    } else if (operator === '+') {
        return numbers.reduce((acc, num) => acc + num, 0);
    }
    
    return 0;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day6.txt');
    
    logger.info(`Reading input from: ${inputPath}`);
    
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Total lines: ${lines.length}`);
    
    // Process columns one at a time using the generator
    let result = 0;
    let columnCount = 0;
    
    for (const { numbers, operator, columnIndex } of columnReader(lines)) {
        columnCount++;
        
        logger.debug(`Column ${columnIndex + 1}: ${numbers.join(', ')} [${operator}]`);
        
        const columnResult = processColumn(numbers, operator);
        logger.debug(`  Result: ${columnResult}`);
        
        result += columnResult;
    }
    
    logger.info(`Processed ${columnCount} columns`);
    
    return result;
}

try {
    logger.section(title);
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
