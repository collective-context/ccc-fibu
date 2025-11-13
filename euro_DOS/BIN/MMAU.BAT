@echo off
set aptmp=%app%
set itmp=%id%
set app=AU

if X%1==Xed edit \euro\bin\mmau.bat
if X%1==Xed goto end
echo ÿ
echo Erstellen der Maskenbibliothek: ¯\euro\EXE\%app%MASK.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=AU
  call mbm 210 211
  call mbm 4210 4211 4212 4214 4216 4218 4220 4300 4236 4280

set id=AU
rem  call mm 111 121 131 250 4250

set id=ST
rem  call mm 110 120 130

set id=SY
  call mm 036

:end
set app=%aptmp%
set id=%itmp%

