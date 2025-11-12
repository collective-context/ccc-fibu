@echo off
set aptmp=%app%
set itmp=%id%
set app=ST

if X%1==Xed edit \euro\bin\mfst.bat
echo ÿ
if X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\%app%FORM.%EXT%®
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\D%MAN%\%app%FORM.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=ST
  call mf 0100 0101 0110 1100 1101 1102 1103 1104 1105 1106 1107 1110
  call mf 1200 1201 1202 1203 1207 1300 1400 1401 1402 1403 1404 1451 1460
  call mf 1700 1800 2100 4201 4202 4203
  
set id=AU
REM    mf 2500
rem    mf 1400 1401 1402 1403 1451 1460

set id=VS
  call mf 1111

echo LKSFORM erstellen!!!
pause
xcopy \euro\dat\STFORM.043 \euro\dat\LKSFORM.*
set app=LKS

set id=LKS
rem    mf 1400

:end
set app=%aptmp%
set id=%itmp%

