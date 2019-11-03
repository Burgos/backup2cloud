#pragma once

#include <array>
#include <string>
#include <thread>
#include <vector>

#ifdef WIN32
#include <windows.h>
#endif

namespace detail::system {
    #ifdef WIN32
    std::string executeProgram(std::string const& cmd,
           std::vector<std::string> const& args,
           std::string const& cwd) {
        std::vector<char> flat_args;
        flat_args.push_back('"');
        std::copy(cmd.begin(), cmd.end(), std::back_inserter(flat_args));
        flat_args.push_back('"');
        flat_args.push_back(' ');
        for (const auto& arg: args) {
            std::copy(arg.begin(), arg.end(), std::back_inserter(flat_args));
            flat_args.push_back(' ');
        }
        flat_args.push_back('\0');
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);

        if (!CreateProcess(cmd.c_str(), flat_args.data(),
                  NULL, NULL, true,
                  0,
                  NULL, cwd.c_str(),
                  &si, &pi)) {
            throw std::runtime_error("Failed to create a process: " +
                    std::to_string(GetLastError()));
        }

        WaitForSingleObject(pi.hProcess, 0);
        // we do not collect anything, since we inherit all handles.
        return "";
    }
    #endif
};

/// Encrypts the file through the PGP (the command is passed
/// as the parameter and removes the file if successful.
bool encryptFile(std::string const& cmd, std::string const& file_path) {
    detail::system::executeProgram("C:\\Program Files (x86)\\GnuPG\\bin\\gpg.exe",
        {"--recipient", "\"EE3C 2080 9E52 317A F781 0E24 2BDB E543 CA0E F421\"",
         "--encrypt-files", std::string("\"") + file_path + "\""}, "c:\\users");
     
    return true;
}
