#include "Utils.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>
#include <cstdio>

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
    std::string cmd = "hashcat -m 22000 --show \"" + hashFile + "\" 2>/dev/null";
    char buffer[256];
    std::string result = "";
    
#ifdef _WIN32
    FILE* pipe = _popen(cmd.c_str(), "r");
#else
    FILE* pipe = popen(cmd.c_str(), "r");
#endif

    if (!pipe) return "";
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    if (!result.empty()) {
        while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
            result.pop_back();
        }
        
        size_t lastColon = result.find_last_of(':');
        if (lastColon != std::string::npos) {
            return result.substr(lastColon + 1);
        }
    }
    
    return "";
}



void Utils::showLoadingAnimation(std::atomic<bool>& keepRunning, const std::string& message) {
    const std::vector<std::string> spinner = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int i = 0;
    auto startTime = std::chrono::steady_clock::now();

    while (keepRunning) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        
        std::cout << "\r\033[35m" << spinner[i % spinner.size()] << "\033[0m " 
                  << message << " [Time: " << elapsed << "s] " << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        i++;
    }
    std::cout << "\r" << std::string(60, ' ') << "\r"; 
}

bool Utils::runAIPredictor(const std::string& keyword, int count, float temp, const std::string& outputFile) {
    std::string venvPython;
    std::string venvPath = "venv";


    #ifdef _WIN32
        venvPython = venvPath + "\\Scripts\\python.exe";
        std::string pipCmd = venvPath + "\\Scripts\\pip.exe install torch numpy --quiet";
    #else
        venvPython = "./" + venvPath + "/bin/python3";
        std::string pipCmd = "./" + venvPath + "/bin/pip install torch numpy --quiet";
    #endif


    if (!fs::exists(venvPython)) {
        std::cout << "[*] First-time setup: Creating virtual environment...\n";
        

        if (std::system("python3 -m venv venv") != 0 && std::system("python -m venv venv") != 0) {
            std::cerr << "[!] Critical Error: Could not create venv. Is Python installed?\n";
            return false;
        }

        std::cout << "[*] Installing dependencies (torch)... This may take a minute.\n";

        if (std::system(pipCmd.c_str()) != 0) {
            std::cerr << "[!] Error: Failed to install torch inside venv.\n";
            return false;
        }
        std::cout << "[+] Setup complete!\n";
    }


    std::string command = venvPython + " main.py --predict-model --keyword \"" + keyword + 
                          "\" --count " + std::to_string(count) + 
                          " --temp " + std::to_string(temp) + 
                          " --output " + outputFile + " --leet";

    std::cout << "[*] AI Predictor is running...\n";
    int result = std::system(command.c_str());
    
    return (result == 0);
}
