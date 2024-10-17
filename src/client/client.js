const net = require('net');
const pipeNames = ['\\\\.\\pipe\\test_endtype.3d', '\\\\.\\pipe\\WALL_30.3d'];

pipeNames.forEach((pipeName) => {
    console.log(`Connecting to the server on pipe ${pipeName}...`);

    setTimeout(() => {
        const client = net.createConnection(pipeName, () => {
            console.log(`Connected to the server on pipe ${pipeName}.`);

            const jsonObj = {message: `Hello from Node.js named pipe client ${pipeName}!`};
            const jsonString = JSON.stringify(jsonObj);
            client.write(jsonString);
            console.log(`Sent to server on pipe ${pipeName}: ${jsonString}`);
        });

        client.on('data', (data) => {
            const jsonifiedData = JSON.parse(data.toString());
            console.log(`Received from server on pipe ${pipeName}: ${JSON.stringify(jsonifiedData, null, 2)}`);
            client.end();
        });

        client.on('error', (err) => {
            console.error(`Connection error on pipe ${pipeName}: ${err}`);
        });

        client.on('end', () => {
            console.log(`Disconnected from the server on pipe ${pipeName}.`);
        });
    }, 1000); // 1-second delay
});