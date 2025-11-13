@echo off
set aptmp=%app%
set itmp=%id%
set app=ST

if X%1==Xed edit \euro\bin\mmst.bat
if X%1==Xed goto end
echo ÿ
echo Erstellen der Maskenbibliothek: ¯\euro\EXE\%app%MASK.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=ST
  call mm     020 030 040 050 051 090 091 092 093 094 095 096
  call mm 110 111 120 121
  call mm 130 131 133 134 135 136 140 170 180 340 341

  call mm 4010 4011 4012 4110 4111 4112 4113 4120 4121 4122 4123 4130
  call mm 4140 4141 4143 4144 4145 4150 4151 4160 4170 4180
  call mm 4201 4203 4310 4312 4410

  call mm 730 750

set id=AU
rem    mm 131  4110 4111 4140 4141 4142 4143 4151 4160

set id=SY
  call mm 036

echo LKSMASK erstellen!!!
pause
xcopy \euro\exe\STMASK.043 \euro\exe\lksmask.*
set app=LKS

set id=LKS


:end
set app=%aptmp%
set id=%itmp%

