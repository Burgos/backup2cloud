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
    FileWithHash(const fs::path& path):
        file_path(path.u8string()), hash(getFileHash(path)) {}

    std::string file_path;
    Hash hash;
};

std::vector<FileWithHash> calculateHashes (const fs::path& path) {
	fs::recursive_directory_iterator it{ path };
    std::vector<FileWithHash> results;
    for (auto& p: it) {
        if (p.is_directory()) {
            continue;
        }
        results.push_back(FileWithHash{ p.path().wstring() });
    }

    return results;
}

std::vector<FileWithHash> calculateHashes (std::string path) {
    fs::path file_path{path};
    return calculateHashes(fs::path{ path });
}

#endif // BACKUP2CLOUD_BACKEND_TRAVERSER_H
