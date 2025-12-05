/**
 * Logger utility for Advent of Code challenges
 * Provides centralized logging and output formatting
 */

import chalk from 'chalk';

class Logger {
    constructor(options = {}) {
        this.debugMode = options.debug || false;
        this.silent = options.silent || false;
    }

    /**
     * Enable/disable debug mode
     */
    setDebug(enabled) {
        this.debugMode = enabled;
    }

    /**
     * Debug logging - only shows when debug mode is enabled
     */
    debug(...args) {
        if (this.debugMode && !this.silent) {
            console.log(chalk.gray('[DEBUG]'), ...args);
        }
    }

    /**
     * Info logging - general information
     */
    info(...args) {
        if (!this.silent) {
            console.log(chalk.blue('[INFO]'), ...args);
        }
    }

    /**
     * Success logging - highlighted success messages
     */
    success(...args) {
        if (!this.silent) {
            console.log(chalk.green('[SUCCESS]'), ...args);
        }
    }

    /**
     * Warning logging
     */
    warn(...args) {
        if (!this.silent) {
            console.log(chalk.yellow('[WARN]'), ...args);
        }
    }

    /**
     * Error logging
     */
    error(...args) {
        console.error(chalk.red('[ERROR]'), ...args);
    }

    /**
     * Result logging - for final answer display
     */
    result(label, value) {
        if (!this.silent) {
            console.log(chalk.bold.cyan('\n=== RESULT ==='));
            console.log(chalk.cyan(label + ':'), chalk.bold.white(value));
            console.log(chalk.bold.cyan('==============\n'));
        }
    }

    /**
     * Section header
     */
    section(title) {
        if (!this.silent) {
            console.log(chalk.bold.magenta(`\n--- ${title} ---`));
        }
    }

    /**
     * Progress indicator - overwrites same line
     */
    progress(current, total) {
        if (!this.silent && this.debugMode) {
            const percentage = ((current / total) * 100).toFixed(1);
            const message = chalk.gray(`Progress: ${current}/${total} (${percentage}%)`);
            process.stdout.write(`\r${message}`);
            
            // Add newline when complete
            if (current === total) {
                process.stdout.write('\n');
            }
        }
    }

    /**
     * Clear the current line (useful after progress updates)
     */
    clearLine() {
        if (!this.silent) {
            process.stdout.write('\r\x1b[K');
        }
    }
}

// Export singleton instance
export const logger = new Logger();

// Export class for custom instances
export default Logger;
