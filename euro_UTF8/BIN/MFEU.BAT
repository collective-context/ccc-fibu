@echo off
set aptmp=%app%
set itmp=%id%
set app=EU

if X%1==Xed edit \euro\bin\mfeu.bat
if X%1==Xed goto end
echo  
if X%MAN%==X echo Erstellen der Formularbibliothek: »\euro\DAT\%app%FORM.%EXT%«
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: »\euro\DAT\D%MAN%\%app%FORM.%EXT%«
echo  
ask "Starten j/n ",j,n
echo  
if errorlevel 2 goto end

set id=EU
  call mf 5000 5100 5110 5200 5210 5500
  call mf 6100 6110 6200 6210 6300 6310 6400 6410 6500 6510 6600 6610
  call mf 7000

set id=VS
  call mf 1100 1101 1102 1111

set id=ST
  call mf 1105 1106

:end
set app=%aptmp%
set id=%itmp%



