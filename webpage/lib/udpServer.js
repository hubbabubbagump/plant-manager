"use strict";

var dgram = require('dgram');
const PORT = 12345;
const HOST = '192.168.7.2';

const updateSocket = 'updateSetting';
const waterSocket = 'waterPlant';
const refreshSocket = 'refreshData';
const updateCmd = 'update';

exports.listen = function(server) {
    var io = require('socket.io').listen(server);
    io.on('connection', function(socket) {
        updateHandler(socket);
        commandHandler(socket);
    });
}

function updateHandler(socket) {
    sendRequest(socket, updateSocket, updateCmd, true);
    const second = 60*1000;
    setInterval(function() {
        sendRequest(socket, updateSocket, updateCmd, true);
    }, second);
}

function commandHandler(socket) {
    socket.on(updateSocket, function(data) {
        sendRequest(socket, updateSocket, data, false);
    });
    socket.on(waterSocket, function(data) {
        sendRequest(socket, updateSocket, data, false);
    });
    socket.on(refreshSocket, function(data) {
        sendRequest(socket, updateSocket, data, false);
    });
}

function sendRequest(socket, socketType, data, addToGraph) {
    var buffer;
    if (data.light != null) {
        buffer = new Buffer(socketType + ' ' + data.light + ' ' 
            + data.moistUpper + ' ' + data.moistLower + ' ' 
            + data.tempUpper + ' ' + data.tempLower + ' '
            + data.humidUpper + ' ' + data.humidLower + '\n');
    } else {
        buffer = new Buffer(data + '\n');
    }
    var client = dgram.createSocket('udp4');
    
    client.send(buffer, 0, buffer.length, PORT, HOST, function(err) {
        if (err) {
            socket.emit(errorSocket, err);
        }
    });

    client.on('message', function(message) {
        var reply = message.toString('utf8');
        var data = {
            plantData: reply,
            addToGraph: addToGraph
        }
        console.log(data);
        socket.emit(socketType, data);
        client.close();
    });
}