const path = require('path');
const fs = require('fs');
const url = require('url');
const utils = require('../utils/utils');
const myAccountController = require('../Controllers/myAccountController');
const companiesController = require('../Controllers/companiesController');
const companyController = require('../Controllers/companyController');
const adminController = require('../Controllers/adminController');
const addReviewController = require('../Controllers/addReviewController');
const myProjectsController = require('../Controllers/myProjectsController');
const myoffersController = require('../Controllers/myoffersController');
const licitationFormController = require('../Controllers/licitationFormController');
const portofolioController = require('../Controllers/portofolioController');
const loginController = require('../Controllers/loginController');


function handleRequest(req, res) {
    let filePath = '';
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;

    switch (true) {
        case pathname === '/':
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'ind.html');
            break;
        case pathname === '/signup':
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'signup.html');
            break;
        case pathname === '/login':
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'login.html');
            break;
        case pathname === '/afterlog':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            else if (!utils.isUserType(req, 'admin')) {
                filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'afterlog.html');
            }
            else filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'admin.html');
            break;
        case pathname === '/myacc':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            else if (!utils.isUserType(req, 'admin')) {
                filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'myacc.html');
            }
            else filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'afterlog.html');
            break;
        case pathname === '/post':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            else if (!utils.isUserType(req, 'client')) {
                filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'afterlog.html');
            }
            else filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'post.html');
            break;
        case pathname === '/reviews':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            else if (!utils.isUserType(req, 'client')) {
                filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'afterlog.html');
            }
            else filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'addReview.html');
            break;
        case pathname === '/about':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'about.html');
            break;
        case pathname === '/companies':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'client')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'companies.html');
            break;
        case pathname === '/licitations':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'company')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'licitations.html');
            break;
        case pathname === '/myoffers':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'client')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'myoffers.html');
            break;
        case pathname.startsWith('/licitationForm'):
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'company')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'licitationForm.html');
            break;
        case pathname.startsWith('/company'):
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'client')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'company.html');
            break;
        case pathname.startsWith('/portofolio'):
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            } else if (!utils.isUserType(req, 'company')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'portofolio.html');
            break;
        case pathname === '/myprojects':
            if (!utils.isAuthenticated(req)) {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            else if (!utils.isUserType(req, 'company')) {
                res.setHeader('Content-Type', 'text/html');
                res.end('<script>window.location.href = "/afterlog";</script>');
                return;
            }
            filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'myprojects.html');
            break;
        case pathname.startsWith('/reset-password'):
            const token = new URL(req.url, `http://${req.headers.host}`).searchParams.get('token');
            if (token && loginController.verifyResetToken(token)) {
                filePath = path.join(__dirname, '..', '..', 'public', 'Contents', 'reset-password.html');
            } else {
                res.writeHead(302, { 'Location': '/login' });
                res.end();
                return;
            }
            break;
        case pathname === '/api/user-data':
            myAccountController.getUserData(req, res);
            return;
        case pathname === '/api/role':
            const role = utils.getUserType(req, res);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ role }));
            return;
        case pathname === '/api/companies':
            companiesController.getCompanies(req, res);
            return;
        case pathname === '/api/company':
            myAccountController.getCompany(req, res);
            return;
        case pathname === '/api/licitations':
            licitationFormController.getAvailableLicitations(req, res);
            return;
        case pathname === '/api/offers':
            myoffersController.getOffers(req, res);
            return;
        case pathname === '/api/projects':
            myProjectsController.getProjects(req, res);
            return;
        case pathname === '/api/getFinishedPhases':
            addReviewController.getFinishedProjects(req, res);
            return;
        case pathname === '/api/getReviews':
            adminController.getReviews(req, res);
            return;
        case pathname.startsWith('/api/getCompanyDetails'):
            companyController.getCompanyDetails(req, res);
            return;
        case pathname.startsWith('/api/getCompanyPhases'):
            companyController.getCompanyPhases(req, res);
            return;
        case pathname.startsWith('/api/getCompanyReviews'):
            companyController.getCompanyReviews(req, res);
            return;
        case pathname.startsWith('/api/getPortofolioDetails'):
            portofolioController.getPortofolioDetails(req, res);
            return;
        case pathname.startsWith('/api/getPortofolioPhases'):
            portofolioController.getPortofolioPhases(req, res);
            return;
        case pathname.startsWith('/api/getPortofolioReviews'):
            portofolioController.getPortofolioReviews(req, res);
            return;
        case pathname === '/api/getName':
            const user = utils.isAuthenticated(req);
            if (!user) {
                res.writeHead(401, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Unauthorized' }));
                return;
            } else if (utils.isUserType(req, 'admin')) {
                res.writeHead(302, { 'Location': '/afterlog' });
                res.end();
                return;
            }
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ username: user.username }));
            return;
        default:
            filePath = path.join(__dirname, '..', '..', 'public', req.url);
    }

    fs.readFile(filePath, (err, content) => {
        if (err) {
            if (err.code === 'ENOENT') {
                res.writeHead(404, { 'Content-Type': 'text/plain' });
                res.end('Not Found');
            } else {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Internal Server Error');
            }
        } else {
            let contentType = 'text/html';
            const extname = path.extname(filePath);
            switch (extname) {
                case '.css':
                    contentType = 'text/css';
                    break;
                case '.js':
                    contentType = 'text/javascript';
                    break;
            }
            res.writeHead(200, { 'Content-Type': contentType });
            res.end(content);
        }
    });
}

module.exports = { handleRequest };
