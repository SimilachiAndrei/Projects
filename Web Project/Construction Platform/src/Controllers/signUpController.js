const userModel = require('../Models/userModel');

async function signup(req, res) {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const username = formData.get('username');
        const password = formData.get('password');
        const userType = formData.get('type');
        const email = formData.get('email');

        if (!username || !password || !userType || !email) {
            res.writeHead(400, { 'Content-Type': 'text/plain' });
            res.end('Missing required fields');
            return;
        }

        const userExists = await userModel.getUser(username);
        if (userExists) {
            res.writeHead(400, { 'Content-Type': 'text/plain' });
            res.end('Username already exists');
            return;
        }

        const userData = {
            username,
            email,
            password,
            role: userType 
        };

        if (userType === 'company') {
            const companyName = formData.get('companyName');
            const companyAddress = formData.get('companyAddress');
            const companyPhone = formData.get('companyPhone');
            const companyProfile = formData.get('companyProfile');

            if (!companyName || !companyAddress || !companyPhone || !companyProfile) {
                res.writeHead(400, { 'Content-Type': 'text/plain' });
                res.end('Missing required company fields');
                return;
            }

            userData.companyName = companyName;
            userData.companyAddress = companyAddress;
            userData.companyPhone = companyPhone;
            userData.companyProfile = companyProfile;
        }

        await userModel.addUser(userData);
        res.writeHead(302, { 'Location': '/login' });
        res.end();
    });
}

module.exports = { signup }