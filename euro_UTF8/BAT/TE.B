echo off
cls
if X%1==X  goto mask_edit
if X%1==Xm edit \euro\mask\%APP%mask.BAT
if X%1==Xf edit \euro\mask\%APP%form.BAT
if X%1==Xd goto druck
if X%1==Xs goto support

if X%1==X  goto end
if X%1==Xm  goto end
if X%1==Xf  goto end

:mask_edit
set sA=%APP%
if X%APP%==Xlks set sA=st
if X%APP%==Xlkf set sA=fi
edit \euro\mask\%sA%%ART%.BAT

goto end


:fehler
echo  
echo  ─────────────────────────────────────────────────
echo   Erlaubte Eingabe »te [m][f] [d][s]«, Fehler »%1«
echo  ─────────────────────────────────────────────────
echo  
echo   Steht für: »mask, form, druck, support«
echo  
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
echo APP wird temporär auf %1 umgestellt.
SHIFT

:START_DRUCK
echo  
echo  ────────────────────────────
echo     [d] == Druck » %LPT%
echo     [i] == Initialisieren
echo     [a] == Abbruch » DOS
echo  ────────────────────────────
echo  
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
if errorlevel 2 copy \euro\dru\te.%DRUCK% %LPT%
echo  
copy \euro\dru\te_kopf + \euro\mask\%app%mask.BAT + \euro\mask\%app%form.BAT TEMP
if NOT %LPT%==PRINT copy TEMP %LPT%
if     %LPT%==PRINT copy TEMP %USER%\PRINT
if     %LPT%==PRINT show %USER%\PRINT
echo  
set APP=%temp%
goto end

:end
echo  

