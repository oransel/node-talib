/**
 * node-talib - Technical Analysis Library
 * CommonJS entry point
 */

'use strict';

const talib = require('./build/Release/talib.node');

// Export the native module
module.exports = talib;
