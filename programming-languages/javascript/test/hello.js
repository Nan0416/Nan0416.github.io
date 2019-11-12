function f(){
    this.x = 10;
}
var f = f();
console.log(this); // {}
console.log(global.x) // undefined.

function what(x){
    console.log(x === undefined);
    console.log(x === null);
}
what(null);

const a = new Date('2019-11-12T22:08:15.454Z');
console.log(a.getHours()); // current hour in local time zone.
console.log(a.getUTCHours()); // current hour in UTC, London.
console.log(a.getTimezoneOffset()); // 300, time difference in minutes, e.g. 300 is the different between UTC and EST.
console.log(a.getTime()) ; //1573596286789, Returns the number of milliseconds since midnight Jan 1 1970, and a specified date
console.log(a.toISOString()); // 2019-11-12T22:08:15.454Z, standard format
console.log(a.toDateString()); // Tue Nov 12 2019
console.log(a.toUTCString()); // Tue, 12 Nov 2019 22:09:38 GMT

console.log('---------')
const err = new Error("What's the error?")
console.log(err.message); // What's the error?
console.log(err.name); // Error

console.log('-----')
let obj = {"first-name": "nan", age:10}
console.log(JSON.stringify(obj, null, 4)); // pretty stringify.
