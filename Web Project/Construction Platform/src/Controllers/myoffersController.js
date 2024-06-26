const offerModel = require('../Models/offerModel');
const utils = require('../utils/utils');


async function getOffers(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user|| user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const offers = await offerModel.getOffers(user.id);
        if (!offers.length) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify('No offers found'));
            return;
        }

        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(offers));
    } catch (error) {
        console.error('Error in myOfferController.getOffers:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

async function acceptOffer(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'client') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    let body = [];
    req.on('data', (chunk) => {
        body.push(chunk);
    }).on('end', () => {
        try {
            body = Buffer.concat(body).toString();
            const { offerId } = JSON.parse(body);

            // Handle your business logic here, e.g., userModel.acceptOffer(offerId)
            offerModel.acceptOffer(offerId);
            // Send response
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Offer accepted successfully' }));
        } catch (error) {
            console.error('Error parsing JSON:', error);
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Invalid JSON' }));
        }
    });
}


module.exports = {acceptOffer, getOffers}