/**
 * Advent of Code 2024 - Day 21, Part 1
 *
 * Description: https://adventofcode.com/2024/day/21
 * 
 * Robot keypad chain puzzle - finding shortest sequences to type door codes
 */

import { readLines, logger, getCallerDir } from './lib/index.js';
import { resolve } from 'path';
import { processCodeThroughRobotChain, calculateComplexity } from './RobotChain.js';

const title = 'Day 21, Part 1';

logger.setDebug(true);

function solve() {
    const scriptDir = getCallerDir(import.meta.url);
    // Use sample-input.txt for testing, or Day21.txt for actual input
    // const inputPath = resolve(scriptDir, 'sample-input.txt');
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day21.txt');
    
    logger.info(`Reading input from: ${inputPath}`);
    
    const lines = readLines(inputPath);
    const codes = lines.filter(line => line.trim().length > 0);
    
    logger.info(`Processing ${codes.length} codes`);
    
    let totalComplexity = 0;
    
    for (const code of codes) {
        const result = processCodeThroughRobotChain(code);
        const complexity = calculateComplexity(code, result.length);
        
        logger.info(`Code ${code}: sequence length = ${result.length}, complexity = ${complexity}`);
        logger.debug(`  Sequence: ${result.sequence.join('')}`);
        
        totalComplexity += complexity;
    }
    
    return totalComplexity;
}

try {
    logger.section(title);
    const startTime = performance.now();
    const result = solve();
    const endTime = performance.now();
    
    logger.result('Sum of Complexities', result);
    logger.success(`Completed in ${(endTime - startTime).toFixed(2)}ms`);
} catch (error) {
    logger.error('Failed to solve:', error.message);
    console.error(error);
    process.exit(1);
}
