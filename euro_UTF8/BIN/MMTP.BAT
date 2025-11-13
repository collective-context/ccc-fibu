@echo off
set aptmp=%app%
set itmp=%id%
set app=TP

if X%1==Xed edit \euro\bin\mmfi.bat
echo  
echo Erstellen der Maskenbibliothek: »\euro\EXE\%app%MASK.%EXT%«
echo  
ask "Starten j/n ",j,n
echo  
if errorlevel 2 goto end

set id=TP
  call mm 110 111 112 113 210 211

set id=SY
  call mm 036

:end
set app=%aptmp%
set id=%itmp%

