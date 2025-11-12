@echo off
set aptmp=%app%
set itmp=%id%
set app=EU

if X%1==Xed edit \euro\bin\mmeu.bat
if X%1==Xed goto end
echo ÿ
echo Erstellen der Maskenbibliothek: ¯\euro\EXE\%app%MASK.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=EU
  call mm 110 111 112 113 900 901 910 995
  call mm 4110 4111 4200 4210 4900

set id=SY
  call mm 036 041

:end
set app=%aptmp%
set id=%itmp%

