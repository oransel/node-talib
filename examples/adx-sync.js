/**
 * Modern ADX Example (Synchronous)
 */

const talib = require('../index.js');
const fs = require('fs');

// Display module version
console.log('\nTALib Version:', talib.version);

// Load market data
const marketContents = fs.readFileSync('examples/marketdata.json', 'utf8');
const marketData = JSON.parse(marketContents);

// Execute ADX indicator function synchronously
const result = talib.execute({
  name: 'ADX',
  startIdx: 0,
  endIdx: marketData.close.length - 1,
  high: marketData.high,
  low: marketData.low,
  close: marketData.close,
  optInTimePeriod: 9
});

// Show the result
console.log('\nADX Function Results:');
console.log(result);
