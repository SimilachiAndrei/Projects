const databaseManager = require('../Database/dbManager');

async function addLicitation(postData, clientId) {
    if (!clientId) {
        throw new Error('Client ID is required to add a post');
    }
    
    try {
        const post = await databaseManager.addLicitation(postData, clientId);
        return post;
    } catch (error) {
        console.error('Error in userModel.addLicitation:', error);
        throw error;
    }
}

async function getAvailableLicitations() {
    try {
        const response = await databaseManager.getAvailableLicitations();
        return response;
    } catch (error) {
        console.error('Error in companyModel.addMotto:', error);
        throw error; // Re-throw to handle in your controller
    }
}

async function getProjects(id) {
    try {
        const response = await databaseManager.getProjects(id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.getProjects:', error);
        throw error; // Re-throw to handle in your controller
    }
}

async function getFinishedProjects(id) {
    try {
        const response = await databaseManager.getFinishedProjects(id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.getFinishedProjects:', error);
        throw error; // Re-throw to handle in your controller
    }
}

async function subcontract(id) {
    try {
        const response = await databaseManager.subcontract(id);
        return response;
    } catch (error) {
        console.error('Error in companyModel.getFinishedProjects:', error);
        throw error; // Re-throw to handle in your controller
    }
}

module.exports = { addLicitation, getAvailableLicitations, getProjects, getFinishedProjects, subcontract };