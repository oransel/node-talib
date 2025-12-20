# node-talib

A modern [Node.js](https://nodejs.org) wrapper around [TA-LIB](http://ta-lib.org/), providing 100+ [technical analysis indicators](http://ta-lib.org/function.html) including ADX, MACD, RSI, Stochastic, Bollinger Bands, TRIX, and candlestick pattern recognition.

## Features

- **100+ Technical Indicators** - Comprehensive indicator library
- **Modern JavaScript** - ES6+, async/await, Promises
- **TypeScript Support** - Full type definitions included
- **Dual API** - Both synchronous and asynchronous execution
- **ESM & CommonJS** - Works with both module systems
- **High Performance** - Native C++ bindings via N-API
- **Cross-Platform** - Works on macOS, Linux, and Windows
- **No Dependencies** - No external dependencies required
- **Lightweight** - No unnecessary dependencies, small footprint

## Prerequisites

- **Node.js** >= 18.0.0
- **Python** (for node-gyp)
- **C++ Build Tools**
  - **macOS**: Xcode Command Line Tools (`xcode-select --install`)
  - **Linux**: `build-essential` package
  - **Windows**: [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/)

## Installation

```bash
npm install talib
```

## Quick Start

### CommonJS

```javascript
const talib = require('talib');

// Synchronous execution
const result = talib.execute({
  name: 'SMA',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 20
});

console.log(result.result.outReal);
```

### ES Modules

```javascript
import talib from 'talib';

const result = talib.execute({
  name: 'RSI',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 14
});
```

### TypeScript

```typescript
import * as talib from 'talib';

const result: talib.ExecuteResult = talib.execute({
  name: 'MACD',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInFastPeriod: 12,
  optInSlowPeriod: 26,
  optInSignalPeriod: 9
});
```

## Building from Source

```bash
git clone https://github.com/oransel/node-talib.git
cd node-talib
npm install
```

The module will be built automatically during installation.

## API Reference

### Module Properties

```javascript
talib.version           // TA-LIB version string
talib.functions         // Array of all available functions
talib.functionUnstIds   // Function unstable period IDs
```

### Methods

#### `talib.execute(params[, callback])`

Execute a technical analysis function.

**Synchronous:**
```javascript
const result = talib.execute({
  name: 'SMA',
  startIdx: 0,
  endIdx: data.length - 1,
  inReal: data,
  optInTimePeriod: 20
});
```

**Asynchronous:**
```javascript
talib.execute({
  name: 'SMA',
  startIdx: 0,
  endIdx: data.length - 1,
  inReal: data,
  optInTimePeriod: 20
}, (err, result) => {
  if (err) return console.error(err);
  console.log(result);
});
```

**With Async/Await:**
```javascript
const result = await new Promise((resolve, reject) => {
  talib.execute(params, (err, result) => {
    if (err) reject(err);
    else resolve(result);
  });
});
```

#### `talib.explain(functionName)`

Get detailed information about a function's parameters.

```javascript
const info = talib.explain('ADX');
console.log(info);
// {
//   name: 'ADX',
//   group: 'Momentum Indicators',
//   hint: 'Average Directional Movement Index',
//   inputs: [...],
//   optInputs: [...],
//   outputs: [...]
// }
```

#### `talib.setUnstablePeriod(functionId, period)`

Set the unstable period for a function.

```javascript
talib.setUnstablePeriod(talib.functionUnstIds.TA_FUNC_UNST_EMA, 30);
```

## Common Indicators

### Moving Averages

```javascript
// Simple Moving Average
const sma = talib.execute({
  name: 'SMA',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 20
});

// Exponential Moving Average
const ema = talib.execute({
  name: 'EMA',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 12
});
```

### Momentum Indicators

```javascript
// Relative Strength Index
const rsi = talib.execute({
  name: 'RSI',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 14
});

// MACD
const macd = talib.execute({
  name: 'MACD',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInFastPeriod: 12,
  optInSlowPeriod: 26,
  optInSignalPeriod: 9
});
```

### Volatility Indicators

```javascript
// Bollinger Bands
const bbands = talib.execute({
  name: 'BBANDS',
  startIdx: 0,
  endIdx: prices.length - 1,
  inReal: prices,
  optInTimePeriod: 20,
  optInNbDevUp: 2,
  optInNbDevDn: 2,
  optInMAType: 0 // SMA
});

// Average True Range
const atr = talib.execute({
  name: 'ATR',
  startIdx: 0,
  endIdx: prices.length - 1,
  high: highs,
  low: lows,
  close: closes,
  optInTimePeriod: 14
});
```

### Moving Average Types

When an indicator accepts `optInMAType`:

```javascript
const MAType = {
  SMA: 0,    // Simple Moving Average
  EMA: 1,    // Exponential Moving Average
  WMA: 2,    // Weighted Moving Average
  DEMA: 3,   // Double Exponential Moving Average
  TEMA: 4,   // Triple Exponential Moving Average
  TRIMA: 5,  // Triangular Moving Average
  KAMA: 6,   // Kaufman Adaptive Moving Average
  MAMA: 7,   // MESA Adaptive Moving Average
  T3: 8      // Triple Exponential Moving Average (T3)
};
```

## Examples

Check the `examples/` directory for more examples:

```bash
# Run basic example
node examples/adx.js

# Run modern async example
node examples/adx-modern.js

# Run synchronous example
node examples/adx-sync.js

# Run ES module example
node examples/esm-example.mjs

# Run multiple indicators example
node examples/multiple-indicators.js
```

## Testing

```bash
npm test
```

## Troubleshooting

### Build Errors

If you encounter build errors:

```bash
# Clean and rebuild
npm run clean
npm install

# Or use rebuild script
npm run rebuild
```

### Python Not Found

Ensure Python is installed and in your PATH:

```bash
# Check Python installation
python --version
# or
python3 --version
```

### Windows Build Issues

Install Visual Studio Build Tools:
- Download from [Visual Studio Downloads](https://visualstudio.microsoft.com/downloads/)
- Select "Desktop development with C++"

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

If this project helped you, consider supporting its development:

- [PayPal](https://www.paypal.com/ncp/payment/HBJX43G329P86)
- BTC: 18gT1wmq3RMoLBm2ZFv4PhiYbU5CMAQC6P

## Links

- [TA-LIB Official Website](http://ta-lib.org/)
- [TA-LIB Function List](http://ta-lib.org/function.html)
- [GitHub Repository](https://github.com/oransel/node-talib)

## License

Copyright (c) 2012-2026 Mustafa Oransel

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
