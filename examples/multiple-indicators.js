/**
 * Example: Calculate Multiple Indicators
 */

const talib = require('../index.js');
const fs = require('fs');

console.log('\nTALib Version:', talib.version);

// Load market data
const marketData = JSON.parse(fs.readFileSync('examples/marketdata.json', 'utf8'));

// Calculate multiple indicators synchronously
const indicators = {
  // Simple Moving Average
  sma: talib.execute({
    name: 'SMA',
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    inReal: marketData.close,
    optInTimePeriod: 20
  }),

  // Relative Strength Index
  rsi: talib.execute({
    name: 'RSI',
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    inReal: marketData.close,
    optInTimePeriod: 14
  }),

  // Moving Average Convergence Divergence
  macd: talib.execute({
    name: 'MACD',
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    inReal: marketData.close,
    optInFastPeriod: 12,
    optInSlowPeriod: 26,
    optInSignalPeriod: 9
  }),

  // Bollinger Bands
  bbands: talib.execute({
    name: 'BBANDS',
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    inReal: marketData.close,
    optInTimePeriod: 20,
    optInNbDevUp: 2,
    optInNbDevDn: 2,
    optInMAType: 0 // SMA
  })
};

console.log('\nCalculated Indicators:');
console.log('SMA (20):', indicators.sma.result.outReal.slice(-5));
console.log('RSI (14):', indicators.rsi.result.outReal.slice(-5));
console.log('MACD:', {
  macd: indicators.macd.result.outMACD.slice(-5),
  signal: indicators.macd.result.outMACDSignal.slice(-5),
  histogram: indicators.macd.result.outMACDHist.slice(-5)
});
console.log('Bollinger Bands:', {
  upper: indicators.bbands.result.outRealUpperBand.slice(-5),
  middle: indicators.bbands.result.outRealMiddleBand.slice(-5),
  lower: indicators.bbands.result.outRealLowerBand.slice(-5)
});
