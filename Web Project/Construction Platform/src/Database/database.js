const config = require("../utils/configuration");
const { encryptedPassword } = require("../security/passwordEncrypted");

const databaseInitialization = async (pool) => {
    try {
        const client = await pool.connect();

        const dropTablesQuery = `
    DROP TABLE IF EXISTS "images" CASCADE;
    DROP TABLE IF EXISTS "phases" CASCADE;
    DROP TABLE IF EXISTS "projects" CASCADE;
    DROP TABLE IF EXISTS "companies" CASCADE;
    DROP TABLE IF EXISTS "clients" CASCADE;
    DROP TABLE IF EXISTS "users" CASCADE;
    DROP TABLE IF EXISTS "offers" CASCADE;
    DROP TABLE IF EXISTS "reviews" CASCADE;


`; 

        // await client.query(dropTablesQuery);

        const createUsersTableQuery = `
    CREATE TABLE IF NOT EXISTS "users"
    (
        "id"          SERIAL PRIMARY KEY,
        "username"    VARCHAR(255) NOT NULL,
        "email"       VARCHAR(255) NOT NULL,
        "password"    VARCHAR(255) NOT NULL,
        "role"        VARCHAR(255) NOT NULL CHECK (role IN ('admin', 'client', 'company'))
    );`;

        const createClientsTableQuery = `
    CREATE TABLE IF NOT EXISTS "clients"
    (
        "id"          SERIAL PRIMARY KEY,
        "user_id"     INTEGER NOT NULL UNIQUE REFERENCES users(id) ON DELETE CASCADE
    );`;

        const createCompaniesTableQuery = `
    CREATE TABLE IF NOT EXISTS "companies"
    (
        "id"          SERIAL PRIMARY KEY,
        "user_id"     INTEGER NOT NULL UNIQUE REFERENCES users(id) ON DELETE CASCADE,
        "company_name" TEXT,
        "company_address" TEXT,
        "company_phone" TEXT,
        "company_profile" TEXT,
        "motto" TEXT,
        "description" TEXT
    );`; //profile picture

        const createProjectsTableQuery = `
    CREATE TABLE IF NOT EXISTS "projects"
    (
        "id"          SERIAL PRIMARY KEY,
        "client_id"   INTEGER NOT NULL REFERENCES clients(user_id) ON DELETE CASCADE,
        "project_name" VARCHAR(255) NOT NULL,
        "description" TEXT
    );`;

        const createPhasesTableQuery = `
    CREATE TABLE IF NOT EXISTS "phases"
    (
        "id"          SERIAL PRIMARY KEY,
        "project_id"  INTEGER NOT NULL REFERENCES projects(id) ON DELETE CASCADE,
        "description" TEXT,
        "start_date"  DATE,
        "end_date"    DATE,
        "state"       TEXT,
        "company_id"  INTEGER REFERENCES companies(user_id) ON DELETE CASCADE,
        "price"       NUMERIC(10, 2)
    );`;

    const createOfferTableQuery = `
    CREATE TABLE IF NOT EXISTS "offers"
    (
        "id"          SERIAL PRIMARY KEY,
        "phase_id"    INTEGER NOT NULL REFERENCES phases(id) ON DELETE CASCADE,
        "start_date"  DATE,
        "end_date"    DATE,
        "company_id"  INTEGER REFERENCES companies(user_id) ON DELETE CASCADE,
        "price"       NUMERIC(10, 2)
    );`;

        const createImagesTableQuery = `
    CREATE TABLE IF NOT EXISTS "images"
    (
        "id"          SERIAL PRIMARY KEY,
        "phase_id"    INTEGER UNIQUE REFERENCES phases(id) ON DELETE CASCADE,
        "name"        VARCHAR(255),
        "data"        BYTEA,
        "profile_picture" INTEGER UNIQUE REFERENCES users(id) ON DELETE CASCADE
    );`;

        const createReviewTableQuery = `
    CREATE TABLE IF NOT EXISTS "reviews"
    (
        "id"          SERIAL PRIMARY KEY,
        "client_id"   INTEGER NOT NULL REFERENCES clients(user_id) ON DELETE CASCADE,
        "company_id"  INTEGER NOT NULL REFERENCES companies(user_id) ON DELETE CASCADE,
        "phase_id"  INTEGER NOT NULL REFERENCES phases(id) ON DELETE CASCADE,
        "description"        Text
    );`;

        await client.query(createUsersTableQuery);
        await client.query(createClientsTableQuery);
        await client.query(createCompaniesTableQuery);
        await client.query(createProjectsTableQuery);
        await client.query(createPhasesTableQuery);
        await client.query(createImagesTableQuery);
        await client.query(createReviewTableQuery);
        await client.query(createOfferTableQuery);


        const insertAdminAccountQuery = `INSERT INTO users (username, email, password, role)
                                 VALUES ($1, $2, $3, $4)`;

        const adminRow = await client.query('SELECT * FROM users WHERE role = $1', [config.adminRole]);
        const values = [config.adminUsername, config.adminEmail, await encryptedPassword(config.adminPassword), config.adminRole];

        if (adminRow.rows.length === 0) {
            await client.query(insertAdminAccountQuery, values);
        } else {
            const modifyAdminAccountQuery = `UPDATE users
                                     SET username = $1,
                                         email = $2,
                                         password = $3,
                                         role = $4
                                     WHERE role = $4`;
            await client.query(modifyAdminAccountQuery, values);
        }

        client.release();
        return true;

    } catch (error) {
        console.error(error.stack);
        return false;
    }

};

module.exports = databaseInitialization;