// const utils = require('../utils/utils');
const myAccountController = require('../Controllers/myAccountController');
const myProjectsController = require('../Controllers/myProjectsController');
const myoffersController = require('../Controllers/myoffersController');
const multer = require('multer');
const upload = multer({ dest: 'uploads/' });


function handleRequest(req, res) {
    switch (req.url) {
        case '/api/company/motto':
            myAccountController.addMotto(req, res);
            break;
        case '/api/company/description':
            myAccountController.addDescription(req, res);
            break;
        case '/api/company/phone':
            myAccountController.addPhone(req, res);
            break;
        case '/api/company/address':
            myAccountController.addAddress(req, res);
            break;
        case '/api/company/name':
            myAccountController.addName(req, res);
            break;
        case '/api/changePassword':
            myAccountController.changePassword(req, res);
            break;
        case '/api/company/profile-picture':
            upload.single('profilePicture')(req, res, function (err) {
                if (err) {
                    console.error('Error in file upload:', err);
                    res.writeHead(500, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ error: 'Internal Server Error' }));
                    return;
                }
                myAccountController.updateProfilePicture(req, res);
            });
            break;
        case '/api/uploadImage':
            upload.single('phasePicture')(req, res, function (err) {
                if (err) {
                    console.error('Error in file upload:', err);
                    res.writeHead(500, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ error: 'Internal Server Error' }));
                    return;
                }
                myProjectsController.addPhasePicture(req, res);
            });
            break;
        case '/api/acceptoffer':
            myoffersController.acceptOffer(req, res);
            break;
        default:
            res.writeHead(404, { 'Content-Type': 'text/plain' });
            res.end('Not Found');
    }
}

module.exports = { handleRequest };