brequest /D:22000

echo off
if %1==e goto start

:anfang
if not exist %app%DOC%1.txt goto ende
..\exe\index %app%DOC%1.txt %1.pro
goto edit

:start
shift

:edit
if not exist %app%DOC%1.pro goto ende
edit %app%DOC%1.pro
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ [d] == Druck ¯ LPT1  ³
echo ³ [a] == Abbruch ¯ DOS ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo ÿ
\%smd%DOS\ask "a/d " d,a
if errorlevel 2 goto ende
if errorlevel 1 goto druck

:druck
copy f:\euro\dru\ff.prn lpt1
copy f:\euro\dru\doc_2.kyo lpt1
copy /b %app%DOC%1.pro lpt1
copy f:\euro\dru\ff.prn lpt1
shift

goto anfang

:ende


