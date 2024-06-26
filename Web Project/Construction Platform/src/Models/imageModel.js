const databaseManager = require('../Database/dbManager');

async function updateOrInsertProfilePicture
    (userId, fileName, imageData) {
        try {
            const response = await databaseManager.updateOrInsertProfilePicture(userId, fileName, imageData);
            return response;
        } catch (error) {
            console.error('Error in companyModel.addMotto:', error);
            throw error; // Re-throw to handle in your controller
        }
}

async function addPhasePicture
    (phaseId, fileName, imageData) {
        try {
            const response = await databaseManager.addPhasePicture(phaseId, fileName, imageData);
            return response;
        } catch (error) {
            console.error('Error in companyModel.addMotto:', error);
            throw error; // Re-throw to handle in your controller
        }
}

module.exports = {addPhasePicture, updateOrInsertProfilePicture}