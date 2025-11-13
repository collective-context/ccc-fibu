@echo off
set aptmp=%app%
set itmp=%id%
set app=VS

if NOT X%1==Xed goto start
if     X%1==Xed edit \euro\bin\mmvs.bat
goto end

:start
echo  
echo Erstellen der Maskenbibliothek: »\euro\EXE\%app%MASK.%EXT%«
echo  
ask "Starten j/n ",j,n
echo  
if errorlevel 2 goto end

set id=ST
  call mm 110 4110 4111

set id=VS
  call mm 111 112 113

set id=SY
  call mm 036

:end
set app=%aptmp%
set id=%itmp%

