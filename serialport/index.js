const SerialPort = require('serialport').SerialPort; 
const { read } = require('fs');
// const { read } = require('fs');
const { parse } = require('path');
const { DelimiterParser } = require('serialport');
cpnst { WebSocket } = require('web')


WebSocket

let socket = new WebSocket("wss://tdc-values");


const puerto = new SerialPort({
    path: '/dev/cu.usbserial-14110',
    baudRate:115200,
    // parser: new Readline("\n")
  });

const parser = puerto.pipe(new DelimiterParser({delimiter: '\n'})); 

parser.on('open', () => {
    console.log('Conexion abierta'); 
})


parser.on('data', (data) => {
    let enc = new TextDecoder(); 
    let arr = new Uint8Array(data); 
    ready = enc.decode(arr); 
    console.log(ready); 
})
