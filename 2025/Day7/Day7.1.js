/**
 * Advent of Code 2025 - Day 7, Part 1
 * 
 * Description: https://adventofcode.com/2025/day/7
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import { resolve } from 'path';

const title = 'Day 7, Part 1';

logger.setDebug(true);

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
 * Simulate the tachyon beam and count splits
 * @param {Object} grid - The 2D grid
 * @param {Object} startPos - Starting position {row, col}
 * @returns {number} Total number of splits
 */
function countBeamSplits(grid, startPos) {
    const rows = grid.length;
    const cols = grid[0].length;
    let splitCount = 0;
    
    // Each beam is represented as {row, col}
    // All beams move downward
    const beams = [{ row: startPos.row, col: startPos.col }];
    
    // Track visited positions to avoid infinite loops
    const visited = new Set();
    
    while (beams.length > 0) {
        logger.progress(visited.size, rows * cols);
        
        const beam = beams.shift();
        let { row, col } = beam;
        
        // Move the beam downward one step
        row++;
        
        // Check if beam is out of bounds
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            continue;
        }
        
        // Create a unique state identifier
        const stateId = `${row},${col}`;
        if (visited.has(stateId)) {
            continue; // Skip if we've already processed this position
        }
        visited.add(stateId);
        
        const cell = grid[row][col];
        
        // If we hit a splitter (^), split the beam
        if (cell === '^') {
            splitCount++;
            // Create two new beams: one at immediate left, one at immediate right
            // Both continue moving downward
            beams.push({ row, col: col - 1 });
            beams.push({ row, col: col + 1 });
        } else {
            // Continue in the same column
            beams.push({ row, col });
        }
    }
    
    return splitCount;
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
    
    // Count the beam splits
    const result = countBeamSplits(grid, startPos);
    
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
