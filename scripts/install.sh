#!/bin/bash

echo -e "\e[36m--- [ Cap2Cat: Linux Deployment ] ---\e[0m"

if ! command -v hashcat &> /dev/null; then
    echo -e "\e[33m[!] Hashcat not found. Installing via pacman...\e[0m"
    sudo pacman -S --noconfirm hashcat hcxtools
fi

INSTALL_DIR="$HOME/.local/bin"
mkdir -p "$INSTALL_DIR"
URL="https://github.com/YOUR_USER/Cap2Cat/releases/latest/download/cap2cat"

echo -e "\e[33m[+] Downloading Cap2Cat Binary...\e[0m"
curl -L $URL -o "$INSTALL_DIR/cap2cat"
chmod +x "$INSTALL_DIR/cap2cat"

if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo "export PATH=\$PATH:$INSTALL_DIR" >> ~/.bashrc
    echo -e "\e[32m[+] Added to PATH. Please restart your terminal or run 'source ~/.bashrc'\e[0m"
fi

echo -e "\e[32m[***] Installation Complete! Try running 'cap2cat'\e[0m"
