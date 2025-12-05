/**
 * Optimized Robot chain utilities for Advent of Code 2024 Day 21 Part 2
 * Uses memoization to handle deep chains without expanding full sequences
 */

import { createNumericKeypad, createDirectionalKeypad } from './Keypad.js';
import { logger } from '../../lib/index.js';

// Memoization cache for sequence lengths
const lengthCache = new Map();

/**
 * Get a cache key for memoization
 */
function getCacheKey(from, to, depth) {
    return `${from}->${to}@${depth}`;
}

/**
 * Calculate the length of the sequence needed to move from one key to another
 * at a given depth in the robot chain, using memoization
 * 
 * @param {string} fromKey - Starting key
 * @param {string} toKey - Target key
 * @param {number} depth - Number of remaining directional keypads
 * @param {Keypad} keypad - The keypad to use for this level
 * @returns {number} Length of the sequence
 */
function getSequenceLength(fromKey, toKey, depth, keypad) {
    // Base case: if we're at the human level (depth 0), 
    // we just count the moves directly
    if (depth === 0) {
        keypad.currentKey = fromKey;
        const moves = keypad.getMovesToKey(toKey);
        return moves.length;
    }
    
    // Check cache
    const cacheKey = getCacheKey(fromKey, toKey, depth);
    if (lengthCache.has(cacheKey)) {
        return lengthCache.get(cacheKey);
    }
    
    // Get the moves needed for this transition
    keypad.currentKey = fromKey;
    const moves = keypad.getMovesToKey(toKey);
    
    // Now calculate the length needed at the next level
    // We need to consider each transition in the moves sequence
    let totalLength = 0;
    let currentPos = 'A'; // All robots start at 'A'
    
    const nextKeypad = createDirectionalKeypad();
    
    for (const move of moves) {
        // Each move requires a transition from currentPos to move
        // at depth-1 (one level deeper in the robot chain)
        totalLength += getSequenceLength(currentPos, move, depth - 1, nextKeypad);
        currentPos = move;
    }
    
    // Cache and return
    lengthCache.set(cacheKey, totalLength);
    return totalLength;
}

/**
 * Process a code through the robot chain and return just the length
 * @param {string} code - The door code to type (e.g., "029A")
 * @param {number} numDirectionalRobots - Number of directional keypads
 * @returns {number} Length of the final sequence
 */
export function processCodeThroughRobotChainOptimized(code, numDirectionalRobots = 25) {
    lengthCache.clear(); // Clear cache for each code
    
    let totalLength = 0;
    let currentKey = 'A';
    
    const numericKeypad = createNumericKeypad();
    
    // Process each character in the code on the numeric keypad
    for (const targetKey of code) {
        // Calculate length for this transition through all robot layers
        const length = getSequenceLength(currentKey, targetKey, numDirectionalRobots, numericKeypad);
        totalLength += length;
        currentKey = targetKey;
    }
    
    logger.debug(`Code ${code}: sequence length = ${totalLength}`);
    
    return totalLength;
}

/**
 * Calculate the complexity of a code
 * @param {string} code - The door code
 * @param {number} sequenceLength - Length of button press sequence
 * @returns {number} Complexity (length * numeric part of code)
 */
export function calculateComplexity(code, sequenceLength) {
    const numericPart = parseInt(code.match(/\d+/)[0], 10);
    return sequenceLength * numericPart;
}
