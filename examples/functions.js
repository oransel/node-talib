var talib = require("../build/Release/talib");
var functions = talib.functions;

// Display module version
console.log();
console.log("TALib Version: " + talib.version);

// Display all indicator functions
console.log(functions);

// Display total indicator function count
console.log();
console.log("Total Functions: " + functions.length);

