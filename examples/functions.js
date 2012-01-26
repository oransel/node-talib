var talib = require("../build/Release/talib");
var functions = talib.functions;

console.log();
console.log("TALib Version: " + talib.version);

console.log(functions);

console.log();
console.log("Total Functions: " + functions.length);

