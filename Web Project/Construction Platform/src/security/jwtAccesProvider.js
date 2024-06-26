const jwt = require("jsonwebtoken");
const sendMessage = require('../utils/sendMessage');
const {serialize} = require('cookie');

JWT_SECRETKEY = '12345te42456g9buhbyutrdryeurdcyr67766hren4444425tggrb334556vf';
const handleSecurity = (res, user) => {

    if (user === null) {
        sendMessage(res, {statusCode: 401, status: 'Unauthorized', message: 'Invalid data.'});
    } else {
        let token = generateToken(user);
        setCookie(res, token);
    }
}

const generateToken = (user) => {
    const payload = {
        username: user.username,
        role: user.role,
        email: user.email
    };

    return jwt.sign(payload, JWT_SECRETKEY, {expiresIn: '2h'});
}


const setCookie = (res, token) => {
    const options = {
        path: '/',
        httpOnly: true
    };
    res.setHeader('Set-Cookie', serialize('token', token, options));
}

const deleteCookie = (res) => {
    const options = {
        path: '/',
        httpOnly: true,
        expires: new Date(0)
    };
    res.setHeader('Set-Cookie', serialize('token', '', options));
}
const getCookie = (cookieName, cookieString) => {
    let cookies = cookieString.split('; ');

    for(let cookie of cookies) {
        let [name, value] = cookie.split('=');
        if (name === cookieName) {
            return value;
        }
    }
    return null;
}
const decodedJWT = (req) => {
    const cookieString = req.headers.cookie;

    if (!cookieString) return null;

    let token = getCookie('token', cookieString);

    if (!token) return null;

    return jwt.verify(token, JWT_SECRETKEY);
}

const verifyJWTRole = (res, req, role) => {
    const decoded = decodedJWT(req);
    if (!decoded) {
        sendMessage(res, {statusCode: 401, status: 'Unauthorized', message: 'No token provided, you need to login'});
        return false;
    }

    if (decoded['role'] !== role) {
        sendMessage(res, {
            statusCode: 401,
            status: 'Unauthorized',
            message: 'You do not have permission to access this resource.'
        });
        return false;
    }
    return true;
}

module.exports = {handleSecurity, deleteCookie, verifyJWTRole, decodedJWT, generateToken};