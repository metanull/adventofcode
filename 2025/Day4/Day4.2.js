/**
 * Advent of Code 2025 - Day 4, Part 2
 *
 * Description: https://adventofcode.com/2025/day/4
 */

import { readLines, logger, getCallerDir } from '../lib/index.js';
import { resolve } from 'path';

const title = 'Day 4, Part 2';

logger.setDebug(true);

/**
 * Decode/parse the entire input into a 2D grid
 * @param {string[]} lines - All input lines
 * @returns {string[][]} 2D grid of characters
 */
function decodeInput(lines) {
    return lines.map(line => line.split(''));
}

/**
 * Get all 8 adjacent neighbors for a position
 * @param {string[][]} grid - 2D grid
 * @param {number} row - Row index
 * @param {number} col - Column index
 * @returns {string[]} Array of neighboring characters
 */
function getNeighbors(grid, row, col) {
    const neighbors = [];
    const rows = grid.length;
    const cols = grid[0].length;

    for (let dr = -1; dr <= 1; dr++) {
        for (let dc = -1; dc <= 1; dc++) {
            if (dr === 0 && dc === 0) continue;

            const newRow = row + dr;
            const newCol = col + dc;

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                neighbors.push(grid[newRow][newCol]);
            }
        }
    }

    return neighbors;
}

/**
 * Find cells matching specific conditions
 * @param {string[][]} grid - 2D grid
 * @returns {Array<[number, number]>} Array of [row, col] coordinates
 */
function findMatchingCells(grid) {
    const matching = [];
    const rows = grid.length;
    const cols = grid[0].length;

    for (let row = 0; row < rows; row++) {
        for (let col = 0; col < cols; col++) {
            logger.progress(row * cols + col + 1, rows * cols);
            if (grid[row][col] === '@') {
                const neighbors = getNeighbors(grid, row, col);
                const paperRollCount = neighbors.filter(n => n === '@').length;

                if (paperRollCount < 4) {
                    matching.push([row, col]);
                }
            }
        }
    }

    return matching;
}

/**
 * Remove matching cells from the grid by replacing '@' with '.'
 * @param {string[][]} grid - 2D grid
 * @param {Array<[number, number]>} cells - Cells to remove
 */
function removeMatchingCells(grid, cells) {
    cells.forEach(([row, col]) => {
        grid[row][col] = '.';
    });
}

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day4.txt');

    logger.info(`Reading input from: ${inputPath}`);

    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Grid size: ${lines.length} x ${lines[0].length}`);

    const grid = decodeInput(lines);
    const results = [];
    let iteration = 0;

    while (true) {
        iteration++;
        logger.debug(`Iteration ${iteration}`);

        const matchingCells = findMatchingCells(grid);

        if (matchingCells.length === 0) {
            logger.debug('No more matching cells found');
            break;
        }

        results.push(matchingCells.length);
        logger.debug(`Found ${matchingCells.length} matching cells`);

        removeMatchingCells(grid, matchingCells);
    }

    const answer = results.reduce((sum, count) => sum + count, 0);
    logger.debug(`Total iterations: ${iteration}, Total cells removed: ${answer}`);

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
