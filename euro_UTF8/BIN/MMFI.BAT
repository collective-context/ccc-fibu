@echo off
set aptmp=%app%
set itmp=%id%
set app=FI

if NOT X%1==Xed goto start
if     X%1==Xed edit \euro\bin\mmfi.bat
goto end

:start
echo ÿ
echo Erstellen der Maskenbibliothek: ¯\euro\EXE\%app%MASK.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=FI
  call mm 015 020 030 110 111 112 113 114 115 120 121 122 123 124 125
  call mm 130 131 132 134 135 136 137
  call mm 140 145 146 150 151 156 160 161 165 170 171
  call mm 210 211 212 220 221 250 251 252 254 260
  call mm 310 311 312 320 321 322 330 331 332 340 345 346 347


  call mm 4030 4110 4111 4112 4114 4115 4116 4120 4121 4122 4124 4125 4126
  call mm 4130 4132 4134 4135 4136 4137 4140 4141 4145 4150 4151 4152 4155
  call mm 4160 4170 4171 4210 4211 4230 4232 4250 4254
  call mm 4310 4311 4312 4315 4320 4321 4322 4330 4340 4342 
  call mm 4350 4351 4352 4360 4432 9210 9312

set id=ST

set id=SY
  call mm 036

echo LKFMASK erstellen!!!
pause
xcopy \euro\exe\FIMASK.043 \euro\exe\LKFMASK.*
set app=LKF

set id=LKF
  call mm 110 120

:end
set app=%aptmp%
set id=%itmp%

