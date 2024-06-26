const GETrouter = require('./GETrouter');
const POSTrouter = require('./POSTrouter');
const PUTrouter = require('./PUTrouter');
const DELETErouter = require('./DELETErouter');


function handleRequest(req, res) {
    if (req.method === 'GET') {
        GETrouter.handleRequest(req, res);
    } else if (req.method === 'POST') {
        POSTrouter.handleRequest(req, res);
    } else if (req.method === 'PUT') {
        PUTrouter.handleRequest(req, res);
    }
    else if (req.method === 'DELETE') {
        DELETErouter.handleRequest(req, res);
    }
    else {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Method Not Allowed');
    }
}

module.exports = handleRequest;
