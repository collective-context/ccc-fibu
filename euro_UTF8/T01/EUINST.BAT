@echo off
cls
echo  
echo  ┌───────────┐ ────────────────────────────────────────────────────────
echo  │euroSOFT-  │	     euroSOFT-WARE Servicetelefon: 03116/27 5 11
echo  │MODULsystem│	 ─────────────────────────────────────────────────────────
echo  │        3  │
echo  │     2 ▓▓▓ │
echo  │  1 ▓▓▓▓▓▓ │	 ... euroSOFT-WARE
echo  │ ▓▓▓▓▓▓▓▓▓ │	 ─────────────────────────────────────────────────────────
echo  └───────────┘			  ... Software-Engineering aus Österreich!
echo  
echo  ┌ Wie kann ich das Installationsprogramm abbrechen?
echo  └──────────────────────────────────────────────────
echo  Sie können jederzeit mit den Tasten [Strg]+[C] die Installation abbrechen!
echo  Nach einem Abbruch stehen Sie im DOS-Betriebssytem und können das Menü
echo  wieder durch die Eingabe von [START]+[┘] starten.
echo  
echo  
echo  ┌ Wie wird das Installationsprogramm gestartet?
echo  └──────────────────────────────────────────────
echo  Wenn Sie die Frage nach dem Diskettenlaufwerk mit [A] oder [B] beantwortet
echo  haben, wird das Installationsprogramm automatisch gestartet. ──────────────┐
echo  ───────────────────────────────────────────────────────────────────────────┘
echo  
ask " Programme vom Diskettenlaufwerk [A] oder [B] einkopieren? ",ab
cls
if errorlevel 2 B:INSTALL
if errorlevel 1 A:INSTALL

