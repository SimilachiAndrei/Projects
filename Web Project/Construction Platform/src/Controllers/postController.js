const utils = require('../utils/utils');
const projectModel = require('../Models/projectModel');

async function addPost(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const postData = Object.fromEntries(formData.entries());

        try {
            const post = await projectModel.addPost(postData, user.id); // Ensure user.id is passed as clientId
            res.writeHead(302, { 'Location': '/myacc' });
            res.end()
        } catch (error) {
            console.error('Error in postController.addPost:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

module.exports = { addPost };