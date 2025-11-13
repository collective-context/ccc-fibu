brequest /D:22000

echo off
set app=VS

:anfang
del %app%doc??.pro
..\exe\stdoc ..\hlp\%app%doc01.txt %app%doc01.pro
..\exe\stdoc ..\hlp\%app%doc02.txt %app%doc02.pro
..\exe\stdoc ..\hlp\%app%doc03.txt %app%doc03.pro
..\exe\stdoc ..\hlp\%app%doc04.txt %app%doc04.pro

echo  
echo  
echo ┌──────────────────────┐
echo │ [d] == Druck » LPT1  │
echo │ [a] == Abbruch » DOS │
echo └──────────────────────┘
echo  
\%smd%DOS\ask "a/d " d,a
if errorlevel 2 goto ende
if errorlevel 1 goto druck

:druck
copy f:\euro\dru\ff.prn lpt1
copy f:\euro\dru\doc_2.kyo lpt1

copy /b %app%doc01.pro lpt1
copy /b %app%doc02.pro lpt1
copy /b %app%doc03.pro lpt1
copy /b %app%doc04.pro lpt1

copy f:\euro\dru\ff.prn lpt1

:ende


