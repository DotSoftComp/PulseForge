const WebSocket = require('ws');
const axios = require('axios');

// ----------------- CONFIG -----------------
const SERVER_HTTP = 'http://localhost:3000';  // Serveur Express
const WS_PORT = 4000;                          // Port pour le moteur C++
console.log('Starting client.js...');

// ----------------- WebSocket pour moteur C++ -----------------
const wsServer = new WebSocket.Server({ port: WS_PORT });

wsServer.on('connection', (socket) => {
  console.log('✅ Engine connected via WebSocket');

  socket.on('message', async (data) => {
    console.log('Received from engine:', data.toString());
    let msg;
    try { msg = JSON.parse(data); } 
    catch { msg = { action: 'unknown', payload: data.toString() }; }

    let response;

    try {
      if (msg.method === 'GET') {
        const res = await axios.get(`${SERVER_HTTP}${msg.path || '/'}`);
        response = res.data;
      } else if (msg.method === 'POST') {
        const res = await axios.post(`${SERVER_HTTP}${msg.path || '/'}`, msg.payload || {});
        response = res.data;
      } else {
        response = { error: 'Unknown method' };
      }
    } catch (err) {
      response = { error: err.message };
    }

    socket.send(JSON.stringify({ action: msg.action || 'response', data: response }));
  });
});

console.log(`WebSocket listening for C++ engine on ws://localhost:${WS_PORT}`);

// ----------------- Interface ligne de commande pour tester -----------------
if (require.main === module) {
  const readline = require('readline');
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });

  rl.on('line', async (line) => {
    const [method, path, payload] = line.split(' ');
    try {
      let res;
      if (method.toUpperCase() === 'GET') {
        res = await axios.get(`${SERVER_HTTP}${path}`);
      } else if (method.toUpperCase() === 'POST') {
        res = await axios.post(`${SERVER_HTTP}${path}`, payload ? JSON.parse(payload) : {});
      }
      console.log('Server response:', res.data);
    } catch (e) {
      console.error('Error:', e.message);
    }
  });
}
