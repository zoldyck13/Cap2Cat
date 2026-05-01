# 🐈‍⬛ Cap2Cat 
**AI-Powered WPA/WPA2 Handshake Cracking Suite**

Cap2Cat is a professional, cross-platform C++ tool designed to streamline the process of cracking WPA/WPA2 handshakes. It bridges the gap between traditional wordlist attacks and modern AI by predicting potential passwords based on user-provided keywords.

---


##  Quick Installation

### Linux (One-Liner)
```bash
curl -sSL [https://raw.githubusercontent.com/zoldyck13/Cap2Cat/main/scripts/install.sh](https://raw.githubusercontent.com/zoldyck13/Cap2Cat/main/scripts/install.sh) | bash
```
### Windows (PowerShell)
```PowerShell
irm [https://raw.githubusercontent.com/zoldyck13/Cap2Cat/main/scripts/install.ps1](https://raw.githubusercontent.com/zoldyck13/Cap2Cat/main/scripts/install.ps1) | iex
```

## Usage
Run ``cap2cat`` without arguments to see the full help menu.

**1. Standard Crack**

Manual attack using an existing wordlist:
```bash
cap2cat capture.cap rockyou.txt
```

**2. AI-Powered Attack**

Predict passwords based on a person's name or a specific keyword:

```bash
cap2cat --predict "Ali Saad" handshake.cap
```

**3. AI Generation**

Generate a targeted wordlist without starting the cracking process:

```bash
cap2cat --gen-only "Admin" 5000 1.2 wordlist.txt
```

**4. Database Management**

```bash
cap2cat --show              # View all cracked handshakes
cap2cat --search "HomeWiFi" # Search for a specific SSID
```
___

## Requirements
The tool handles most dependencies automatically, but ensure you have:

- ``hashcat`` & ``hcxtools`` (for cracking)

- ``python3`` (for the AI engine)

- ``cmake`` & ``gcc/mingw`` (if building from source)


___
## License & Disclaimer

This tool is for educational and authorized security testing purposes only. The author is not responsible for any misuse or damage caused by this program.
