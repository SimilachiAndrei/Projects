const utils = require('../utils/utils');
const companyModel = require('../Models/companyModel');

async function getCompanyDetails(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const company = await companyModel.getCompanyDetails(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(company));
    } catch (error) {
        console.error('Error in getCompanyDetails:', error);
    }
}

async function getCompanyPhases(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const phases = await companyModel.getCompanyPhases(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(phases));
    } catch (error) {
        console.error('Error in getCompanyPhases:', error);
    }
}


async function getCompanyReviews(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }
    try {
        const fullPath = req.url;

        const pathParts = fullPath.split('/');
        const companyName = pathParts[pathParts.length - 1];
        const reviews = await companyModel.getCompanyReviews(companyName);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(reviews));
    } catch (error) {
        console.error('Error in getCompanyReviews:', error);
    }
}

module.exports = { getCompanyReviews, getCompanyDetails, getCompanyPhases }