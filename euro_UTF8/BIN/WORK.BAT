echo off
set lpt=lpt1

:PRINT
edit eurowork.prn
echo ┌──────────────────────┐
echo │ [d] == Druck » LPT1  │
echo │ [i] == Initialisieren│
echo │ [a] == Abbruch » DOS │
echo └──────────────────────┘
echo  
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
    if errorlevel 2 copy ..\dru\pro.kyo %LPT%
REM if errorlevel 2 copy ..\dru\pro.ibm %LPT%
copy eurowork.prn %lpt%
goto END

:END

