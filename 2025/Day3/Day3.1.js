/**
 * Advent of Code 2025 - Day 3, Part 1
 *
 * Description: https://adventofcode.com/2025/day/3
 */

import { readLines, logger, sum, int, getCallerDir } from '../lib/index.js';
import { resolve } from 'path';

const title = 'Day 3, Part 1';

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
 * @returns {number} Two-digit number: (highest * 10) + (second highest)
 */
function processLine(digits, lineIndex) {
    // Find the first occurrence of the highest number in the first n-1 digits
    const digitsExceptLast = digits.slice(0, -1);
    const maxValue1 = Math.max(...digitsExceptLast);
    const pos1 = digitsExceptLast.indexOf(maxValue1);

    // Find the first occurrence of the highest number after position of (1)
    const remaining = digits.slice(pos1 + 1);
    const maxValue2 = Math.max(...remaining);
    const pos2 = pos1 + 1 + remaining.indexOf(maxValue2);

    // Result is (1)*10 + (2)
    const result = maxValue1 * 10 + maxValue2;

    logger.debug(`Line ${lineIndex + 1}: highest=${maxValue1} at pos ${pos1}, second=${maxValue2} at pos ${pos2}, result=${result}`);

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
