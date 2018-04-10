// receiver and concrete command, no command
function execute3000() {
    var http = require('http');
    http.createServer((req, res)=>{
        res.statusCode = 200;
        res.end('OK:3000');
    }).listen(3000);
}
function execute3001() {
    var http = require('http');
    http.createServer((req, res)=>{
        var cliHttp = require('http');
        cliHttp.get('http://nodejs.org/dist/index.json', (res1)=>{
            let rawData = '';
            res1.on('data', (chunk)=>{
                rawData += chunk;
            });
            res1.on('end', ()=>{
                const parsedData = JSON.parse(rawData);
                res.statusCode = 200;
                res.setHeader('Content-Type', 'application/json');
                res.end(JSON.stringify(parsedData));
            })
        });
       
    }).listen(3001);
}
// application binding
const EventEmitter = require('events');
const event = new EventEmitter();
event.on('server3000', execute3000);
event.on('server3001', execute3001);
// invoker
event.emit('server3000');
event.emit('server3001');

