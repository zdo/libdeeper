CREATE TABLE deeper_info (
    `key` TEXT NOT NULL,
    `value` TEXT
);
INSERT INTO deeper_info (key, value) VALUES ('version', '1');

CREATE TABLE categories (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `title` TEXT NOT NULL,
    `is_archived` BOOLEAN NOT NULL DEFAULT FALSE,
    `parent_id` INTEGER,
    `order_index` INTEGER NOT NULL DEFAULT 0,
    `color` TEXT,
    FOREIGN KEY(`parent_id`) REFERENCES `categories`(`id`) ON DELETE CASCADE
);

CREATE TABLE notes (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `title` TEXT NOT NULL,
    `text` TEXT,
    `is_archived` BOOLEAN NOT NULL DEFAULT FALSE,
    `category_id` INTEGER NOT NULL,
    `parent_note_id` INTEGER,
    `order_index` INTEGER NOT NULL DEFAULT 0,
    `creation_time` INTEGER NOT NULL DEFAULT (datetime('now','localtime')),
    `scheduled` INTEGER,
    `scheduled_duration` INTEGER,
    `deadline` INTEGER,
    FOREIGN KEY(`category_id`) REFERENCES `categories`(`id`) ON DELETE CASCADE,
    FOREIGN KEY(`parent_note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);

CREATE TABLE note_states (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `title` TEXT NOT NULL,
    `color` TEXT,
    `is_finished` BOOLEAN NOT NULL DEFAULT FALSE,
    `order_index` INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE note_states_history (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `creation_time` INTEGER NOT NULL DEFAULT (datetime('now','localtime')),
    `note_id` INTEGER NOT NULL,
    `state_id` INTEGER,
    `comment` TEXT,
    FOREIGN KEY(`state_id`) REFERENCES `note_states`(`id`) ON DELETE CASCADE,
    FOREIGN KEY(`note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);

CREATE TABLE tags (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `title` TEXT NOT NULL,
    `color` TEXT
);

CREATE TABLE categories_tags (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `category_id` INTEGER NOT NULL,
    `tag_id` INTEGER NOT NULL,
    FOREIGN KEY(`tag_id`) REFERENCES `tags`(`id`) ON DELETE CASCADE,
    FOREIGN KEY(`category_id`) REFERENCES `categories`(`id`) ON DELETE CASCADE
);

CREATE TABLE notes_tags (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `note_id` INTEGER NOT NULL,
    `tag_id` INTEGER NOT NULL,
    FOREIGN KEY(`tag_id`) REFERENCES `tags`(`id`) ON DELETE CASCADE,
    FOREIGN KEY(`note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);

CREATE TABLE time_tracks (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `note_id` INTEGER NOT NULL,
    `start` INTEGER NOT NULL DEFAULT (datetime('now','localtime')),
    `end` INTEGER,
    FOREIGN KEY(`note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);

CREATE TABLE milestones (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `note_id` INTEGER NOT NULL,
    `creation_time` INTEGER NOT NULL DEFAULT (datetime('now','localtime')),
    `comment` TEXT,
    FOREIGN KEY(`note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);

CREATE TABLE goals (
    `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
    `is_archived` BOOLEAN NOT NULL DEFAULT FALSE,
    `note_id` INTEGER NOT NULL,
    `creation_time` INTEGER NOT NULL DEFAULT (datetime('now','localtime')),
    `title` TEXT NOT NULL,
    `comment` TEXT,
    `duration` INTEGER NOT NULL,
    `target_milestones_count` INTEGER DEFAULT 0,
    `target_minutes_tracked` INTEGER DEFAULT 0,
    FOREIGN KEY(`note_id`) REFERENCES `notes`(`id`) ON DELETE CASCADE
);
