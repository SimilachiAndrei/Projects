const utils = require('../utils/utils');
const companyModel = require('../Models/companyModel');

async function getPortofolioDetails(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        // Split the path by '/' and get the last part
        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const company = await companyModel.getPortofolioDetails(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(company));
    } catch (error) {
        console.error('Error in getCompanyDetails:', error);
    }
}

async function getPortofolioPhases(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        // Split the path by '/' and get the last part
        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const phases = await companyModel.getPortofolioPhases(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(phases));
    } catch (error) {
        console.error('Error in getCompanyPhases:', error);
    }
}


async function getPortofolioReviews(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        // Split the path by '/' and get the last part
        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const reviews = await companyModel.getPortofolioReviews(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(reviews));
    } catch (error) {
        console.error('Error in getCompanyReviews:', error);
    }
}

module.exports = { getPortofolioReviews, getPortofolioDetails, getPortofolioPhases }