CREATE TABLE IF NOT EXISTS tariffs (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    space INTEGER NOT NULL UNIQUE
);#

INSERT OR REPLACE INTO tariffs (id, name, space) VALUES (1, 'Standard', 10737418240);#

INSERT OR REPLACE INTO tariffs (id, name, space) VALUES (2, 'Extended', 21474836480);#

CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY,
    login TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    tariff INTEGER NOT NULL
);#

CREATE TABLE IF NOT EXISTS files (
    id INTEGER NOT NULL,
    name TEXT NOT NULL UNIQUE
);#

CREATE TABLE IF NOT EXISTS statistics (
    id INTEGER NOT NULL UNIQUE,
    used_space INTEGER NOT NULL,
    files INTEGER NOT NULL
);#

CREATE TRIGGER IF NOT EXISTS create_statistics
    AFTER INSERT ON users
BEGIN
        INSERT INTO statistics (id, used_space, files)
        VALUES (NEW.id, 0, 0);
END;#

CREATE TRIGGER IF NOT EXISTS update_statistics_add
    AFTER INSERT ON files
BEGIN
        UPDATE statistics SET files=files+1
        WHERE id=NEW.id;
END;#

CREATE TRIGGER IF NOT EXISTS update_statistics_delete
    AFTER DELETE ON files
BEGIN
        UPDATE statistics SET files=files-1
        WHERE id=OLD.id;
END;#
