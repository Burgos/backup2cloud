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

template <typename Context>
Hash getFileHash(const std::string& path) {
    Context ctx;

    std::string buffer(64*1024*1024, static_cast<char>(0));
    std::ifstream input(path, std::ifstream::binary);
    while (!input.eof()) {
        input.read(buffer.data(), buffer.size());
        assert(input.gcount() <= buffer.size());
        ctx.Update({buffer.data(), static_cast<std::size_t>(input.gcount())});
    }
    return ctx.GetFinalHash();
}

template <>
Hash getFileHash<FileStat_Context>(const std::string& path) {
    return FileStat_Context::GetFinalHash(path);
}

Hash getFileHash(const std::string& path) {
    return getFileHash<FileStat_Context>(path);
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
