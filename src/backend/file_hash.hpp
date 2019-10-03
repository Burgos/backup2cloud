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
#else
	#include <windows.h>
#endif

namespace fs = std::filesystem;
using Hash = std::string;

// Gets the seconds since sys clock epoch.
// Used to workaround the file_clock issue
// in latest GCC (C++20 should bring to_sys_time which works
// with this clock. Right now even date library can't accept it)
unsigned long long getFileTime (const fs::path& path) {
#ifdef WIN32
	HANDLE hFile;
	FILETIME write_time;
	std::string str_path = path.u8string();
	hFile = CreateFile(str_path.c_str(), GENERIC_READ, FILE_SHARE_READ, 
						nullptr, OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL,
						nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Could not open the file.");
	}
	if (GetFileTime(hFile, nullptr, nullptr, &write_time) == 0) {
		throw std::runtime_error("Could not stat the file.");
	}
	CloseHandle(hFile);
	ULARGE_INTEGER large_ticks;
	large_ticks.LowPart = write_time.dwLowDateTime;
	large_ticks.HighPart = write_time.dwHighDateTime;
	unsigned long long ticks = *reinterpret_cast<unsigned long long*>(&large_ticks);
    return ticks / 10000000 - 11644473600LL;
#else
    struct stat s;
    std::string str_path = path.u8string();
    if (stat(str_path.c_str(), &s) == -1) {
        throw std::runtime_error("Could not stat the file.");
    }
    return static_cast<unsigned long long>(s.st_mtime);
#endif
}

template <typename It>
std::string toHexStr(It begin, It end) {
   std::ostringstream result;
   std::copy(begin, end, std::ostream_iterator<unsigned long long>(result, ""));
   return result.str();
}

struct FileStat_Context {
    static Hash GetFinalHash (const fs::path& path) {
        fs::path fpath{path};
 
        unsigned long long sec_since_epoch = getFileTime(path);
        std::vector<unsigned long long> vec =
            {fs::file_size(fpath),
             sec_since_epoch};
        return toHexStr(vec.begin(), vec.end());
    }
};

Hash getFileHash(const fs::path& path) {
    return FileStat_Context::GetFinalHash(path);
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
