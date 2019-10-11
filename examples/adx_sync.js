const talib = require("../build/Release/talib");
const fs = require("fs");

// Display module version
console.log();
console.log("TALib Version: " + talib.version);

// Load market data
const marketContents = fs.readFileSync('examples/marketdata.json','utf8'); 
const marketData = JSON.parse(marketContents);

// execute ADX indicator function with time period 9
const result = talib.execute({
    name: "ADX",
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    high: marketData.high,
    low: marketData.low,
    close: marketData.close,
    optInTimePeriod: 9
});

// show results
console.log("ADX Function Results:");
console.log(result);
