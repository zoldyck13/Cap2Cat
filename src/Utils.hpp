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


        static void saveToHistory(const std::string& ssid, const std::string& password);
        static void showHistory();
        static void searchHistory(const std::string& ssid);
        static std::string getSSIDFromCap(const std::string& capPath); 
        static std::string getCrackedPassword(const std::string& hashFile); 
};

#endif
