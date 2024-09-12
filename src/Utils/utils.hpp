#pragma once
#include <string>
#include <chrono>





namespace utils {

    static inline long getEpochMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }


    int getConsoleWidth();

    std::string formatChar(char c, ulong col, bool useColor = false);
    std::string shortenInteger(ulong n);
    std::string formatMilliseconds(long n);

    std::string readFile(std::ifstream &f);




    struct PathCheckResult {
        bool exists   = false;
        bool isDir    = false;
        bool canRead  = false;
        bool canWrite = false;
        bool canExec  = false;
    };
    PathCheckResult checkPath(std::string const &path);
}