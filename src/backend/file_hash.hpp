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
#include <chrono>
#include <date/date.h>
#include <date/tz.h>
#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace fs = std::filesystem;
using Hash = std::string;

// Gets the seconds since sys clock epoch.
// Used to workaround the file_clock issue
// in latest GCC (C++20 should bring to_sys_time which works
// with this clock. Right now even date library can't accept it)
time_t getFileTime (const fs::path& path) {
#ifdef WIN32
    return std::chrono::duration_cast<std::chrono::seconds>(fs::last_write_time(fpath).time_since_epoch()).count();
#else
    struct stat s;
    std::string str_path = path.u8string();
    if (stat(str_path.c_str(), &s) == -1) {
        throw std::runtime_error("Could not stat the file.");
    }
    return s.st_mtime;
#endif
}

template <typename It>
std::string toHexStr(It begin, It end) {
   std::ostringstream result;
   std::copy(begin, end, std::ostream_iterator<uintmax_t>(result, ""));
   return result.str();
}

struct FileStat_Context {
    static Hash GetFinalHash (const fs::path& path) {
        fs::path fpath{path};
 
        unsigned long int sec_since_epoch = getFileTime(path);
        std::vector<uintmax_t> vec =
            {fs::file_size(fpath),
             sec_since_epoch};
        return toHexStr(vec.begin(), vec.end());
    }
};

Hash getFileHash(const fs::path& path) {
    return FileStat_Context::GetFinalHash(path);
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
