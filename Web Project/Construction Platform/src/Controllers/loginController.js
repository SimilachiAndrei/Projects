const auth = require('../utils/auth');
const userModel = require('../Models/userModel');
const bcrypt = require('bcrypt');
const nodemailer = require('nodemailer');
const crypto = require('crypto');

let resetToken = null; // Temporary storage for reset token
let resetTokenExpires = null; // Temporary storage for token expiry
let remember = null;

function verifyResetToken(token) {
    if (!resetToken || !resetTokenExpires) {
        return false;
    }

    const now = new Date();
    if (token === resetToken && now < resetTokenExpires) {
        return true;
    } else {
        return false;
    }
}

async function login(req, res) {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        const formData = new URLSearchParams(body);
        const username = formData.get('username');
        const password = formData.get('password');

        const user = await userModel.getUser(username);

        if (!user || !(await bcrypt.compare(password, user.password))) {
            res.writeHead(400, { 'Content-Type': 'text/html' });
            res.end(`
            <script>alert('Invalid credentials!');</script>
            <script>window.location.href = "/login";</script>
          `);
            return;
        }
        const token = auth.generateToken(user);

        res.writeHead(302, {
            'Location': '/afterlog',
            'Set-Cookie': `token=${token}; HttpOnly; SameSite=Strict`
        });
        res.end();
    });
}


function logout(req, res) {
    res.writeHead(302, {
        'Location': '/login',
        'Set-Cookie': 'token=; HttpOnly; SameSite=Strict; Max-Age=0'
    });
    res.end();
}

async function forgotPassword(req, res) {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        try {
            const data = JSON.parse(body);
            const { username } = data;

            const user = await userModel.getUser(username);

            if (user) {
                resetToken = crypto.randomBytes(20).toString('hex'); // Generate token
                resetTokenExpires = new Date();
                resetTokenExpires.setHours(resetTokenExpires.getHours() + 1); // Token expires in 1 hour
                remember = username;
                const resetLink = `http://localhost:3000/reset-password?token=${resetToken}`;
                await sendPasswordResetEmail(user.email, resetLink);

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ message: 'Password reset link sent to your email' }));
            } else {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Failed to retrieve email! No such account!' }));
            }
        } catch (error) {
            console.error('Error in forgotPassword:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}


async function sendPasswordResetEmail(email, resetLink) {
    const transporter = nodemailer.createTransport({
        host: "smtp-mail.outlook.com",
        port: 587,
        secure: false, 
        auth: {
            user: 'andrei.flx@outlook.com', 
            pass: 'mockPassword' 
        }
    });

    const mailOptions = {
        from: 'andrei.flx@outlook.com', 
        to: email,
        subject: 'Password Reset',
        text: `You are receiving this email because you (or someone else) have requested the reset of the password for your account.\n\n`
            + `Please click on the following link, or paste this into your browser to complete the process:\n\n`
            + `${resetLink}\n\n`
            + `If you did not request this, please ignore this email and your password will remain unchanged.\n`
    };

    try {
        const info = await transporter.sendMail(mailOptions);
        console.log('Email sent:', info.response);
        return info;
    } catch (error) {
        console.error('Error sending email:', error);
        throw new Error('Failed to send password reset email');
    }
}

async function resetPassword(req, res) {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        try {
            const data = JSON.parse(body);
            const { token, newPassword } = data;

            if (!verifyResetToken(token)) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Invalid or expired reset token' }));
                return;
            }

            await userModel.forgotPassword(remember, newPassword);

            // Clear the reset token and expiry
            resetToken = null;
            resetTokenExpires = null;
            remember = null;

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Password reset successfully' }));
        } catch (error) {
            console.error('Error in resetPassword:', error);
            res.writeHead(500, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Internal Server Error' }));
        }
    });
}



module.exports = { login, logout, forgotPassword, resetPassword, verifyResetToken };
