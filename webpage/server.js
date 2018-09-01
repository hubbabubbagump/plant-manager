"use strict";

var express = require('express');
var path = require('path');
var http = require('http');
var udpServer = require('./lib/udpServer');
var app = module.exports.app = express();

var server = http.createServer(app);
const PORT = 8088;

app.use(express.static('public'));

app.get('/', function(req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});

server.listen(PORT, function() {
    console.log('Listening on Port ' + PORT);
})

udpServer.listen(server);