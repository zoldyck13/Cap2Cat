#include <iostream>
#include <string>
#include <vector>
#include "Utils.hpp"


#ifdef _WIN32
    #define NULL_DEVICE "NUL"
#else
    #define NULL_DEVICE "/dev/null"
#endif


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  cap2cat <handshake.cap> <wordlist.txt>  \n";
        std::cout << "  cap2cat --show                          \n";
        std::cout << "  cap2cat --search <SSID>                 \n";
        return 1;
    }


    std::string firstArg = argv[1];

    
    if (firstArg == "--show") {
        Utils::showHistory();
        return 0;
    }
    
    if (firstArg == "--search") {
        if (argc < 3) {
            std::cout << "Usage: cap2cat --search <SSID>\n";
            return 1;
        }
        Utils::searchHistory(argv[2]);
        return 0;
    }

    if (argc < 3) {
        std::cout << "Usage: cap2cat <handshake.cap> <wordlist.txt>\n";
        return 1;
    }

    std::string inputCap = argv[1];
    std::string wordlist = argv[2];

    std::cout << "--- [ Cap2Cat: Starting Deployment ] ---\n";

    if (!Utils::isToolInstalled("hcxpcapngtool")) {
        std::cerr << "[!] Critical error: hcxpcapngtool missing. Check your PATH.\n";
        return 1;
    }

    if (!Utils::isToolInstalled("hashcat")){
        std::cerr << "[!] Error: hcxpcapngtool (hcxtools) is not installed.\n";
        return 1;
    }

    std::string cleanCap = Utils::sanitizePath(inputCap);
    std::string cleanWordlist = Utils::sanitizePath(wordlist);

    if (cleanCap.empty() || cleanWordlist.empty()) {
        std::cerr << "[!] Error: Invalid file paths provided.\n";
        return 1;
    }

    std::string outputHash = cleanCap + ".hc22000";
    std::cout << "[+] Converting: " << fs::path(cleanCap).filename() << " -> " << fs::path(outputHash).filename() << "\n";
    
    std::string convertCmd = "hcxpcapngtool -o " + outputHash + " " + cleanCap + " > " + NULL_DEVICE + " 2>&1";
    
    if (std::system(convertCmd.c_str()) != 0) {
        std::cerr << "[!] Conversion failed. The file might not contain a valid handshake.\n";
        return 1;
    }

    std::cout << "[+] Starting Hashcat with Mode 22000...\n";
    
    std::string crackCmd = "hashcat -m 22000 " + outputHash + " " + cleanWordlist;

    int result = std::system(crackCmd.c_str());

    if (result == 0) {
        std::cout << "\n[***] Success! Check Hashcat output for the password.\n";
    } else {
        std::cerr << "\n[!] Hashcat finished with an error or was stopped.\n";
    }

    return 0;
}
