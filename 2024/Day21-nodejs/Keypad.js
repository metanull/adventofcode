/**
 * Keypad class for Advent of Code 2024 Day 21
 * Represents a keypad (numeric or directional) with position tracking
 */

export class Keypad {
    /**
     * @param {Object} layout - Object mapping keys to {x, y} positions
     * @param {{x: number, y: number}} gapPos - Position of the gap (invalid position)
     */
    constructor(layout, gapPos) {
        this.layout = layout;
        this.gapPos = gapPos;
        this.currentKey = 'A'; // All robots start at 'A'
    }

    /**
     * Get the position of a key
     * @param {string} key - The key to find
     * @returns {{x: number, y: number}} Position of the key
     */
    getPosition(key) {
        return this.layout[key];
    }

    /**
     * Find the shortest path from current position to target key, avoiding the gap
     * @param {string} targetKey - The key to move to
     * @returns {string[]} Array of directional moves ('^', 'v', '<', '>', 'A')
     */
    getMovesToKey(targetKey) {
        const start = this.getPosition(this.currentKey);
        const end = this.getPosition(targetKey);
        
        const dx = end.x - start.x;
        const dy = end.y - start.y;
        
        // Generate horizontal and vertical moves
        const horizontalMoves = [];
        const verticalMoves = [];
        
        // Horizontal: negative dx means move left (<), positive means right (>)
        for (let i = 0; i < Math.abs(dx); i++) {
            horizontalMoves.push(dx < 0 ? '<' : '>');
        }
        
        // Vertical: negative dy means move up (^), positive means down (v)
        for (let i = 0; i < Math.abs(dy); i++) {
            verticalMoves.push(dy < 0 ? '^' : 'v');
        }
        
        // Determine order: avoid the gap position
        // Check if horizontal-first path crosses gap
        const hFirstCrossesGap = start.y === this.gapPos.y && end.x === this.gapPos.x;
        // Check if vertical-first path crosses gap
        const vFirstCrossesGap = start.x === this.gapPos.x && end.y === this.gapPos.y;
        
        let moves = [];
        
        if (hFirstCrossesGap && !vFirstCrossesGap) {
            // Must go vertical first
            moves = [...verticalMoves, ...horizontalMoves];
        } else if (vFirstCrossesGap && !hFirstCrossesGap) {
            // Must go horizontal first
            moves = [...horizontalMoves, ...verticalMoves];
        } else {
            // No gap conflict - prefer order that minimizes direction changes
            // Heuristic: prefer left moves first, then vertical, then right
            if (dx < 0) {
                // Going left: do horizontal first
                moves = [...horizontalMoves, ...verticalMoves];
            } else {
                // Going right or no horizontal: do vertical first
                moves = [...verticalMoves, ...horizontalMoves];
            }
        }
        
        // Add 'A' to press the button
        moves.push('A');
        
        // Update current position
        this.currentKey = targetKey;
        
        return moves;
    }

    /**
     * Get sequence of moves for a full code
     * @param {string[]} code - Array of keys to press
     * @returns {string[]} Complete sequence of directional moves
     */
    getSequenceForCode(code) {
        const sequence = [];
        for (const key of code) {
            const moves = this.getMovesToKey(key);
            sequence.push(...moves);
        }
        return sequence;
    }

    /**
     * Reset to initial state (at 'A')
     */
    reset() {
        this.currentKey = 'A';
    }
}

/**
 * Create a numeric keypad
 * Layout:
 * 7 8 9
 * 4 5 6
 * 1 2 3
 *   0 A
 */
export function createNumericKeypad() {
    const layout = {
        '7': { x: 0, y: 0 },
        '8': { x: 1, y: 0 },
        '9': { x: 2, y: 0 },
        '4': { x: 0, y: 1 },
        '5': { x: 1, y: 1 },
        '6': { x: 2, y: 1 },
        '1': { x: 0, y: 2 },
        '2': { x: 1, y: 2 },
        '3': { x: 2, y: 2 },
        '0': { x: 1, y: 3 },
        'A': { x: 2, y: 3 }
    };
    const gapPos = { x: 0, y: 3 };
    return new Keypad(layout, gapPos);
}

/**
 * Create a directional keypad
 * Layout:
 *   ^ A
 * < v >
 */
export function createDirectionalKeypad() {
    const layout = {
        '^': { x: 1, y: 0 },
        'A': { x: 2, y: 0 },
        '<': { x: 0, y: 1 },
        'v': { x: 1, y: 1 },
        '>': { x: 2, y: 1 }
    };
    const gapPos = { x: 0, y: 0 };
    return new Keypad(layout, gapPos);
}
