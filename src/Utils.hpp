#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <filesystem>

namespace fs = std::filesystem;


class Utils{
    public:
        static bool isToolInstalled(const std::string& toolName);
        
        static std::string sanitizePath(const std::string& rawPath);

        static bool isValidHandshakeFile(const std::string& filePath);
};

#endif
