/**
 * Robot chain utilities for Advent of Code 2024 Day 21
 * Handles chaining multiple keypads together
 */

import { createNumericKeypad, createDirectionalKeypad } from './Keypad.js';
import { logger } from '../../lib/index.js';

/**
 * Process a code through the robot chain
 * @param {string} code - The door code to type (e.g., "029A")
 * @param {number} numDirectionalRobots - Number of directional keypads (default: 2)
 * @returns {{sequence: string[], length: number}} Final sequence and its length
 */
export function processCodeThroughRobotChain(code, numDirectionalRobots = 2) {
    // Start with the numeric keypad
    const numericKeypad = createNumericKeypad();
    let sequence = numericKeypad.getSequenceForCode(code.split(''));
    
    logger.debug(`After numeric keypad: ${sequence.join('')} (length: ${sequence.length})`);
    
    // Process through directional keypads
    for (let i = 0; i < numDirectionalRobots; i++) {
        const directionalKeypad = createDirectionalKeypad();
        sequence = directionalKeypad.getSequenceForCode(sequence);
        logger.debug(`After directional keypad ${i + 1}: ${sequence.join('')} (length: ${sequence.length})`);
    }
    
    return {
        sequence,
        length: sequence.length
    };
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
