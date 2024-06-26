const phaseModel = require('../Models/phaseModel');
const imageModel = require('../Models/imageModel');
const utils = require('../utils/utils');
const fs = require('fs');

async function addPhasePicture(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        if (typeof res.status === 'function') {
            res.status(401).json({ error: 'Unauthorized' });
        } else {
            res.writeHead(401, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Unauthorized' }));
        }
        return;
    }

    const { phaseId } = req.body;

    try {
        const fileData = fs.readFileSync(req.file.path);
        await imageModel.addPhasePicture(phaseId, req.file.filename, fileData);
        if (typeof res.status === 'function') {
            res.status(200).json({ message: 'Phase picture updated successfully' });
        } else {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Phase picture updated successfully' }));
        }
    } catch (error) {
        console.error('Error in myProjectsController.addPhasePicture:', error);
        if (typeof res.status === 'function') {
            res.status(500).json({ error: 'Internal Server Error' });
        } else {
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    } finally {
        fs.unlinkSync(req.file.path); // Remove the file from the temp directory after processing
    }
}

async function getProjects(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const projects = await phaseModel.getProjects(user.id);
        console.log(projects);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(projects));
    } catch (error) {
        console.error('Error in myProjectsController.getProjects:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }

}


async function subcontract(res,phaseId) {
    try {
        const response = await phaseModel.subcontract(phaseId);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(response));
    } catch (error) {
        console.error('Error in myProjectsController.subcontract:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }

}

module.exports = {addPhasePicture, getProjects, subcontract}