const utils = require('../utils/utils');
const clientModel = require('../Models/clientModel');
const companyModel = require('../Models/companyModel');
const phaseModel = require('../Models/phaseModel');
const imageModel = require('../Models/imageModel');
const userModel = require('../Models/userModel');

const fs = require('fs');


async function getUserData(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user) {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const userData = await clientModel.getClientData(user.username);
        if (!userData) {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'User not found' }));
            return;
        }

        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(userData));
    } catch (error) {
        console.error('Error in myAccountController.getUserData:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

async function addMotto(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const data = Object.fromEntries(formData.entries());

        try {
            const motto = await companyModel.addMotto(data, user.id);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Motto added successfully' }));
        } catch (error) {
            console.error('Error in myAccountController.addMotto:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

async function addDescription(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const data = Object.fromEntries(formData.entries());

        try {
            const description = await companyModel.addDescription(data, user.id);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Description added successfully' }));
        } catch (error) {
            console.error('Error in myAccountController.addDescription:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

async function addPhone(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const data = Object.fromEntries(formData.entries());

        try {
            const phone = await companyModel.addPhone(data, user.id);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Phone number added successfully' }));
        } catch (error) {
            console.error('Error in myAccountController.addPhone:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

async function addAddress(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const data = Object.fromEntries(formData.entries());

        try {
            const address = await companyModel.addAddress(data, user.id);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Address added successfully' }));
        } catch (error) {
            console.error('Error in myAccountController.addAddress:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

async function addName(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const data = Object.fromEntries(formData.entries());

        try {
            const name = await companyModel.addName(data, user.id);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Name added successfully' }));
        } catch (error) {
            console.error('Error in myAccountController.addName:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}

async function addLicitation(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const postData = Object.fromEntries(formData.entries());

        try {
            const post = await phaseModel.addLicitation(postData, user.id); // Ensure user.id is passed as clientId
            res.writeHead(302, { 'Location': '/myacc' });
            res.end();
        } catch (error) {
            console.error('Error in myAccountController.addLicitation:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });

}

const multer = require('multer');
const upload = multer({ dest: 'uploads/' });

async function updateProfilePicture(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const fileData = fs.readFileSync(req.file.path);
        await imageModel.updateOrInsertProfilePicture(user.id, fileData);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ message: 'Profile picture updated successfully' }));
    } catch (error) {
        console.error('Error in myAccountController.updateProfilePicture:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
    finally {
        fs.unlinkSync(req.file.path); // Remove the file from the temp directory after processing
    }
}

async function getCompany(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const company = await companyModel.getCompany(user.id);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(company));
    } catch (error) {
        console.error('Error in myAccountController.getCompany:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }

}


async function changePassword(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role == 'admin') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    

    req.on('end', async () => {
        try {
            const data = JSON.parse(body);
            const { currentPassword, newPassword, confirmPassword } = data;
            
            console.log(currentPassword);

            if (newPassword !== confirmPassword) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'New password and confirmed password do not match' }));
                return;
            }

            const response = await userModel.changePassword(user.id, currentPassword, newPassword);
            
            if (response != null) {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ message: 'Password changed successfully' }));
            } else {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify('Failed to change password' ));
            }
        } catch (error) {
            console.error('Error in changePassword:', error);
        }
    });
}


module.exports = { getUserData, getCompany, updateProfilePicture, addLicitation, addMotto,
    addDescription, addPhone, addAddress, addName, changePassword
 };