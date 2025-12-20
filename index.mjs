/**
 * node-talib - Technical Analysis Library
 * ES Module entry point
 */

import { createRequire } from 'module';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
const require = createRequire(import.meta.url);

const talib = require('./build/Release/talib.node');

export const {
  version,
  functions,
  functionUnstIds,
  explain,
  execute,
  setUnstablePeriod
} = talib;

export default talib;
