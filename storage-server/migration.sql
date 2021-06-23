CREATE TABLE IF NOT EXISTS tariffs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    space INTEGER NOT NULL UNIQUE
);#

INSERT OR REPLACE INTO tariffs (id, name, space) VALUES (1, 'Standard', 10000);#

CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    login TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    tariff INTEGER NOT NULL,
    FOREIGN KEY (tariff) REFERENCES tariffs (id)
);#

CREATE TABLE IF NOT EXISTS files (
    id INTEGER NOT NULL,
    name TEXT NOT NULL UNIQUE
);#

CREATE TABLE IF NOT EXISTS statistics (
    id INTEGER NOT NULL UNIQUE,
    used_space INTEGER NOT NULL,
    files INTEGER NOT NULL,
    FOREIGN KEY (id) REFERENCES users (id)
);#

CREATE TRIGGER IF NOT EXISTS create_statistics
    AFTER INSERT ON users
BEGIN
        INSERT INTO statistics (id, used_space)
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
