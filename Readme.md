#node-talib

A thin [node.js](http://nodejs.org) wrapper around [TA-LIB](http://ta-lib.org/), a technical analysis library with 100+ [indicators](http://ta-lib.org/function.html) such as ADX, MACD, RSI, Stochastic, Bollinger Bands, TRIX and candlestick pattern recognition.

## Installation

To install the most recent release from npm, run:

    npm install talib

## Building

The source code is available at [github](http://github.com/oransel/node-talib). You can either clone the repository or download a zip file of the latest release.

Once you have the source, you can build the module by running

	make all
	
in the main directory. If everything goes well, the module will be available in the build/Release folder.

## Examples

TALib is very simple to use.

``` js
// load the module and display its version
var talib = require('./build/Release/talib');
console.log("TALib Version: " + talib.version);

// display all available indicator function names
var functions = talib.functions;
for (i in functions) {
	console.log(functions[i].name);
}

// retreive Average Directional Movement Index indicator specifications
var function_desc = talib.explain("ADX");
console.log(function_desc.name);
console.log(function_desc.group);
console.log(function_desc.hint);
console.log(function_desc.inputs);
console.log(function_desc.optInputs);
console.log(function_desc.outputs);
```

Assuming the market data is readily available, you can calculate an indicator by calling the `execute` function with the name of the indicator and required input parameters.

``` js
// market data as arrays
var marketdata = { open: [...], close: [...], high: [...], low: [...], volume: [...] };

// execute Average Directional Movement Index indicator function with time period 9
talib.execute({
    name: "ADX",
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    high: marketData.high,
    low: marketData.low,
    close: marketData.close,
    optInTimePeriod: 9
}, function (result) {
	
    console.log("ADX Function Results:");
    console.log(result);

});
```

For working examples look in the `examples/` directory. You can execute the examples using node.

	node examples/adx.js

## TODO

* Add async processing

## License

Copyright (c) 2012 Mustafa Oransel

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
