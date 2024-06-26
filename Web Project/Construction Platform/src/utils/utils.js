const { parse } = require('cookie');
const auth = require('../utils/auth');

function isAuthenticated(req) {
    const cookies = parse(req.headers.cookie || '');
    const token = cookies.token;
    const user = auth.verifyToken(token);
    return user ? user : null;
}

function isUserType(req, type) {
    const cookies = parse(req.headers.cookie || '');
    const token = cookies.token;
    const user = auth.verifyToken(token);
    if (user.role === type) return true;
    return false;
}

function getUserType(req, type) {
    const cookies = parse(req.headers.cookie || '');
    const token = cookies.token;
    const user = auth.verifyToken(token);
    return user.role;
}

module.exports = {isAuthenticated, isUserType, getUserType}