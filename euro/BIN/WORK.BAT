echo off
set lpt=lpt1

:PRINT
edit eurowork.prn
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ [d] == Druck ¯ LPT1  ³
echo ³ [i] == Initialisieren³
echo ³ [a] == Abbruch ¯ DOS ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo ÿ
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
    if errorlevel 2 copy ..\dru\pro.kyo %LPT%
REM if errorlevel 2 copy ..\dru\pro.ibm %LPT%
copy eurowork.prn %lpt%
goto END

:END

