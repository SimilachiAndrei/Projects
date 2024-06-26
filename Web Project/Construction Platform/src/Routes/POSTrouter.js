const postController = require('../Controllers/postController');
const signUpController = require('../Controllers/signUpController');
const loginController = require('../Controllers/loginController');
const myAccountController = require('../Controllers/myAccountController');
const licitationFormController = require('../Controllers/licitationFormController');
const addReviewController = require('../Controllers/addReviewController');


function handleRequest(req, res) {


    switch (req.url) {
        case '/signup':
            signUpController.signup(req, res);
            break;
        case '/login':
            loginController.login(req, res);
            break;
        case '/post':
            postController.addPost(req, res);
            break;
        case '/api/addlicitation':
            myAccountController.addLicitation(req, res);
            break;
        case '/api/addoffer':
            licitationFormController.addOffer(req, res);
            break;
        case '/api/submitReview':
            addReviewController.submitReview(req, res);
            break;
        case '/api/forgotPassword':
            loginController.forgotPassword(req, res);
            break;
        case '/api/resetPassword':
            loginController.resetPassword(req, res);
            break;
        default:
            res.writeHead(404, { 'Content-Type': 'text/plain' });
            res.end('Not Found');
    }

}

module.exports = { handleRequest };