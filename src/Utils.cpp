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
    std::string home = getenv("HOME");
    std::string path = home + "/.cap2cat_history";
    
    std::ifstream check(path);
    std::string line;
    while (std::getline(check, line)) {
        if (line == (ssid + ":" + password)) return; 
    }
    check.close();

    std::ofstream file(path, std::ios::app);
    file << ssid << ":" << password << "\n";
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


std::string Utils::getCrackedPassword(const std::string& hashFile) {
    std::ifstream hFile(hashFile);
    std::string firstLine, targetHash;
    if (std::getline(hFile, firstLine)) {
        if (firstLine.length() >= 32)
            targetHash = firstLine.substr(0, 32); 
    }
    hFile.close();

    if (targetHash.empty()) return "";

    const char* homeEnv = std::getenv("HOME");
    std::string home = (homeEnv != nullptr) ? std::string(homeEnv) : "";

    std::vector<std::string> potPaths = {
        home + "/.local/share/hashcat/hashcat.potfile",
        home + "/.hashcat/hashcat.potfile",
        "hashcat.potfile"
    };

    for (const auto& path : potPaths) {
        std::ifstream pot(path);
        if (!pot.is_open()) continue;

        std::string line;
        while (std::getline(pot, line)) {
            // إذا وجدنا الهاش في السطر
            if (line.find(targetHash) != std::string::npos) {
                size_t lastColon = line.find_last_of(':');
                if (lastColon != std::string::npos) {
                    return line.substr(lastColon + 1); 
                }
            }
        }
    }
    return "";
}
