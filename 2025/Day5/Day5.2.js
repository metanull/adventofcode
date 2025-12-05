/**
 * Advent of Code 2025 - Day 5, Part 2
 *
 * Description: https://adventofcode.com/2025/day/5
 */

import { readLines, logger, int, getCallerDir } from '../lib/index.js';
import { resolve } from 'path';

const title = 'Day 5, Part 2';

logger.setDebug(true);

/**
 * Decode/parse the input into ranges
 * @param {string[]} lines - All input lines
 * @returns {Array<[number, number]>} Array of [start, end] ranges
 */
function decodeInput(lines) {
    const blankIndex = lines.findIndex(line => line.trim() === '');
    const rangeLines = lines.slice(0, blankIndex);

    return rangeLines.map(line => {
        const [start, end] = line.split('-').map(int);
        return [start, end];
    });
}

/**
 * Check if two ranges overlap (including touching ranges like 1-3 and 4-5)
 * @param {[number, number]} range1 - First range [start, end]
 * @param {[number, number]} range2 - Second range [start, end]
 * @returns {boolean} True if ranges overlap or are adjacent
 */
function rangesOverlap(range1, range2) {
    const [start1, end1] = range1;
    const [start2, end2] = range2;

    return !(end1 < start2 - 1 || end2 < start1 - 1);
}

/**
 * Merge two overlapping ranges
 * @param {[number, number]} range1 - First range
 * @param {[number, number]} range2 - Second range
 * @returns {[number, number]} Merged range
 */
function mergeRanges(range1, range2) {
    const [start1, end1] = range1;
    const [start2, end2] = range2;

    return [Math.min(start1, start2), Math.max(end1, end2)];
}

/**
 * Build a consolidated list of non-overlapping ranges
 * @param {Array<[number, number]>} ranges - Input ranges (may overlap)
 * @returns {Array<[number, number]>} Consolidated non-overlapping ranges (sorted)
 */
function consolidateRanges(ranges) {
    if (ranges.length === 0) return [];

    const sorted = [...ranges].sort((a, b) => a[0] - b[0]);
    const consolidated = [sorted[0]];

    for (let i = 1; i < sorted.length; i++) {
        logger.progress(i, sorted.length);
        const currentRange = sorted[i];
        const lastConsolidated = consolidated[consolidated.length - 1];

        if (rangesOverlap(lastConsolidated, currentRange)) {
            consolidated[consolidated.length - 1] = mergeRanges(lastConsolidated, currentRange);
        } else {
            consolidated.push(currentRange);
        }
    }

    return consolidated;
}

/**
 * Count total distinct IDs covered by all ranges
 * @param {Array<[number, number]>} ranges - Non-overlapping ranges
 * @returns {number} Total count of distinct IDs
 */
function countDistinctIds(ranges) {
    return ranges.reduce((sum, [start, end]) => {
        return sum + (end - start + 1);
    }, 0);
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day5.txt');

    logger.info(`Reading input from: ${inputPath}`);

    const lines = readLines(inputPath);
    const ranges = decodeInput(lines);

    logger.info(`Input ranges: ${ranges.length}`);

    const consolidated = consolidateRanges(ranges);
    logger.debug(`Consolidated ranges: ${consolidated.length}`);
    consolidated.forEach(([start, end]) => {
        logger.debug(`  [${start}, ${end}]`);
    });

    const answer = countDistinctIds(consolidated);

    logger.debug(`Total distinct IDs: ${answer}`);

    return answer;
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
