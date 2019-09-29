#ifndef BACKUP2CLOUD_BACKEND_TRAVERSER_H
#define BACKUP2CLOUD_BACKEND_TRAVERSER_H

#include <string>
#include <vector>
#include <filesystem>
#include "file_hash.hpp"

namespace fs = std::filesystem;

/// Traverses the given directory recursively and calculates
/// the hashes of all files found
struct FileWithHash {
    FileWithHash(const std::string& path):
        file_path(path), hash(getFileHash(path)) {}

    std::string file_path;
    Hash hash;
};

std::vector<FileWithHash> calculateHashes (fs::recursive_directory_iterator it) {
    std::vector<FileWithHash> results;
    for (auto& p: it) {
        if (p.is_directory()) {
            continue;
        }
        results.emplace_back(p.path());
    }

    return results;
}

std::vector<FileWithHash> calculateHashes (std::string path) {
    fs::path file_path{path};
    return calculateHashes(fs::recursive_directory_iterator{file_path});
}

#endif // BACKUP2CLOUD_BACKEND_TRAVERSER_H
