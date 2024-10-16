const net = require('net');
const pipeName = '\\\\.\\pipe\\CadworkPipe';

console.log('Connecting to the server...');

const client = net.createConnection(pipeName, () => {
    console.log('Connected to the server.');

    const jsonObj = { message: 'Hello from Node.js client!' };
    const jsonString = JSON.stringify(jsonObj);
    client.write(jsonString);
    console.log(`Sent to server: ${jsonString}`);
});

client.on('data', (data) => {
    console.log(`Received from server: ${data.toString()}`);
    client.end();
});

client.on('error', (err) => {
    console.error(`Connection error: ${err}`);
});

client.on('end', () => {
    console.log('Disconnected from the server.');
});