Write-Host "--- [ Cap2Cat: Windows Deployment ] ---" -ForegroundColor Cyan

$installDir = "$HOME\.cap2cat"
if (!(Test-Path $installDir)) { New-Item -ItemType Directory -Path $installDir }

$url = "https://github.com/YOUR_USER/Cap2Cat/releases/latest/download/cap2cat.exe"
$exePath = "$installDir\cap2cat.exe"

Write-Host "[+] Downloading Cap2Cat..." -ForegroundColor Yellow
Invoke-WebRequest -Uri $url -OutFile $exePath

$env:Path += ";$installDir"

Write-Host "[!] Success! You can now use 'cap2cat' in this terminal." -ForegroundColor Green
Write-Host "Usage: cap2cat <file.cap> <wordlist.txt>"
