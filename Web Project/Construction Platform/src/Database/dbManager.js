// src/Database/databaseManager.js

const { Pool } = require('pg');
const bcrypt = require('bcrypt');


const pool = new Pool({
    user: 'web',
    host: 'localhost',
    database: 'tw',
    password: 'web',
    port: 5432,
    max: 20,
});

// Function to get a user by username
async function getUserByUsername(username) {
    try {
        const result = await pool.query('SELECT * FROM users WHERE username = $1', [username]);
        return result.rows[0];
    } catch (error) {
        console.error('Error getting user:', error);
        throw error;
    }
}

// Function to add a user 
async function addUser(userData) {
    try {
        const { username, email, password, role, ...additionalData } = userData;
        if (!role) {
            throw new Error("Role is required");
        }

        const hashedPassword = await bcrypt.hash(password, 10);
        const result = await pool.query(
            'INSERT INTO users (username, email, password, role) VALUES ($1, $2, $3, $4) RETURNING *',
            [username, email, hashedPassword, role]
        );

        const userId = result.rows[0].id;

        if (role === 'client') {
            // Insert client-specific data into the clients table
            await pool.query(
                'INSERT INTO clients (user_id) VALUES ($1)',
                [userId]
            );
        } else if (role === 'company') {
            // Insert company-specific data into the companies table
            await pool.query(
                'INSERT INTO companies (user_id, company_name, company_address, company_phone, company_profile) VALUES ($1, $2, $3, $4, $5)',
                [userId, additionalData.companyName, additionalData.companyAddress, additionalData.companyPhone, additionalData.companyProfile]
            );
        }

        return result.rows[0];
    } catch (error) {
        console.error('Error adding user:', error);
        throw error;
    }
}

async function addPost(postData, clientId) {
    if (!clientId) {
        throw new Error('Client ID is required');
    }

    const { projectName, description, ...phasesObj } = postData;

    try {
        // Insert project
        const result = await pool.query(
            'INSERT INTO projects (client_id, project_name, description) VALUES ($1, $2, $3) RETURNING *',
            [clientId, projectName, description]
        );

        const projectId = result.rows[0].id;

        const phaseList = Object.values(phasesObj);

        // Insert phases
        for (const phase of phaseList) {
            await pool.query(
                'INSERT INTO phases (project_id, description,state) VALUES ($1, $2,$3)',
                [projectId, phase, 'notpending']
            );
        }

        return result.rows[0];
    } catch (error) {
        console.error('Error adding post:', error);
        throw error;
    }
}

async function addLicitation(postData, clientId) {
    if (!clientId) {
        throw new Error('Client ID is required');
    }

    try {
        const jsonString = Object.keys(postData)[0];
        const data = JSON.parse(jsonString);
        const id = data.phaseId;

        const result = await pool.query(
            'UPDATE phases SET state = $1 WHERE id = $2',
            ['pending', id]
        );

        return result.rows[0];
    } catch (error) {
        console.error('Error adding post:', error);
        throw error;
    }
}


async function addOffer(data, companyId) {
    const jsonString = Object.keys(data)[0];
    const dates = JSON.parse(jsonString);
    const { phase_id, start_date, end_date, price } = dates;
    try {
        // Check if the phase exists and its state is 'pending'
        const phaseQuery = 'SELECT state FROM phases WHERE id = $1';
        const phaseResult = await pool.query(phaseQuery, [phase_id]);

        if (phaseResult.rows.length === 0) {
            return 'Phase not found';
        }

        const phaseState = phaseResult.rows[0].state;

        if (phaseState !== 'pending') {
            return 'Cannot add offer for phases with state other than pending';
        }

        // Insert the offer into the offers table
        const insertQuery = `
            INSERT INTO offers (phase_id, start_date, end_date, company_id, price)
            VALUES ($1, $2, $3, $4, $5)
            RETURNING id`;

        const values = [phase_id, start_date, end_date, companyId, price];
        const result = await pool.query(insertQuery, values);

        return { offerId: result.rows[0].id };
    } catch (error) {
        console.error('Error adding offer:', error);
        throw error; // Re-throw the error to be handled in the controller
    }
}



//TO DO later for my account and other stuff
const getAllUserData = async (username) => {
    const query = `
        SELECT 
            u.id AS user_id, 
            u.username, 
            u.email, 
            u.role,
            p.id AS project_id,
            p.project_name,
            p.description AS project_description,
            ph.id AS phase_id,
            ph.description AS phase_description,
            ph.start_date,
            ph.end_date,
            ph.state,
            ph.company_id,
            co.company_name,
            co.company_address,
            co.company_phone,
            co.company_profile,
            i.id AS image_id,
            i.data AS image_data
        FROM 
            users u
        LEFT JOIN 
            clients c ON c.user_id = u.id
        LEFT JOIN 
            projects p ON p.client_id = c.user_id
        LEFT JOIN 
            phases ph ON ph.project_id = p.id
        LEFT JOIN 
            companies co ON co.user_id = ph.company_id
        LEFT JOIN 
            images i ON i.phase_id = ph.id
        WHERE 
            u.username = $1;
    `;
    const values = [username];

    try {
        const result = await pool.query(query, values);

        // Check if result.rows has any elements
        if (result.rows.length === 0) {
            return [];
        }

        const userData = {
            user: {
                id: result.rows[0].user_id,
                username: result.rows[0].username,
                email: result.rows[0].email,
                role: result.rows[0].role
            },
            projects: result.rows.reduce((projects, row) => {
                let project = projects.find(project => project.id === row.project_id);
                if (!project) {
                    project = {
                        id: row.project_id,
                        name: row.project_name,
                        description: row.project_description,
                        phases: []
                    };
                    projects.push(project);
                }
                if (row.phase_id) {
                    let phase = project.phases.find(phase => phase.id === row.phase_id);
                    if (!phase) {
                        phase = {
                            id: row.phase_id,
                            description: row.phase_description,
                            startDate: row.start_date,
                            endDate: row.end_date,
                            state: row.state,
                            company: row.company_id ? {
                                id: row.company_id,
                                name: row.company_name,
                                address: row.company_address,
                                phone: row.company_phone,
                                profile: row.company_profile
                            } : null,
                            images: []
                        };
                        project.phases.push(phase);
                    }
                    if (row.image_id) {
                        phase.images.push({
                            id: row.image_id,
                            data: row.image_data
                        });
                    }
                }
                return projects;
            }, [])
        };

        return userData;
    } catch (error) {
        console.error('Error retrieving all user data:', error);
        throw error;
    }
};



const getCompanies = async () => {
    const query = `
        SELECT 
            co.id AS company_id,
            co.company_name,
            co.company_address,
            co.company_phone,
            co.company_profile,
            i.id AS image_id,
            i.name AS image_name,
            i.data AS image_data
        FROM 
            companies co
        LEFT JOIN 
            images i ON i.profile_picture = co.user_id;
    `;

    try {
        const result = await pool.query(query);
        const companiesData = result.rows.map(row => ({
            company: {
                id: row.company_id,
                name: row.company_name,
                address: row.company_address,
                phone: row.company_phone,
                profile: row.company_profile
            },
            image: {
                id: row.image_id,
                name: row.image_name,
                data: row.image_data
            }
        }));
        return companiesData;
    } catch (error) {
        console.error('Error retrieving company data:', error);
        throw error;
    }
};

async function addMotto(userData, id) {
    try {
        const data = JSON.parse(Object.keys(userData)[0]);

        const motto = data.motto;

        // Update the companies table with the motto
        const result = await pool.query(
            'UPDATE companies SET motto = $1 WHERE user_id = $2 RETURNING id',
            [motto, id]
        );

        const companyId = result.rows[0].id; // Get the updated company ID

        // Return the updated company information
        return result.rows[0];
    } catch (error) {
        console.error('Error adding motto:', error);
        throw error;
    }
}

async function addDescription(userData, id) {
    try {
        const data = JSON.parse(Object.keys(userData)[0]);

        const description = data.description;

        // Update the companies table with the motto
        const result = await pool.query(
            'UPDATE companies SET description = $1 WHERE user_id = $2 RETURNING id',
            [description, id]
        );

        const companyId = result.rows[0].id; // Get the updated company ID

        // Return the updated company information
        return result.rows[0];
    } catch (error) {
        console.error('Error adding description:', error);
        throw error;
    }
}

async function addName(userData, id) {
    try {
        const data = JSON.parse(Object.keys(userData)[0]);

        const name = data.name;

        // Update the companies table with the motto
        const result = await pool.query(
            'UPDATE companies SET company_name = $1 WHERE user_id = $2 RETURNING id',
            [name, id]
        );

        const companyId = result.rows[0].id; // Get the updated company ID

        // Return the updated company information
        return result.rows[0];
    } catch (error) {
        console.error('Error adding name:', error);
        throw error;
    }
}


async function addAddress(userData, id) {
    try {
        const data = JSON.parse(Object.keys(userData)[0]);

        const address = data.address;

        // Update the companies table with the motto
        const result = await pool.query(
            'UPDATE companies SET company_address = $1 WHERE user_id = $2 RETURNING id',
            [address, id]
        );

        const companyId = result.rows[0].id; // Get the updated company ID

        // Return the updated company information
        return result.rows[0];
    } catch (error) {
        console.error('Error adding address:', error);
        throw error;
    }
}

async function addPhone(userData, id) {
    try {
        const data = JSON.parse(Object.keys(userData)[0]);

        const phone = data.phone;

        // Update the companies table with the motto
        const result = await pool.query(
            'UPDATE companies SET company_phone = $1 WHERE user_id = $2 RETURNING id',
            [phone, id]
        );

        const companyId = result.rows[0].id; // Get the updated company ID

        // Return the updated company information
        return result.rows[0];
    } catch (error) {
        console.error('Error adding phone:', error);
        throw error;
    }
}

async function updateOrInsertProfilePicture(userId, fileData) {
    try {
        const query = `
            INSERT INTO images (name, profile_picture, data)
            VALUES ($1, $2, $3)
            ON CONFLICT (profile_picture) DO UPDATE
            SET data = EXCLUDED.data
        `;
        const values = ['profile', userId, fileData];
        await pool.query(query, values);
    } catch (error) {
        console.error('Error in companyModel.updateOrInsertProfilePicture:', error);
        throw error;
    }
}


async function getCompany(id) {
    const query = `
        SELECT 
            co.company_name,
            co.company_address,
            co.company_phone,
            co.company_profile,
            co.motto,
            co.description,
            i.id AS image_id,
            i.name AS image_name,
            i.data AS image_data
        FROM 
            companies co
        LEFT JOIN 
            images i ON i.profile_picture = co.user_id
        WHERE 
            co.user_id = $1;
    `;

    try {
        const result = await pool.query(query, [id]);
        if (result.rows.length === 0) {
            throw new Error(`Company with id ${id} not found`);
        }

        const row = result.rows[0];
        const companyData = {
            company: {
                id: row.company_id,
                name: row.company_name,
                address: row.company_address,
                phone: row.company_phone,
                profile: row.company_profile,
                motto: row.motto,
                description: row.description
            },
            image: {
                id: row.image_id,
                name: row.image_name,
                data: row.image_data,
            },
        };

        return companyData;
    } catch (error) {
        console.error('Error retrieving company data:', error);
        throw error;
    }
}

// select * from phases ph join projects pr on ph.project_id = pr.id join users u on pr.client_id=u.id;

const getAvailableLicitations = async () => {
    const query = `
        SELECT
            ph.id,
            ph.project_id,
            ph.description,
            ph.start_date,
            ph.end_date,
            ph.state,
            ph.company_id,
            ph.price,
            u.username,
            u.id AS user_id,
            pr.project_name
        FROM
            phases ph
            join projects pr on ph.project_id = pr.id 
            join users u on pr.client_id=u.id
        WHERE
            ph.state = 'pending'
    `;

    try {
        const result = await pool.query(query);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving available licitations:', error);
        throw error;
    }
};



async function getOffers(clientId) {
    const query = `
        SELECT 
            o.id, 
            o.phase_id, 
            o.start_date, 
            o.end_date, 
            o.company_id, 
            o.price,
            u.username AS company_name
        FROM 
            offers o 
        JOIN 
            phases ph ON ph.id = o.phase_id 
        JOIN 
            projects pr ON ph.project_id = pr.id 
        JOIN 
            companies co ON co.user_id = o.company_id 
        JOIN 
            users u ON co.user_id = u.id
        WHERE 
            pr.client_id = $1;
    `;

    try {
        const result = await pool.query(query, [clientId]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving offers:', error);
        throw error;
    }
}

async function acceptOffer(offerId) {
    try {
        // Begin transaction
        await pool.query('BEGIN');

        try {
            // Retrieve offer details
            const offerQuery = `
                SELECT 
                    o.phase_id, 
                    o.start_date, 
                    o.end_date, 
                    o.company_id, 
                    o.price
                FROM 
                    offers o
                LEFT JOIN phases p ON p.id = o.phase_id
                WHERE 
                    o.id = $1 and p.state LIKE 'pending';
            `;
            const offerResult = await pool.query(offerQuery, [offerId]);
            const offer = offerResult.rows[0];

            if (!offer) {
                return "Offer or phase not found!";
            }

            // Update phases table
            const updatePhaseQuery = `
                UPDATE phases
                SET start_date = $1,
                    end_date = $2,
                    price = $3,
                    company_id = $4,
                    state = 'taken'
                WHERE id = $5;
            `;
            await pool.query(updatePhaseQuery, [offer.start_date, offer.end_date, offer.price, offer.company_id, offer.phase_id]);

            // Delete related offers
            const deleteOffersQuery = `
                DELETE FROM offers
                WHERE phase_id = $1;
            `;
            await pool.query(deleteOffersQuery, [offer.phase_id]);

            // Commit transaction
            await pool.query('COMMIT');

            return { message: 'Offer accepted successfully' };
        } catch (error) {
            // Rollback transaction on error
            await pool.query('ROLLBACK');
            console.error('Error during transaction in acceptOffer:', error);
            throw error;
        }
    } catch (error) {
        console.error('Error in acceptOffer:', error);
        throw error;
    }
}


async function getProjects(companyId) {
    const query = `
     SELECT id , description FROM
     phases where company_id = $1 and state LIKE 'taken'
    `;

    try {
        const result = await pool.query(query, [companyId]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving projects:', error);
        throw error;
    }
}

async function getFinishedProjects(userId) {
    const query = `
    SELECT ph.id, ph.company_id
    FROM phases ph
    JOIN projects pr ON pr.id = ph.project_id
    WHERE pr.client_id = $1 AND ph.state LIKE 'finished';
    `;

    try {
        const result = await pool.query(query, [userId]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving projects:', error);
        throw error;
    }
}


async function addPhasePicture(phaseId, fileName, fileData) {
    try {
        const query = `
            INSERT INTO images (name, phase_id, data)
            VALUES ($1, $2, $3) ON CONFLICT (phase_id) DO UPDATE
            SET data = EXCLUDED.data
        `;
        const values = [fileName, phaseId, fileData];
        await pool.query(query, values);

        const currentDate = new Date();

        const year = currentDate.getFullYear();
        const month = String(currentDate.getMonth() + 1).padStart(2, '0');
        const day = String(currentDate.getDate()).padStart(2, '0');
        const formattedDate = `${year}-${month}-${day}`;
        const endDate = formattedDate;



        const query2 = `
            UPDATE phases SET state = 'finished', end_date = $2 WHERE id = $1
        `;
        const values2 = [phaseId,endDate];
        await pool.query(query2, values2);
    } catch (error) {
        console.error('Error in companyModel.addPhasePicture:', error);
        throw error;
    }
}


async function submitReview(data, userId) {
    const jsonString = Object.keys(data)[0];
    const dates = JSON.parse(jsonString);
    const { company_id, phase_id, description } = dates;
    try {
        // Insert the offer into the offers table client_id | company_id | phase_id | description
        const selectQuery = `SELECT state FROM phases WHERE id = $1;`;

        const insertQuery = `
            INSERT INTO reviews (client_id, company_id, phase_id, description)
            VALUES ($1, $2, $3, $4)
            RETURNING id`;
        const state = (await pool.query(selectQuery, [phase_id])).rows[0];
        if (state == 'reviewed') return "Phase already reviewed!";
        const values = [userId, company_id, phase_id, description];
        const result = await pool.query(insertQuery, values);

        const updatePhaseQuery = `
        UPDATE phases SET state = 'reviewed' WHERE id = $1`;

        const updateValues = [phase_id];
        await pool.query(updatePhaseQuery, updateValues);

        return true;
    } catch (error) {
        console.error('Error adding offer:', error);
        throw error;
    }
}


async function getReviews() {
    const query = `
    SELECT id, description FROM reviews;
    `;

    try {
        const result = await pool.query(query, []);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving projects:', error);
        throw error;
    }
}

async function deleteReview(id) {
    const query = `
    DELETE FROM reviews WHERE id = $1;
    `;

    try {
        const result = await pool.query(query, [id]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving projects:', error);
        throw error;
    }
}


async function getCompanyDetails(companyName) {
    const query = `
        SELECT u.username, c.id, c.user_id, c.company_name, c.company_address, c.company_phone, 
               c.company_profile, c.motto, c.description, i.data
        FROM companies c
        JOIN users u ON u.id = c.user_id
        LEFT JOIN images i ON i.profile_picture = c.user_id
        WHERE c.company_name = $1
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows[0];
    } catch (error) {
        console.error('Error retrieving company details:', error);
        throw error;
    }
}


async function getCompanyPhases(companyName) {
    const query = `
        SELECT p.id, p.description, p.start_date, p.end_date, p.state, p.price , i.name,
        i.data
        FROM phases p
        JOIN companies c ON p.company_id = c.user_id
        LEFT JOIN images i ON i.phase_id = p.id
        WHERE c.company_name = $1 and (p.state LIKE 'finished' or p.state LIKE 'reviewed')
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving company phases:', error);
        throw error;
    }
}


async function getCompanyReviews(companyName) {
    const query = `
        SELECT r.id, r.client_id, r.phase_id, r.description
        FROM reviews r
        JOIN companies c ON r.company_id = c.user_id
        WHERE c.company_name = $1
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving company reviews:', error);
        throw error;
    }
}

async function getPortofolioDetails(companyName) {
    const query = `
        SELECT u.username, c.id, c.user_id, c.company_name, c.company_address, c.company_phone, 
               c.company_profile, c.motto, c.description, i.data
        FROM companies c
        JOIN users u ON u.id = c.user_id
        LEFT JOIN images i ON i.profile_picture = c.user_id
        WHERE u.username = $1
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows[0];
    } catch (error) {
        console.error('Error retrieving company details:', error);
        throw error;
    }
}


async function getPortofolioPhases(companyName) {
    const query = `
        SELECT p.id, p.description, p.start_date, p.end_date, p.state, p.price , i.name,
        i.data
        FROM phases p
        JOIN companies c ON p.company_id = c.user_id
        LEFT JOIN images i ON i.phase_id = p.id
        JOIN users u ON c.user_id = u.id
        WHERE u.username = $1 and (p.state LIKE 'finished' or p.state LIKE 'reviewed')
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving company phases:', error);
        throw error;
    }
}


async function getPortofolioReviews(companyName) {
    const query = `
        SELECT r.id, r.client_id, r.phase_id, r.description
        FROM reviews r
        JOIN companies c ON r.company_id = c.user_id
        JOIN users u ON u.id = c.user_id
        WHERE u.username = $1
    `;
    try {
        const result = await pool.query(query, [companyName]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving company reviews:', error);
        throw error;
    }
}



async function subcontract(id) {
    const query = `
        UPDATE phases SET price = NULL , company_id = NULL, start_date = NULL, end_date = NULL,
        state = 'pending' where id = $1;
    `;
    try {
        const result = await pool.query(query, [id]);
        return result.rows;
    } catch (error) {
        console.error('Error retrieving company reviews:', error);
        throw error;
    }
}

async function changePassword(userId, oldPassword, newPassword) {
    const selectQuery = `
        SELECT password FROM users WHERE id = $1;
    `;
    const updateQuery = `
        UPDATE users SET password = $1 WHERE id = $2;
    `;

    try {
        // Fetch the existing hashed password from the database
        const result = await pool.query(selectQuery, [userId]);
        if (result.rows.length === 0) {
            return null
        }

        const storedHashedPassword = result.rows[0].password;

        // Compare the provided old password with the stored hashed password
        const isMatch = await bcrypt.compare(oldPassword, storedHashedPassword);
        if (!isMatch) {
            return null;
        }

        // Hash the new password
        const hashedNewPassword = await bcrypt.hash(newPassword, 10);

        // Update the password in the database
        await pool.query(updateQuery, [hashedNewPassword, userId]);

        return { success: true, message: 'Password changed successfully' };
    } catch (error) {
        console.error('Error changing password:', error);
        throw error;
    }
}

async function forgotPassword(username, newPassword) {
    const updateQuery = `
        UPDATE users SET password = $1 WHERE username = $2;
    `;

    try {

        const hashedNewPassword = await bcrypt.hash(newPassword, 10);

        // Update the password in the database
        await pool.query(updateQuery, [hashedNewPassword, username]);

        return { success: true, message: 'Password changed successfully' };
    } catch (error) {
        console.error('Error changing password:', error);
        throw error;
    }
}

module.exports = {
    getUserByUsername, addUser, getAllUserData, addPost,
    getCompanies, addMotto, getCompany, updateOrInsertProfilePicture, addLicitation,
    getAvailableLicitations, addOffer, getOffers, acceptOffer, getProjects,
    addPhasePicture, getFinishedProjects, submitReview, getReviews, deleteReview,
    getCompanyDetails, getCompanyPhases, getCompanyReviews, subcontract,
    addPhone, addAddress, addDescription, addName, changePassword, getPortofolioReviews,
    getPortofolioDetails, getPortofolioPhases, forgotPassword
};
