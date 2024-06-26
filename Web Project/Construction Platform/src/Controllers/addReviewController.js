const utils = require('../utils/utils');
const reviewModel = require('../Models/reviewModel');
const phaseModel = require('../Models/phaseModel');



async function submitReview(req, res) {
    // Check if the user is authenticated and is a company
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', async () => {
            try {
                const formData = new URLSearchParams(body);
                const data = Object.fromEntries(formData.entries());

                const result = await reviewModel.submitReview(data, user.id);

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ message: 'Review added successfully' }));
            } catch (error) {
                console.error('Error adding review:', error);
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: error.message }));
            }
        });
    } catch (error) {
        console.error('Error in submitReview controller:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

async function getFinishedProjects(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const projects = await phaseModel.getFinishedProjects(user.id);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(projects));
    } catch (error) {
        console.error('Error in companiesController.getFinishedProjects:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

module.exports = {submitReview, getFinishedProjects}