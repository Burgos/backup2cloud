#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "backend/traverser.hpp"
#include "session/backup_session.hpp"
#include "session/do_backup.hpp"
#include "config/configuration.hpp"

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

	auto program_config = readProgramConfig(argv[1]);
	if (!program_config.has_value()) {
		std::cerr << "Could not read the program config.\n";
		return 1;
	}

	auto backups = readBackupsConfig(argv[2]);
	if (!backups.has_value()) {
		std::cerr << "Could not read the backups config.\n";
		return 1;
	}

    for (const auto& backup: *backups) {
		std::cout <<  "Processing config: " << backup.backup_name << std::endl;
        const bool isFull = !checkPreviousBackup(backup.backup_name, db);
        auto backup_dir = createBackupDirectory(fs::path{program_config->backup_path}, backup.backup_name, isFull);
        auto files = listFilesForBackup(backup.backup_name, backup.root_path, db);
        performBackup(backup.backup_name, files, db, [backup_dir, root = fs::path{backup.root_path}](const std::string& path) {
                return backup_file(backup_dir, root, fs::path{path});
        });
    }
    return 0;
} 
