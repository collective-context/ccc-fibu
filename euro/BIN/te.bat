echo off
SET LPT=LPT1
set sA=%APP%
if X%APP%==Xlks set sA=st
if X%APP%==Xlkf set sA=fi


cls
if X%1==X  m v:\euro\mask\%sA%%ART%.BAT
if X%1==Xm m v:\euro\mask\%sA%mask.BAT
if X%1==Xf m v:\euro\mask\%sA%form.BAT
if X%1==Xd goto druck
if X%1==Xs goto support

if X%1==X  goto end
if X%1==Xm  goto end
if X%1==Xf  goto end
goto end


:fehler
echo ÿ
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo   Erlaubte Eingabe ¯te [m][f] [d][s]®, Fehler ¯%1®
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿ
echo   Steht fr: ¯mask, form, druck, support®
echo ÿ
goto end

:support
copy \euro\dru\te_kopf + \euro\mask\%app%mask.BAT + \euro\mask\%app%form.BAT \euro\hlp\%app%prog.%EXT%
show \euro\hlp\%app%prog.%ext%
goto end

:druck
shift
set temp=%APP%
if X%LPT%==X   SET LPT=LPT1

if X%1==X  goto start_druck
set APP=%1
echo APP wird tempor„r auf %1 umgestellt.
SHIFT

:START_DRUCK
echo ÿ
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo     [d] == Druck ¯ %LPT%
echo     [i] == Initialisieren
echo     [a] == Abbruch ¯ DOS
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿ
C:\DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
if errorlevel 2 copy \euro\dru\te.%DRUCK% %LPT%
echo ÿ
copy \euro\dru\te_kopf + \euro\mask\%app%mask.BAT + \euro\mask\%app%form.BAT TEMP
if NOT %LPT%==PRINT copy TEMP %LPT%
if     %LPT%==PRINT copy TEMP %USER%\PRINT
if     %LPT%==PRINT show %USER%\PRINT
echo ÿ
set APP=%temp%
goto end

:end
echo ÿ
