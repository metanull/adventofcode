/**
 * Common utilities for Advent of Code challenges
 */

/**
 * Sum an array of numbers
 */
export function sum(numbers) {
    return numbers.reduce((acc, num) => acc + num, 0);
}

/**
 * Product of an array of numbers
 */
export function product(numbers) {
    return numbers.reduce((acc, num) => acc * num, 1);
}

/**
 * Count occurrences in an array
 */
export function count(array, predicate) {
    return array.filter(predicate).length;
}

/**
 * Parse integer safely
 */
export function int(value) {
    const parsed = parseInt(value, 10);
    if (isNaN(parsed)) {
        throw new Error(`Cannot parse integer: ${value}`);
    }
    return parsed;
}

/**
 * Parse all integers from a string
 */
export function extractIntegers(str) {
    const matches = str.match(/-?\d+/g);
    return matches ? matches.map(int) : [];
}

/**
 * Split string by whitespace
 */
export function splitWhitespace(str) {
    return str.trim().split(/\s+/);
}

/**
 * Measure execution time
 */
export function measureTime(fn) {
    const start = performance.now();
    const result = fn();
    const end = performance.now();
    return {
        result,
        time: end - start
    };
}

/**
 * Async version of measureTime
 */
export async function measureTimeAsync(fn) {
    const start = performance.now();
    const result = await fn();
    const end = performance.now();
    return {
        result,
        time: end - start
    };
}

/**
 * Create a 2D array filled with a value
 */
export function create2DArray(rows, cols, fillValue = 0) {
    return Array(rows).fill(null).map(() => Array(cols).fill(fillValue));
}

/**
 * Deep clone an object
 */
export function deepClone(obj) {
    return JSON.parse(JSON.stringify(obj));
}

/**
 * Range generator
 */
export function range(start, end, step = 1) {
    const result = [];
    if (step > 0) {
        for (let i = start; i < end; i += step) {
            result.push(i);
        }
    } else {
        for (let i = start; i > end; i += step) {
            result.push(i);
        }
    }
    return result;
}

/**
 * Get unique values from array
 */
export function unique(array) {
    return [...new Set(array)];
}

/**
 * Group array elements by a key function
 */
export function groupBy(array, keyFn) {
    return array.reduce((groups, item) => {
        const key = keyFn(item);
        if (!groups[key]) {
            groups[key] = [];
        }
        groups[key].push(item);
        return groups;
    }, {});
}
