#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
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
    
    if (firstArg == "--gen-only") {
    if (argc < 3) {
        std::cout << "Usage: cap2cat --gen-only <\"keyword\"> <\"range\"> <\"temp\"> <\"output name\"> \n";
        return 1;
    }
    std::string keyword = argv[2];
    std::string range = argv[3];
    std::string temp = argv[4];
    std::string outputName = argv[5];
    
    int rangeInt = std::stoi(range);
    int tempInt = std::stoi(temp);
    
    Utils::runAIPredictor(keyword, rangeInt, tempInt=1.1, outputName);
    std::cout << "[+] Wordlist generated successfully. You can find it in " << outputName <<"\n";
    return 0; 
}
    
    std::string inputCap;
    std::string wordlist;

    if (firstArg == "--predict") {
        if (argc < 4) {
            std::cout << "Usage: cap2cat --predict <\"keyword\"> <handshake.cap>\n";
            return 1;
        }
        std::string keyword = argv[2];
        inputCap = argv[3];
        wordlist = "ai_generated_list.txt";

        
        if (!Utils::runAIPredictor(keyword, 1000, 1.1, wordlist)) {
            std::cerr << "[!] Error: AI Prediction engine failed.\n";
            return 1;
        }
    } else {
        
        if (argc < 3) {
            std::cout << "Usage: cap2cat <handshake.cap> <wordlist.txt>\n";
            return 1;
        }
        inputCap = argv[1];
        wordlist = argv[2];
    }

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


    std::atomic<bool> keepRunning(true);
    std::string msg = "Cracking in progess... (Press 's' for status)";

    std::thread animationThread([&]() {
            Utils::showLoadingAnimation(std::ref(keepRunning), msg);
        });



    std::cout << "[+] Starting Hashcat with Mode 22000...\n";
    
    std::string crackCmd = "hashcat -m 22000 " + outputHash + " " + cleanWordlist + " --status --status-timer=5";
    int result = std::system(crackCmd.c_str());

    keepRunning = false;
    if (animationThread.joinable()) {
        animationThread.join();
    }

    if (result == 0 || result == 1) { // 0: كسر، 1: انتهى الـ potfile
        std::string password = Utils::getCrackedPassword(outputHash);
        if (!password.empty()) {
            std::string ssid = fs::path(cleanCap).stem().string();
            Utils::saveToHistory(ssid, password);
            std::cout << "\n[***] Success! SSID: " << ssid << " | Password: " << password << "\n";
        } else {
            std::cout << "\n[!] Hashcat finished, but no password found in potfile.\n";
        }
    } else {
        std::cerr << "\n[!] Hashcat was interrupted or failed.\n";
    }

    return 0;
}
