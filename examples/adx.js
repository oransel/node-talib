var talib = require("../build/Release/talib");
var fs = require("fs");

console.log();
console.log("TALib Version: " + talib.version);

var marketContents = fs.readFileSync('examples/marketdata.json','utf8'); 
var marketData = JSON.parse(marketContents);

talib.execute({
    name: "ADX",
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    high: marketData.high,
    low: marketData.low,
    close: marketData.close,
    optInTimePeriod: 9
}, function (result) {
    console.log();
    console.log("ADX Function:");
    console.log(result);
});

