#ifndef BACKUP2CLOUD_BACKEND_BACKUP_SESSION_H
#define BACKUP2CLOUD_BACKEND_BACKUP_SESSION_H


#include <SQLiteCpp/SQLiteCpp.h>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

/// Defines a backup session for the directory.
/// Goes over a directory and compares if the files are changed, and
/// runs a scheduled work.
std::vector<FileWithHash> listFilesForBackup (const std::string& backup_name, const fs::path& root, SQLite::Database& db) {
    std::vector<FileWithHash> affected_files;
    auto files = calculateHashes(root);
    SQLite::Statement query{db, "SELECT count(*) as num_matches FROM backup_files WHERE backup_name = ? AND filename = ? AND filehash = ?"};
    query.bind(1, backup_name);
    for (const auto& fh: files) {
        // query the db
        query.reset();
        query.bind(2, fh.file_path);
        query.bind(3, fh.hash);
        int num_matches = 0;
        if (query.executeStep() == true) {
            num_matches = query.getColumn("num_matches").getInt();
        }

        switch (num_matches) {
            case 0:
                // backup this one.
                affected_files.push_back(fh);
                break;
            case 1:
                // skip, found a match
                break;
            default:
                throw std::runtime_error("Several hashes for the same filename found.");
                break;
        }
    }
    return affected_files;
}

template <typename BackupFunction>
void performBackup (const std::string& backup_name, std::vector<FileWithHash> filesToBackup, SQLite::Database& db, BackupFunction backup) {
    SQLite::Statement query{db, "INSERT OR REPLACE INTO backup_files(backup_name, filename, filehash) VALUES (?, ?, ?)"};
    query.bind(1, backup_name);
    for(const auto& fh: filesToBackup) {
		if (!backup(fh.file_path)) {
			continue; // keep trying for the failed files
		}
        std::cout << fh.file_path << " " << fh.hash << std::endl;
        query.reset();
        query.bind(2, fh.file_path);
        query.bind(3, fh.hash);
        query.exec();
    }
}

bool checkPreviousBackup(const std::string& backup_name, SQLite::Database& db) {
    SQLite::Statement query{db, "SELECT count(*) as num_matches FROM backup_files WHERE backup_name = ?"};
    query.bind(1, backup_name);
    int num_matches = 0;
    if (query.executeStep() == true) {
        num_matches = query.getColumn("num_matches").getInt();
    }
    return num_matches > 0;
}

#endif
