#include <iostream>
#include "backend/traverser.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        for (const auto& fh: calculateHashes(argv[i])) {
            std::cout << fh.file_path << "\t" << fh.hash << std::endl;
            std::flush(std::cout);
        }
    }
    return 0;
} 
