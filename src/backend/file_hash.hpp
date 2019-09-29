#ifndef BACKUP2CLOUD_BACKEND_FILE_HASH_H
#define BACKUP2CLOUD_BACKEND_FILE_HASH_H

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <openssl/md5.h>

using Hash = std::vector<unsigned char>;

struct MD5_Context {
    MD5_Context () {
        MD5_Init (&context);
    }
    
    void Update (std::string data) {
        MD5_Update(&context, data.data(), data.size());
    }

    Hash GetFinalHash (){
        Hash hash(MD5_DIGEST_LENGTH);
        MD5_Final(hash.data(), &context);
        return hash;
    }

private:
    MD5_CTX context;
};

Hash getFileHash(const std::string& path) {
    MD5_Context ctx;
    return ctx.GetFinalHash();
}

#endif // BACKUP2CLOUD_BACKEND_FILE_HASH_H
