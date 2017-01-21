var talib = require("../build/Release/talib");
var fs = require("fs");

// Display module version
console.log();
console.log("TALib Version: " + talib.version);

// Load market data
var marketContents = fs.readFileSync('examples/marketdata.json','utf8'); 
var marketData = JSON.parse(marketContents);

// execute CDLHAMMER indicator function
talib.execute({
    name: "CDLHAMMER",
    startIdx: 0,
    endIdx: marketData.close.length - 1,
    open: marketData.open,
    high: marketData.high,
    low: marketData.low,
    close: marketData.close
}, function (err, result) {
	
	// Show the result array
    console.log("CDLHAMMER Function Results:");
    console.log(result);

});
