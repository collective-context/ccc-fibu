@echo off
cls
echo ÿ
echo ÿÚÄÄÄÄÄÄÄÄÄÄÄ¿ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo  ³euroSOFT-  ³	     euroSOFT-WARE Servicetelefon: 03116/27 5 11
echo  ³MODULsystem³	 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo  ³        3  ³
echo ÿ³     2 ²²² ³
echo ÿ³  1 ²²²²²² ³	 ... euroSOFT-WARE
echo ÿ³ ²²²²²²²²² ³	 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿÀÄÄÄÄÄÄÄÄÄÄÄÙ			  ... Software-Engineering aus ™sterreich!
echo ÿ
echo  Ú Wie kann ich das Installationsprogramm abbrechen?
echo  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo  Sie k”nnen jederzeit mit den Tasten [Strg]+[C] die Installation abbrechen!
echo  Nach einem Abbruch stehen Sie im DOS-Betriebssytem und k”nnen das Men
echo  wieder durch die Eingabe von [START]+[Ù] starten.
echo ÿ
echo ÿ
echo  Ú Wie wird das Installationsprogramm gestartet?
echo  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo  Wenn Sie die Frage nach dem Diskettenlaufwerk mit [A] oder [B] beantwortet
echo  haben, wird das Installationsprogramm automatisch gestartet. ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo ÿ
ask " Programme vom Diskettenlaufwerk [A] oder [B] einkopieren? ",ab
cls
if errorlevel 2 B:INSTALL
if errorlevel 1 A:INSTALL

