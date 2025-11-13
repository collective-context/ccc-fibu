@echo off
set aptmp=%app%
set itmp=%id%
set app=SY

if X%1==Xed edit \euro\bin\mmsy.bat
if X%1==Xed goto end
echo  
echo Erstellen der Maskenbibliothek: »\euro\EXE\%app%MASK.%EXT%«
echo  
ask "Starten j/n ",j,n
echo  
if errorlevel 2 goto end

set id=SY
  call mm 020 021 025 028 030 031 032 033 034 035 036 041 042 043 045 050 910
  call mm 4020 4041


:end
set app=%aptmp%
set id=%itmp%

