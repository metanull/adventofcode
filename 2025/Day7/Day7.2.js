/**
 * Advent of Code 2025 - Day 7, Part 2
 * 
 * Description: https://adventofcode.com/2025/day/7
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day 7, Part 2';

logger.setDebug(false);

/**
 * Parse the input into a 2D grid
 * @param {string[]} lines - Raw input lines
 * @returns {Object} Object containing grid and start position
 */
function parseGrid(lines) {
    const grid = lines.map(line => line.split(''));
    let startPos = null;
    
    // Find the starting position 'S'
    for (let row = 0; row < grid.length; row++) {
        for (let col = 0; col < grid[row].length; col++) {
            if (grid[row][col] === 'S') {
                startPos = { row, col };
                break;
            }
        }
        if (startPos) break;
    }
    
    return { grid, startPos };
}

/**
 * Simulate the tachyon beam and count distinct timelines efficiently
 * @param {Object} grid - The 2D grid
 * @param {Object} startPos - Starting position {row, col}
 * @returns {number} Total number of distinct timelines
 */
function countUniqueTimelines(grid, startPos) {
    const rows = grid.length;
    const cols = grid[0].length;
    
    // Memoization: cache results for (row, col) -> number of timelines
    const memo = new Map();
    
    // DFS with memoization to count timelines from a given position
    function countTimelinesFrom(row, col, depth = 0) {
        const indent = '  '.repeat(depth);
        
        // Check bounds
        if (row >= rows || col < 0 || col >= cols) {
            logger.debug(`${indent}[${row},${col}] OUT OF BOUNDS → 1 timeline`);
            return 1; // Reached end: this is one complete timeline
        }
        
        // Check memo
        const key = `${row},${col}`;
        if (memo.has(key)) {
            const cached = memo.get(key);
            logger.debug(`${indent}[${row},${col}] CACHED → ${cached} timelines`);
            return cached;
        }
        
        const cell = grid[row][col];
        logger.debug(`${indent}[${row},${col}] cell='${cell}'`);
        
        let result;
        if (cell === '^') {
            // At a splitter: count timelines from both left and right
            logger.debug(`${indent}  SPLIT: exploring left and right...`);
            const leftTimelines = countTimelinesFrom(row, col - 1, depth + 1);
            const rightTimelines = countTimelinesFrom(row, col + 1, depth + 1);
            result = leftTimelines + rightTimelines;
            logger.debug(`${indent}  SPLIT result: ${leftTimelines} (left) + ${rightTimelines} (right) = ${result} timelines`);
        } else {
            // Continue downward
            logger.debug(`${indent}  continuing downward...`);
            result = countTimelinesFrom(row + 1, col, depth + 1);
            logger.debug(`${indent}  downward result: ${result} timelines`);
        }
        
        memo.set(key, result);
        logger.debug(`${indent}[${row},${col}] MEMO SET → ${result} timelines`);
        return result;
    }
    
    // Start from the position below the start
    logger.debug(`\nStarting timeline exploration from (${startPos.row + 1}, ${startPos.col}):`);
    return countTimelinesFrom(startPos.row + 1, startPos.col);
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day7.txt');
    
    logger.info(`Reading input from: ${inputPath}`);
    
    // Read all lines
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Total lines: ${lines.length}`);
    
    // Parse the grid
    const { grid, startPos } = parseGrid(lines);
    logger.debug(`Grid size: ${grid.length} x ${grid[0].length}`);
    logger.debug(`Start position: (${startPos.row}, ${startPos.col})`);
    
    // Count unique cells visited across all timelines
    const result = countUniqueTimelines(grid, startPos);
    
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
