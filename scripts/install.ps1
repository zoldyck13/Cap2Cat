$installDir = "$HOME\.cap2cat"
if (!(Test-Path $installDir)) { New-Item -ItemType Directory -Path $installDir }

$baseUrl = "https://github.com/zoldyck13/Cap2Cat/releases/tag/v1.2.3"

Write-Host "[*] Downloading Cap2Cat for Windows..." -ForegroundColor Cyan
Invoke-WebRequest -Uri "$baseUrl/cap2cat.exe" -OutFile "$installDir\cap2cat.exe"
Invoke-WebRequest -Uri "$baseUrl/main.py" -OutFile "$installDir\main.py"
Invoke-WebRequest -Uri "$baseUrl/cap2cat_ai_model.pth" -OutFile "$installDir\cap2cat_ai_model.pth"


cd $installDir
if (!(Test-Path "venv")) {
    Write-Host "[*] Initializing AI Environment..." -ForegroundColor Yellow
    python -m venv venv
    .\venv\Scripts\pip install torch numpy --quiet
}


$path = [Environment]::GetEnvironmentVariable("Path", "User")
if ($path -notlike "*$installDir*") {
    [Environment]::SetEnvironmentVariable("Path", "$path;$installDir", "User")
}

Write-Host "[+] Installation Complete! Restart terminal and type 'cap2cat'." -ForegroundColor Green
