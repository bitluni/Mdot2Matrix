const fs = require('fs');
const https = require('https');
const WebSocket = require('ws');

const server = https.createServer({
  cert: fs.readFileSync('/etc/letsencrypt/live/bitluni.net/fullchain.pem'),
  key: fs.readFileSync('/etc/letsencrypt/live/bitluni.net/privkey.pem')
});

const wss = new WebSocket.Server({ server });
const clients = new Set();

const MESSAGE_COOLDOWN_MS = 30;
const MAX_MESSAGE_LENGTH = 241;
const VALID_CHAR_REGEX = /^[0-9a-fA-Fs]+$/;

console.log('Secure WebSocket server starting on wss://localhost:8443');

wss.on('connection', (ws) => {
  console.log('Client connected');
  clients.add(ws);
  ws.lastMessageTime = 0;

  ws.on('message', (message) => {
    const now = Date.now();

    if (now - ws.lastMessageTime < MESSAGE_COOLDOWN_MS) return;

    const text = message.toString();

    if (
      text.length <= MAX_MESSAGE_LENGTH &&
      VALID_CHAR_REGEX.test(text)
    ) {
      ws.lastMessageTime = now;

      for (const client of clients) {
        if (client.readyState === WebSocket.OPEN) {
          client.send(text);
        }
      }
    }
  });

  ws.on('close', () => {
    clients.delete(ws);
    console.log('Client disconnected');
  });
});

server.listen(8443, () => {
  console.log('Server is listening on port 8443');
});
