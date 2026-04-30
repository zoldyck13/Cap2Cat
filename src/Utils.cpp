#include "Utils.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#ifdef _WIN32
    #define NULL_DEVICE "NUL"
#else 
    #define NULL_DEVICE "/dev/null"
#endif

namespace fs = std::filesystem;


bool Utils::isToolInstalled(const std::string& toolName){
    std::string checkCmd = toolName + " --version > " + NULL_DEVICE + " 2>&1";
    return (std::system(checkCmd.c_str()) == 0);
}

std::string Utils::sanitizePath(const std::string& rawPath){
    try{
        fs::path p(rawPath);
        if (fs::exists(p)){
            return fs::absolute(p).string();
        }
    } catch(...) {}
    return "";
}

bool Utils::isValidHandshakeFile(const std::string& filePath){
    fs::path p(filePath);
    std::string ext = p.extension().string();
    for(auto &c : ext) c = std::tolower(c);

    return (ext == ".cap" || ext == ".pcap" || ext == ".pcapng");
}

void Utils::saveToHistory(const std::string& ssid, const std::string& password) {
    std::string homeDir = getenv("HOME");
    std::ofstream historyFile(homeDir + "/.cap2cat_history", std::ios::app);
    if (historyFile.is_open()) {
        historyFile << ssid << ":" << password << std::endl;
        historyFile.close();
    }
}

void Utils::showHistory() {
    std::string homeDir = getenv("HOME");
    std::ifstream historyFile(homeDir + "/.cap2cat_history");
    std::string line;
    std::cout << "\n--- [ Cap2Cat Cracked History ] ---\n";
    if (!historyFile.is_open()) {
        std::cout << "No history found yet.\n";
        return;
    }
    while (std::getline(historyFile, line)) {
        std::cout << "󰖩 " << line << std::endl;
    }
}

void Utils::searchHistory(const std::string& ssid) {
    std::string homeDir = getenv("HOME");
    std::ifstream historyFile(homeDir + "/.cap2cat_history");
    std::string line;
    bool found = false;
    while (std::getline(historyFile, line)) {
        if (line.find(ssid + ":") == 0) {
            std::cout << "\n[!] Match Found: " << line << std::endl;
            found = true;
        }
    }
    if (!found) std::cout << "\n[!] No records for SSID: " << ssid << std::endl;
}

