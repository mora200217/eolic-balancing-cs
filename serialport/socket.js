const WebSocketServer = require('ws').WebSocketServer; 

const wss = new WebSocketServer({ port: 8000 });



wss.on('connection', function connection(ws) {
  ws.on('error', console.error);

  ws.on('message', function message(data) {
    console.log('received: %s', data);
  });
  
  ws.on('open', () => {
    console.log("Abierto")
  })



  ws.send('something');
});