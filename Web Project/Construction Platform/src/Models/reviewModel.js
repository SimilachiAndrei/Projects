const databaseManager = require('../Database/dbManager');

async function submitReview(data, id) {
    try {
        const response = await databaseManager.submitReview(data, id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.addMotto:', error);
        throw error; // Re-throw to handle in your controller
    }
}

async function getReviews() {
    try {
        const response = await databaseManager.getReviews();
        return response;
    } catch (error) {
        console.error('Error in companyModel.getReviews:', error);
        throw error; // Re-throw to handle in your controller
    }
}

async function deleteReview(data) {
    try {
        const response = await databaseManager.deleteReview(data);
        return response;
    } catch (error) {
        console.error('Error in companyModel.deleteReview:', error);
        throw error; // Re-throw to handle in your controller
    }
}

module.exports = { submitReview, getReviews, deleteReview }