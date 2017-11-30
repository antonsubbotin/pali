const pali = require('./build/Release/pali');

console.time("pali");
console.log('The returned value is: ', pali.get(6));
console.timeEnd("pali");