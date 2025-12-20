/**
 * ES Module Example
 */

import talib from '../index.mjs';
import { readFile } from 'fs/promises';

async function main() {
  console.log('\nTALib Version:', talib.version);

  // Load market data
  const marketContents = await readFile('examples/marketdata.json', 'utf8');
  const marketData = JSON.parse(marketContents);

  // Calculate RSI synchronously
  const rsi = talib.execute({
    name: 'RSI',
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    inReal: marketData.close,
    optInTimePeriod: 14
  });

  console.log('\nRSI Results:');
  console.log('Last 10 values:', rsi.result.outReal.slice(-10));
}

main().catch(console.error);
