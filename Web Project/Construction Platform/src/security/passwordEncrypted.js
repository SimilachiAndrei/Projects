const bcrypt = require('bcryptjs');
const sendMessage = require('../utils/sendMessage');

number = 12;

async function encryptedPassword(password) {
    return await bcrypt.hash(password, number);
}

async function matchPassword(password, databasePassword, res) {
    if (!await bcrypt.compare(password, databasePassword)) {
        sendMessage(res, {statusCode: 400, status: 'Bad Request', message: 'Incorrect password'});
        return false;
    }
    return true;
}

module.exports = {encryptedPassword, matchPassword};