const sendMessage = function (res, err) {
    res.statusCode = err.statusCode || 500;
    const response = {
        status: err.status || 'error',
        message: err.message || 'Error '
    };
    res.end(JSON.stringify(response));
};

module.exports = sendMessage;