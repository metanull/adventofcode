/**
 * Advent of Code 2024 - Day 21, Part 2
 *
 * Description: https://adventofcode.com/2024/day/21
 * 
 * Robot keypad chain puzzle - with 25 directional robots!
 */

import { readLines, logger, getCallerDir } from './lib/index.js';
import { resolve } from 'path';
import { processCodeThroughRobotChainOptimized, calculateComplexity } from './RobotChainOptimized.js';

const title = 'Day 21, Part 2';

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
        const sequenceLength = processCodeThroughRobotChainOptimized(code, 25);
        const complexity = calculateComplexity(code, sequenceLength);
        
        logger.info(`Code ${code}: sequence length = ${sequenceLength}, complexity = ${complexity}`);
        
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
