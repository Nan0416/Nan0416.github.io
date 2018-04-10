const http = require('http');
const express = require('express');
const morgan = require('morgan');
const bodyparser = require('body-parser');
const hostname = 'localhost';
const port = 3000;

var app = express();
app.use(morgan('dev'));
app.use(bodyparser.json());
app.use(express.static(__dirname + '/public')); // serve the static page

// handle ajax request

app.get('/:path', (req, res) =>{
    
    if (req.params.path == 'ajax_get_1') {
        res.statusCode = 200;
        res.setHeader('Content-Type','text/plain');
        res.end('This is an ajax get response!');
    } else {
        res.statusCode = 200;
        res.setHeader('Content-Type','text/plain');
        res.end('Not found ' + req.url);
    }
});
app.post('/:path', (req, res) =>{
    
    if (req.params.path == 'ajax_post_1') {
        res.statusCode = 200;
        console.log(req.body.name);
        res.setHeader('Content-Type','text/plain');
        res.end(req.body.name);
    } else {
        res.statusCode = 200;
        console.log(req.body.name);
        res.setHeader('Content-Type','text/plain');
        res.end("error");
    }
});
const server = http.createServer(app);
server.listen(port, hostname, ()=>{
    console.log(`http://${hostname}:${port}`);
});