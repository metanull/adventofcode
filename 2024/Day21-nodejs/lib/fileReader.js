/**
 * File reading utilities for Advent of Code challenges
 * Provides various methods to read and process input files
 */

import { readFileSync } from 'fs';
import { resolve, dirname } from 'path';
import { fileURLToPath } from 'url';

/**
 * Get the directory of the calling script
 */
export function getCallerDir(importMetaUrl) {
    return dirname(fileURLToPath(importMetaUrl));
}

/**
 * Read entire file as string
 */
export function readFile(filePath) {
    try {
        return readFileSync(filePath, 'utf8');
    } catch (error) {
        throw new Error(`Failed to read file ${filePath}: ${error.message}`);
    }
}

/**
 * Read file and return array of lines
 */
export function readLines(filePath, options = {}) {
    const content = readFile(filePath);
    const lines = content.split(/\r?\n/);
    
    // Remove empty lines if specified
    if (options.skipEmpty) {
        return lines.filter(line => line.trim().length > 0);
    }
    
    // Remove trailing empty line if exists
    if (lines[lines.length - 1] === '') {
        lines.pop();
    }
    
    return lines;
}

/**
 * Read file and process line by line with a callback
 * Returns array of processed results
 */
export function processLines(filePath, callback, options = {}) {
    const lines = readLines(filePath, options);
    return lines.map((line, index) => callback(line, index));
}

/**
 * Read file and process line by line, aggregating results
 */
export function aggregateLines(filePath, callback, initialValue, options = {}) {
    const lines = readLines(filePath, options);
    return lines.reduce((accumulator, line, index) => {
        return callback(accumulator, line, index);
    }, initialValue);
}

/**
 * Read file as comma-separated values
 */
export function readCSV(filePath, delimiter = ',') {
    const content = readFile(filePath).trim();
    return content.split(delimiter).map(item => item.trim());
}

/**
 * Read file as 2D grid (array of arrays)
 */
export function readGrid(filePath) {
    const lines = readLines(filePath, { skipEmpty: true });
    return lines.map(line => line.split(''));
}

/**
 * Resolve input file path relative to the calling script
 */
export function resolveInputPath(scriptDir, filename) {
    // Try local directory first
    let inputPath = resolve(scriptDir, filename);
    
    // If not found, try ../Input directory
    if (!inputPath) {
        inputPath = resolve(scriptDir, '..', 'Input', filename);
    }
    
    return inputPath;
}
