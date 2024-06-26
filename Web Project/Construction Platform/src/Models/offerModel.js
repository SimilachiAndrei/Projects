const databaseManager = require('../Database/dbManager');

async function getOffers(id) {
    try {
        const offers = await databaseManager.getOffers(id);
        return offers;
    } catch (error) {
        console.error('Error in userModel.getUser:', error);
        throw error; 
    }
}

async function acceptOffer(id) {
    try {
        const offers = await databaseManager.acceptOffer(id);
        return offers;
    } catch (error) {
        console.error('Error in userModel.acceptOffer:', error);
        throw error; 
    }
}

async function addOffer(data, id) {
    try {
        const response = await databaseManager.addOffer(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addOffer:', error);
        throw error; // Re-throw to handle in your controller
    }
}

module.exports = { getOffers, acceptOffer, addOffer }