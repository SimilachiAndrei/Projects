const utils = require('../utils/utils');
const reviewModel = require('../Models/reviewModel');

async function getReviews(req, res) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'admin') {
        res.writeHead(401, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Unauthorized' }));
        return;
    }

    try {
        const reviews = await reviewModel.getReviews();
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(reviews));
    } catch (error) {
        console.error('Error in adminController.reviewModel:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}


async function deleteReview(req, res, reviewId) {
    const user = utils.isAuthenticated(req);
    if (!user || user.role !== 'admin') {
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
                const result = await reviewModel.deleteReview(reviewId);

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify('Review deleted successfully'));
            } catch (error) {
                console.error('Error deleting review:', error);
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(error.message));
            }
        });
    } catch (error) {
        console.error('Error in adminController:', error);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
}

module.exports = {deleteReview, getReviews}