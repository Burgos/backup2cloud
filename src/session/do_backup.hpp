#include <string>
#include <filesystem>
#include <chrono>
#include <date/date.h>
#include <iostream>

namespace fs = std::filesystem;

fs::path createBackupDirectory(fs::path backup_path, std::string backup_name, bool is_full) {
    std::string date = date::format(" [%F %T]", std::chrono::system_clock::now());
    backup_path = backup_path.append(backup_name + date + (is_full? "(Full)" : "(Incremental)"));
    std::cout << "Creating backup directory: " << backup_path << std::endl;
    fs::create_directory(backup_path);
    return backup_path;
}
