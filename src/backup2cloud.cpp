#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "backend/traverser.hpp"
#include "session/backup_session.hpp"
#include "session/do_backup.hpp"

namespace fs = std::filesystem;

void initializeDatabase(SQLite::Database& db) {
    SQLite::Statement create_tables{db, "CREATE TABLE backup_files (backup_name TEXT, filename TEXT, filehash TEXT)"};
    create_tables.exec();
    SQLite::Statement create_index{db, "CREATE UNIQUE INDEX file_index ON backup_files(backup_name, filename)"};
    create_index.exec();
}

int main(int argc, char** argv) {
    SQLite::Database db("files.sql", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    if (!db.tableExists("backup_files")) {
        initializeDatabase(db);
    }
    std::string backup_name(argv[1]);
    const bool isFull = !checkPreviousBackup(backup_name, db);
    createBackupDirectory(fs::path{"/Users/burgos/backup/"}, backup_name, isFull);
    for (int i = 2; i < argc; i++) {
        auto files = listFilesForBackup(backup_name, argv[i], db);
        performBackup(backup_name, files, db, [](const std::string& path) {
            std::cout << "Backing up " << path << "\n";
        });
    }
    return 0;
} 
