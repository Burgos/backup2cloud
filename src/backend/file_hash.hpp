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
#include <openssl/md5.h>

using Hash = std::string;

// https://stackoverflow.com/a/25173025/133707
template <typename T, int W>
struct FixedWidthVal
{
    FixedWidthVal(T v_) : v(v_) {}
    T v;
};

template <typename T, int W>
std::ostream& operator<< (std::ostream& ostr, const FixedWidthVal<T,W> &fwv)
{
   return ostr << std::setw(W) << std::setfill('0') << std::hex << std::nouppercase << fwv.v;
}

template <typename It>
std::string toHexStr(It begin, It end) {
   std::ostringstream result;
   std::copy(begin, end, std::ostream_iterator<FixedWidthVal<unsigned int, 2>>(result, ""));
   return result.str();
}

struct MD5_Context {
    MD5_Context () {
        MD5_Init (&context);
    }
    
    void Update (std::string_view data) {
        MD5_Update(&context, data.data(), data.size());
    }

    Hash GetFinalHash (){
        std::vector<unsigned char> hash(MD5_DIGEST_LENGTH, static_cast<unsigned char>(0));
        MD5_Final(hash.data(), &context);
        return toHexStr(hash.begin(), hash.end());
    }

private:
    MD5_CTX context;
};

Hash getFileHash(const std::string& path) {
    MD5_Context ctx;

    std::string buffer(16*1024*1024, static_cast<char>(0));
    std::ifstream input(path, std::ifstream::binary);
    while (!input.eof()) {
        input.read(buffer.data(), buffer.size());
        assert(input.gcount() <= buffer.size());
        ctx.Update({buffer.data(), static_cast<std::size_t>(input.gcount())});
    }
    return ctx.GetFinalHash();
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
