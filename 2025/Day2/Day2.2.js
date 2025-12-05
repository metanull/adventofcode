/**
 * Advent of Code 2025 - Day 2, Part 2
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
 * Check if a number is an invalid ID (a sequence of digits repeated at least twice)
 * @param {number} num - The number to check
 * @returns {boolean} True if number is a repeated pattern
 */
function isRepeatedPattern(num) {
    const str = num.toString();
    const len = str.length;

    // Try all possible pattern lengths (from 1 to len/2)
    for (let patternLen = 1; patternLen <= len / 2; patternLen++) {
        // Check if the string length is divisible by pattern length
        if (len % patternLen !== 0) {
            continue;
        }

        const pattern = str.substring(0, patternLen);
        const repetitions = len / patternLen;

        // Check if the entire string is this pattern repeated
        if (pattern.repeat(repetitions) === str) {
            return true;
        }
    }

    return false;
}

/**
 * Find all invalid IDs in a range that match the repeated pattern
 * @param {number} start - Start of range (inclusive)
 * @param {number} end - End of range (inclusive)
 * @returns {number[]} Array of invalid IDs
 */
function findInvalidIdsInRange(start, end) {
    const invalidIds = [];

    for (let id = start; id <= end; id++) {
        if (isRepeatedPattern(id)) {
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

// Run solution
try {
    logger.section('Day 2, Part 2');

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
