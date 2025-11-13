@echo off
set aptmp=%app%
set itmp=%id%
set app=TE

if NOT X%1==Xed goto start
if     X%1==Xed edit \euro\bin\mmte.bat
goto end

:start
echo  
echo Erstellen der Maskenbibliothek: »\euro\EXE\%app%MASK.%EXT%«
echo  
ask "Starten j/n ",j,n
echo  
if errorlevel 2 goto end

set id=TE
  call mm 6510 6520 6530 6610 6620 6630

set id=SY
  call mm 036

:end
set app=%aptmp%
set id=%itmp%

