/**
 * Modern ADX Example (Async/Await)
 */

const talib = require('../index.js');
const { readFile } = require('fs/promises');

async function calculateADX() {
  try {
    // Display module version
    console.log('\nTALib Version:', talib.version);

    // Load market data
    const marketContents = await readFile('examples/marketdata.json', 'utf8');
    const marketData = JSON.parse(marketContents);

    // Execute ADX indicator function with time period 9 (using Promise wrapper)
    const result = await new Promise((resolve, reject) => {
      talib.execute(
        {
          name: 'ADX',
          startIdx: 0,
          endIdx: marketData.close.length - 1,
          high: marketData.high,
          low: marketData.low,
          close: marketData.close,
          optInTimePeriod: 9
        },
        (err, result) => {
          if (err) reject(err);
          else resolve(result);
        }
      );
    });

    // Show the result
    console.log('\nADX Function Results:');
    console.log(result);
  } catch (error) {
    console.error('Error:', error);
    process.exit(1);
  }
}

// Run the example
calculateADX();
