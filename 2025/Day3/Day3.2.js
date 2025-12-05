/**
 * Advent of Code 2025 - Day 3, Part 2
 *
 * Description: https://adventofcode.com/2025/day/3
 */

import { readLines, logger, sum, int, getCallerDir } from '../lib/index.js';
import { resolve } from 'path';

const title = 'Day 3, Part 2';

logger.setDebug(true);

/**
 * Decode/parse a single line of input
 * @param {string} line - Raw input line (digits 0-9)
 * @returns {number[]} Array of integers, one per digit
 */
function decodeLine(line) {
    return line.split('').map(digit => int(digit));
}

/**
 * Process a single decoded line
 * @param {number[]} digits - Array of digit values
 * @param {number} lineIndex - Line number (0-based)
 * @returns {number} 12-digit number formed by the 12 picked digits
 */
function processLine(digits, lineIndex) {
    const pickedDigits = [];
    let searchStart = 0;

    // Pick exactly 12 digits
    for (let i = 0; i < 12; i++) {
        // Exclude the last (12 - i - 1) digits from the search range
        const excludeFromEnd = 12 - i - 1;
        const searchEnd = digits.length - excludeFromEnd;

        // Find the maximum in the range [searchStart, searchEnd)
        const searchRange = digits.slice(searchStart, searchEnd);
        const maxValue = Math.max(...searchRange);
        const maxIndex = searchRange.indexOf(maxValue);

        pickedDigits.push(maxValue);

        // Move search start to position after the found digit
        searchStart = searchStart + maxIndex + 1;

        logger.debug(`Digit ${i + 1}: max=${maxValue} at absolute pos ${searchStart - 1}`);
    }

    // Convert array of digits to a single number
    const result = parseInt(pickedDigits.join(''), 10);

    logger.debug(`Line ${lineIndex + 1}: picked digits=${pickedDigits.join('')}, result=${result}`);

    return result;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day3.txt');

    logger.info(`Reading input from: ${inputPath}`);

    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Total lines: ${lines.length}`);

    const results = lines.map((line, index) => {
        logger.progress(index + 1, lines.length);
        const decoded = decodeLine(line);
        return processLine(decoded, index);
    });

    return sum(results);
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
