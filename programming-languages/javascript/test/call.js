const m = require('./hello')
console.log(m);
local = 10; // global variable since without prefix var or let.
var local = 15; // because this function block variable is defined outside function, so it is same as a global variable, and it can override the above global variable.
if(true){
    var local = 20; // it is the above function block variable.
    t = 100;
}
console.log(local);
console.log(t);
console.log(__filename);
console.log(global.process.mainModule)
console.log(module)