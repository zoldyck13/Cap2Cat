#!/bin/bash


INSTALL_DIR="$HOME/.local/bin"
mkdir -p "$INSTALL_DIR"

echo -e "\e[36m[*] Downloading Cap2Cat resources...\e[0m"


BASE_URL="https://github.com/zoldyck13/Cap2Cat/releases/tag/v1.2.3"

curl -L "$BASE_URL/Cap2Cat" -o "$INSTALL_DIR/Cap2Cat"
curl -L "$BASE_URL/main.py" -o "$INSTALL_DIR/main.py"
curl -L "$BASE_URL/cap2cat_ai_model.pth" -o "$INSTALL_DIR/cap2cat_ai_model.pth"

chmod +x "$INSTALL_DIR/Cap2Cat"


cd "$INSTALL_DIR"
if [ ! -d "venv" ]; then
    echo -e "\e[33m[*] Creating Python Virtual Environment...\e[0m"
    python3 -m venv venv
    ./venv/bin/pip install torch numpy --quiet
fi


if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo "export PATH=\$PATH:$INSTALL_DIR" >> ~/.bashrc
    source ~/.bashrc
fi

echo -e "\e[32m[+] Installation successful! Run 'Cap2Cat' to begin.\e[0m"
