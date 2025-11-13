@echo off

if _%1==_R copy f:\euro\dru\tohp.kyo lpt1
if _%1==_R goto end


 :hp
   ask "Zeichensatz-Version [R]oman 8, [I]BM PC, [E]inladung ?  " r,i,e
      if errorlevel 3 copy f:\euro\dru\tohpibm.kyo lpt1
      if errorlevel 3      G:\LASER\LASER G:\WORD\DBS\einlad.FTL
      if errorlevel 2 copy f:\euro\dru\tohpibm.kyo lpt1
      if errorlevel 1 copy f:\euro\dru\tohp.kyo lpt1
      goto end

 :end
