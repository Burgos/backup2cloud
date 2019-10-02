#ifndef BACKUP2CLOUD_BACKEND_FILE_HASH_H
#define BACKUP2CLOUD_BACKEND_FILE_HASH_H

#include <fstream>
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <iterator>
#include <iomanip>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;
using Hash = std::string;

template <typename It>
std::string toHexStr(It begin, It end) {
   std::ostringstream result;
   std::copy(begin, end, std::ostream_iterator<uintmax_t>(result, ""));
   return result.str();
}

struct FileStat_Context {
    static Hash GetFinalHash (const fs::path& path) {
        fs::path fpath{path};
        std::vector<uintmax_t> vec =
            {fs::file_size(fpath),
             static_cast<long unsigned int>(fs::last_write_time(fpath).time_since_epoch().count())};
        return toHexStr(vec.begin(), vec.end());
    }
};

Hash getFileHash(const fs::path& path) {
    return FileStat_Context::GetFinalHash(path);
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
