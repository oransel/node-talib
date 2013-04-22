var talib = require("../build/Release/talib");
var functions = talib.functions;

// Display module version
console.log();
console.log("TALib Version: " + talib.version);

// Display ADX indicator function specifications
console.log(talib.explain("ADX"));
