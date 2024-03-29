#include <string>
#include <filesystem>
#include <chrono>
#include <date/date.h>
#include <iostream>

#include "pgp/encrypt.hpp"

namespace fs = std::filesystem;

fs::path createBackupDirectory(fs::path backup_path, std::string backup_name, bool is_full) {
    std::string date = date::format(" [%F %H%M%S]", std::chrono::system_clock::now());
    backup_path = backup_path.append(backup_name + date + (is_full? "(Full)" : "(Incremental)"));
    std::cout << "Creating backup directory: " << backup_path << std::endl;
    fs::create_directory(backup_path);
    return backup_path;
}

void deleteEmptyDirectory(fs::path dir_path) {
    std::cout << "Removing empty directory (no backups): " << dir_path.u8string() << std::endl;
    fs::remove(dir_path);
}

bool backup_file(fs::path backup_directory, const fs::path& root, const fs::path& file_path) {
	try {
		fs::path relative = fs::relative(file_path, root).remove_filename();
		std::cout << "Backing up: " << file_path << "\n";
		fs::path dest_dir = backup_directory / relative;
		fs::create_directories(dest_dir);
		dest_dir /= file_path.filename();
		fs::copy_file(file_path, dest_dir);
        encryptFile("", dest_dir.u8string());
		return true;
	}
	catch (std::exception& ex) {
		std::wcerr << "ERROR: Could not backup " << file_path.wstring() << " : " << ex.what() << std::endl;
		return false;
	}
}
