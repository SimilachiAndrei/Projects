const databaseManager = require('../Database/dbManager');

async function getCompanies(req, res) {
    try {
        const companiesData = await databaseManager.getCompanies();
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(companiesData));
    } catch (error) {
        console.error('Error in getCompanies controller:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

async function addMotto(data, id) {
    try {
        const response = await databaseManager.addMotto(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addMotto:', error);
        throw error;
    }
}

async function addName(data, id) {
    try {
        const response = await databaseManager.addName(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addName:', error);
        throw error;
    }
}

async function addAddress(data, id) {
    try {
        const response = await databaseManager.addAddress(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addAddress:', error);
        throw error;
    }
}

async function addPhone(data, id) {
    try {
        const response = await databaseManager.addPhone(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addPhone:', error);
        throw error;
    }
}


async function addDescription(data, id) {
    try {
        const response = await databaseManager.addDescription(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addDescription:', error);
        throw error;
    }
}

async function getCompany(id) {
    try {
        const response = await databaseManager.getCompany(id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.getCompany:', error);
        throw error;
    }
}

async function getCompanyDetails(companyName) {
    try {
        const company = await databaseManager.getCompanyDetails(companyName);
        return company;
    } catch (error) {
        throw error;
    }
}

async function getCompanyPhases(companyName) {
    try {
        const phases = await databaseManager.getCompanyPhases(companyName);
        return phases;
    } catch (error) {
        throw error;
    }
}


async function getCompanyReviews(companyName) {
    try {
        const reviews = await databaseManager.getCompanyReviews(companyName);
        return reviews;
    } catch (error) {
        throw error;
    }
}

async function getPortofolioDetails(companyName) {
    try {
        const company = await databaseManager.getPortofolioDetails(companyName);
        return company;
    } catch (error) {
        throw error;
    }
}

async function getPortofolioPhases(companyName) {
    try {
        const phases = await databaseManager.getPortofolioPhases(companyName);
        return phases;
    } catch (error) {
        throw error;
    }
}


async function getPortofolioReviews(companyName) {
    try {
        const reviews = await databaseManager.getPortofolioReviews(companyName);
        return reviews;
    } catch (error) {
        throw error;
    }
}


module.exports = { getCompanies, addMotto, getCompany, getCompanyDetails, 
    getCompanyPhases, getCompanyReviews, addName, addAddress, addPhone, addDescription,
    getPortofolioReviews, getPortofolioPhases, getPortofolioDetails };
