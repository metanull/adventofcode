/**
 * Advent of Code 2025 - Day 2, Part 1
 *
 * Description: https://adventofcode.com/2025/day/2
 */

import { readLines, logger, sum, int, getCallerDir } from '../lib/index.js';
import { resolve } from 'path';

logger.setDebug(true);

/**
 * Decode/parse a single line of input
 * @param {string} line - Raw input line (comma-separated ranges: "11-22,95-115,...")
 * @returns {Array<[number, number]>} Array of [start, end] range tuples
 */
function decodeLine(line) {
    const ranges = line.split(',').filter(r => r.trim().length > 0);
    return ranges.map(range => {
        const [start, end] = range.trim().split('-').map(int);
        return [start, end];
    });
}

/**
 * Check if a number is a "doubled" invalid ID (a sequence of digits repeated twice)
 * @param {number} num - The number to check
 * @returns {boolean} True if number is doubled pattern
 */
function isDoubledPattern(num) {
    const str = num.toString();
    const len = str.length;

    // Must be even length
    if (len % 2 !== 0) {
        return false;
    }

    const half = len / 2;
    const first = str.substring(0, half);
    const second = str.substring(half);

    return first === second;
}

/**
 * Find all invalid IDs in a range that match the doubled pattern
 * @param {number} start - Start of range (inclusive)
 * @param {number} end - End of range (inclusive)
 * @returns {number[]} Array of invalid IDs
 */
function findInvalidIdsInRange(start, end) {
    const invalidIds = [];

    for (let id = start; id <= end; id++) {
        if (isDoubledPattern(id)) {
            invalidIds.push(id);
        }
    }

    return invalidIds;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day2.txt');

    logger.info(`Reading input from: ${inputPath}`);

    // Read input (1 single line)
    const lines = readLines(inputPath, { skipEmpty: true });
    const decoded = decodeLine(lines[0]);
    const allInvalidIds = [];

    decoded.forEach(([start, end], index) => {
        logger.progress(index + 1, decoded.length);
        const invalidIds = findInvalidIdsInRange(start, end);
        allInvalidIds.push(...invalidIds);
    });
    
    return sum(allInvalidIds);
}

try {
    logger.section('Day 2, Part 1');

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
