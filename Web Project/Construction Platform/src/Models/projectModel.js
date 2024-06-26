const databaseManager = require('../Database/dbManager');

async function addPost(postData, clientId) {
    if (!clientId) {
        throw new Error('Client ID is required to add a post');
    }
    
    try {
        const post = await databaseManager.addPost(postData, clientId);
        return post;
    } catch (error) {
        console.error('Error in userModel.addPost:', error);
        throw error;
    }
}


module.exports = { addPost };