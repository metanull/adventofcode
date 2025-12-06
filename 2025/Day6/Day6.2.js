/**
 * Advent of Code 2025 - Day 6, Part 1
 * 
 * Description: https://adventofcode.com/2025/day/6
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day 6, Part 2';

logger.setDebug(true);

/**
 * Detect column boundaries by finding operator positions in the operator line
 * @param {string} operatorLine - The line containing operators (+ or *)
 * @returns {number[]} Array of starting positions for each column
 */
function detectColumnBoundaries(operatorLine) {
    const boundaries = [];
    let i = 0;
    
    while (i < operatorLine.length) {
        // Skip spaces
        while (i < operatorLine.length && operatorLine[i] === ' ') {
            i++;
        }
        
        if (i < operatorLine.length) {
            // Found start of a column (operator position)
            boundaries.push(i);
            i++; // Move past the operator
        }
    }
    
    return boundaries;
}

/**
 * Get the width of a column based on operator positions
 * @param {number[]} boundaries - Column starting positions
 * @param {number} columnIndex - Index of the column
 * @param {number} maxWidth - Maximum line width
 * @returns {number} Width of the column
 */
function getColumnWidth(boundaries, columnIndex, maxWidth) {
    if (columnIndex < boundaries.length - 1) {
        return boundaries[columnIndex + 1] - boundaries[columnIndex];
    } else {
        // Last column extends to end of line
        return maxWidth - boundaries[columnIndex];
    }
}

/**
 * Generator that yields columns one at a time as fixed-width strings
 * @param {string[]} lines - All input lines
 * @yields {{strings: string[], operator: string, columnIndex: number, columnWidth: number}}
 */
function* columnReader(lines) {
    if (lines.length === 0) return;
    
    // The last line contains operators, others are data
    const operatorLine = lines[lines.length - 1];
    const dataLines = lines.slice(0, -1);
    
    // Detect column boundaries based on operator positions
    const boundaries = detectColumnBoundaries(operatorLine);
    
    if (boundaries.length === 0) return;
    
    // Get the maximum width from the longest line
    const maxWidth = Math.max(
        ...lines.map(line => line.length),
        operatorLine.length
    );
    
    let columnIndex = 0;
    
    // Read each column
    for (const boundary of boundaries) {
        const columnWidth = getColumnWidth(boundaries, columnIndex, maxWidth);
        const strings = [];
        
        // Extract strings from each data line with fixed width
        for (const line of dataLines) {
            const endPos = Math.min(boundary + columnWidth, line.length);
            const str = line.substring(boundary, endPos);
            // Pad if necessary to maintain column width
            const padded = str.length < columnWidth ? str + ' '.repeat(columnWidth - str.length) : str;
            strings.push(padded);
        }
        
        // Extract operator (first non-space character in this column from operator line)
        const endPos = Math.min(boundary + columnWidth, operatorLine.length);
        const opStr = operatorLine.substring(boundary, endPos).trim();
        const operator = opStr || '+';
        
        yield { strings, operator, columnIndex, columnWidth };
        columnIndex++;
    }
}

/**
 * Extract numbers from column strings (read right-to-left)
 * Each character in a column position represents a digit
 * Numbers are formed by reading the column vertically from bottom to top (excluding the operator)
 * @param {string[]} strings - Array of strings representing each line in the column
 * @param {number} columnWidth - Width of the column
 * @returns {number[]} Array of numbers extracted from the column
 */
function extractNumbersFromColumn(strings, columnWidth) {
    const numbers = [];
    
    // Process each character position from right to left in the column
    for (let charPos = columnWidth - 1; charPos >= 0; charPos--) {
        let numberStr = '';
        
        // Read from top to bottom, extracting the digit at this position
        for (const str of strings) {
            if (charPos < str.length) {
                const char = str[charPos];
                numberStr += char; // Build the number string top-to-bottom
            }
        }
        
        // Try to parse as a number (trim spaces and convert)
        const num = parseInt(numberStr, 10);
        if (!isNaN(num) && num !== 0) {
            numbers.push(num);
        }
    }
    
    return numbers;
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
    
    for (const { strings, operator, columnIndex, columnWidth } of columnReader(lines)) {
        columnCount++;
        
        // Extract numbers from the column strings
        const numbers = extractNumbersFromColumn(strings, columnWidth);
        
        logger.debug(`Column ${columnIndex + 1} (width ${columnWidth}): strings=[${strings.map(s => `"${s}"`).join(', ')}]`);
        logger.debug(`  Extracted numbers: [${numbers.join(', ')}] [${operator}]`);
        
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
