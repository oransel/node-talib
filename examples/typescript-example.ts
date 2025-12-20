/**
 * TypeScript Example
 * Run with: npx tsx examples/typescript-example.ts
 */

import * as talib from '../index';
import { readFileSync } from 'fs';

interface MarketDataFile {
  open: number[];
  high: number[];
  low: number[];
  close: number[];
  volume: number[];
}

// Display version
console.log('\nTALib Version:', talib.version);

// Load market data
const marketData: MarketDataFile = JSON.parse(
  readFileSync('examples/marketdata.json', 'utf8')
);

// Calculate indicators with type safety
const sma = talib.execute({
  name: 'SMA',
  startIdx: 0,
  endIdx: marketData.close.length - 1,
  inReal: marketData.close,
  optInTimePeriod: 20
});

const rsi = talib.execute({
  name: 'RSI',
  startIdx: 0,
  endIdx: marketData.close.length - 1,
  inReal: marketData.close,
  optInTimePeriod: 14
});

// Get function explanation
const adxInfo = talib.explain('ADX');

console.log('\nADX Function Info:');
console.log('Name:', adxInfo.name);
console.log('Group:', adxInfo.group);
console.log('Description:', adxInfo.hint);

console.log('\nSMA Results:', sma.result.outReal.slice(-5));
console.log('RSI Results:', rsi.result.outReal.slice(-5));
