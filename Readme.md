# node-talib

A thin [node.js](http://nodejs.org) wrapper around [TA-LIB](http://ta-lib.org/), a technical analysis library with 100+ [indicators](http://ta-lib.org/function.html) such as ADX, MACD, RSI, Stochastic, Bollinger Bands, TRIX and candlestick pattern recognition.

## Prerequisites

* [Node.js](http://nodejs.org/)
* [Windows Build Tools](https://www.npmjs.com/package/windows-build-tools) (Windows Only)

If you are using __Windows__, you will need to install [Windows Build Tools](https://www.npmjs.com/package/windows-build-tools), which downloads and installs Visual C++ Build Tools 2015 and Python 2.7, configuring your machine and npm appropriately.

## Installation

To install the most recent release from npm, run:

    npm install talib

## Building

The source code is available at [github](http://github.com/oransel/node-talib). You can either clone the repository or download a zip file of the latest release.

Once you have the source, you can build the module by running

	npm install

in the main directory. If everything goes well, the module will be available in the build/Release folder.

## Examples

TALib is very simple to use.

``` js
// load the module and display its version
var talib = require('./build/Release/talib');
console.log("TALib Version: " + talib.version);

// Display all available indicator function names
var functions = talib.functions;
for (i in functions) {
	console.log(functions[i].name);
}
```

Assuming the market data is readily available, you can calculate an indicator by calling the `execute` function with the name of the indicator and required input parameters.

``` js
// market data as arrays
var marketData = { open: [...], close: [...], high: [...], low: [...], volume: [...] };

// execute Average Directional Movement Index indicator with time period 9
talib.execute({
    name: "ADX",
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    high: marketData.high,
    low: marketData.low,
    close: marketData.close,
    optInTimePeriod: 9
}, function (err, result) {

    console.log("ADX Function Results:");
    console.log(result);

});
```

Input parameters can be discovered by:

``` js
// Retreive Average Directional Movement Index indicator specifications
var function_desc = talib.explain("ADX");
console.dir(function_desc);


{
  // Function Name
  name: 'ADX',

  // Function Group Name
  group: 'Momentum Indicators',

  // Function Description
  hint: 'Average Directional Movement Index',

  // Input Parameters
  inputs:
   [ {
       // Parameter Name
       name: 'inPriceHLC',

       // Parameter Type
       //   price, real, or integer
       type: 'price',

       // Parameter keys to be passed in when calling the function
       //   open, high, low, close, volume,
       //   openinterest, or timestamp
       flags: [ 'high', 'low', 'close' ] } ],

  // Optional Input Parameters
  optInputs:
   [ {
       // Parameter Name
       name: 'optInTimePeriod',

       // Parameter Display Label
       displayName: 'Time Period',

       // Parameter Default Value
       defaultValue: 14,

       // Parameter Description
       hint: 'Number of period',

       // Parameter Type
       //   real_range, real_integer,
       //   integer_range, or integer_list
       type: 'range_integer' } ],

  // Output Values
  outputs:
   [ {
       // Value Name
       name: 'outReal',

       // Value Type
       //   real or integer
       type: 'real',

       // Suggested Value Visualization Hint
       //   line, line_dot, line_dash, dot,
       //   histogram, pattern_bool, pattern_bull_bear,
       //   pattern_strength, positive, negative, zero,
       //   limit_upper, or limit_lower
       flags: [ 'line' ] } ] }
```

Some indicators require or accept a `optInMAType` flag:

```
SMA   = 0
EMA   = 1
WMA   = 2
DEMA  = 3
TEMA  = 4
TRIMA = 5
KAMA  = 6
MAMA  = 7
T3    = 8
```

For working examples look in the `examples/` directory. You can execute the examples using node.

	node examples/adx.js

## License

Copyright (c) 2012-2018 Mustafa Oransel

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
