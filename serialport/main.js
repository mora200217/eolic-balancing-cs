const SerialPort = require('serialport').SerialPort; 
const { DelimiterParser } = require('serialport');

const WebSocketServer = require('ws').WebSocketServer; 

const wss = new WebSocketServer({ port: 8000 });



const puerto = new SerialPort({
    path: '/dev/cu.usbserial-14110',
    baudRate:115200,
    // parser: new Readline("\n")
});

const parser = puerto.pipe(new DelimiterParser({delimiter: '\n'})); 

parser.on('open', () => {
    console.log('Conexion abierta'); 
})


wss.on('connection', function connection(ws) {


   
parser.on('data', (data) => {
    let enc = new TextDecoder(); 
    let arr = new Uint8Array(data); 
    ready = enc.decode(arr); 
    // console.log(ready)
    ws.send(ready);
    })

    ws.on('error', console.error);
  
    ws.on('message', function message(data) {
      console.log('received: %s', data);
    });
    
    ws.on('open', () => {
      console.log("Abierto")
    })
  
  
  

  });
  
  

