// server.js

const http = require('http');
const path = require('path');
const router = require('./src/Routes/router');
const initDB = require('./src/Database/database');

const { Pool } = require('pg');

const pool = new Pool({
    user: 'web',
    host: 'localhost',
    database: 'tw',
    password: 'web',
    port: 5432,
    max: 20,
});

initDB(pool).then(result => {
    if(result){
        console.log("Database initialised successfully!");
    }
});

const PORT = process.env.PORT || 3000;

const server = http.createServer((req, res) => {
    router(req, res);
});

server.listen(PORT, '0.0.0.0', () => {
    console.log(`Server is running on port ${PORT}`);
});
