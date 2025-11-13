@echo off
set aptmp=%app%
set itmp=%id%
set app=FI

if X%1==Xed edit \euro\bin\mffi.bat
if X%1==Xed goto end
echo ÿ
if X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\%app%FORM.%EXT%®
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\D%MAN%\%app%FORM.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=FI
REM    mf 2110 4180
  call mf 0300 0301 0302 
  call mf 1100 1101 1110 1200 1201 1210 1300 1400 1500 1505 1510 1550
  call mf 1600 1700 1710
  call mf 2100 2101 2105 2110 2500 2505 2540
  call mf 3100 3102 3105 3110 3200 3220 4100 4105 4106 4110 
  call mf 4120 4140 4150 4160 4400

echo LKFFORM erstellen!!!
pause
xcopy \euro\dat\FIFORM.043 \euro\dat\LKFFORM.*
set app=LKF

set id=LKF
  call mf 2500

:end
set app=%aptmp%
set id=%itmp%

