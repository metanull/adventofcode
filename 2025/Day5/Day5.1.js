/**
 * Advent of Code 2025 - Day 5, Part 1
 *
 * Description: https://adventofcode.com/2025/day/5
 */

import { readLines, logger, int, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day 5, Part 1';

logger.setDebug(true);

/**
 * Decode/parse the input into ranges and available IDs
 * @param {string[]} lines - All input lines
 * @returns {{ranges: Array<[number, number]>, available: number[]}} Parsed data
 */
function decodeInput(lines) {
    const blankIndex = lines.findIndex(line => line.trim() === '');

    const rangeLines = lines.slice(0, blankIndex);
    const availableLines = lines.slice(blankIndex + 1);

    const ranges = rangeLines.map(line => {
        const [start, end] = line.split('-').map(int);
        return [start, end];
    });

    const available = availableLines.map(line => int(line.trim()));

    return { ranges, available };
}

/**
 * Check if an ID falls within any of the ranges
 * @param {number} id - Ingredient ID to check
 * @param {Array<[number, number]>} ranges - Array of [start, end] ranges
 * @returns {boolean} True if ID is in any range
 */
function isInAnyRange(id, ranges) {
    return ranges.some(([start, end]) => id >= start && id <= end);
}

/**
 * Find available IDs that match fresh ingredient ranges
 * @param {Array<[number, number]>} ranges - Fresh ingredient ranges
 * @param {number[]} available - Available ingredient IDs
 * @returns {number[]} Array of matching IDs
 */
function findMatchingIds(ranges, available) {
    const matching = [];

    available.forEach((id, index) => {
        logger.progress(index + 1, available.length);

        if (isInAnyRange(id, ranges)) {
            matching.push(id);
        }
    });

    return matching;
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day5.txt');

    logger.info(`Reading input from: ${inputPath}`);

    const lines = readLines(inputPath);
    const { ranges, available } = decodeInput(lines);

    logger.info(`Fresh ranges: ${ranges.length}, Available IDs: ${available.length}`);

    const matchingIds = findMatchingIds(ranges, available);

    logger.debug(`Found ${matchingIds.length} matching IDs`);

    return matchingIds.length;
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
