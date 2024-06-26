const companyModel = require('../Models/companyModel');


async function getCompanies(req, res) {
    try {
        await companyModel.getCompanies(req, res);
    } catch (error) {
        console.error('Error in myAccountController:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

module.exports = { getCompanies };