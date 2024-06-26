const databaseManager = require('../Database/dbManager');


async function getClientData(username) {
    try {
        const userData = await databaseManager.getAllUserData(username);
        return userData;
    } catch (error) {
        console.error('Error in userModel.getAllUserData:', error);
        throw error;
    }
}

module.exports = {getClientData}