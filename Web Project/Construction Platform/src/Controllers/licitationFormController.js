const utils = require('../utils/utils');
const offerModel = require('../Models/offerModel');
const phaseModel = require('../Models/phaseModel');


async function addOffer(req, res) {
    // Check if the user is authenticated and is a company
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', async () => {
            try {
                const formData = new URLSearchParams(body);
                const data = Object.fromEntries(formData.entries());

                const result = await offerModel.addOffer(data, user.id);

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ message: 'Offer added successfully', offerId: result.offerId }));
            } catch (error) {
                console.error('Error adding offer:', error);
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: error.message }));
            }
        });
    } catch (error) {
        console.error('Error in addOffer licitationFormController:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

async function getAvailableLicitations(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'company') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const licitations = await phaseModel.getAvailableLicitations();
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(licitations));
    } catch (error) {
        console.error('Error in licitationFormController.phaseModel:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }

}

module.exports = { addOffer, getAvailableLicitations }