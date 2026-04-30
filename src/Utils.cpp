#include "Utils.hpp"
#include <iostream>
#include <cstdlib>


#ifdef _WIN32
    #define NULL_DEVICE "NUL"
#else 
    #define NULL_DEVICE "/dev/null"
#endif


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


