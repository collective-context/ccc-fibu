echo off
set APTMP=%APP%
if NOT X%AP2%==X set APP=%AP2%
if X%LPT%==X SET LPT=LPT1
if X%1==Xm goto mask
if X%1==Xf goto form
goto %ART%

if X%1==X  goto start
if X%1==Xe goto start
set app=%1
echo APP wird temporär auf %1 umgestellt.
SHIFT

:START
if %ART% == FORM goto FORM
if %ART% == Form goto FORM
if %ART% == form goto FORM
if %ART% == MASK goto MASK
if %ART% == Mask goto MASK
if %ART% == mask goto MASK
if %ART% == PRINT goto PRINT
if %ART% == Print goto PRINT
if %ART% == print goto PRINT
echo %ART% ist falsche Umgebungsvariable
goto END

:FORM
if     X%1==Xe edit %user%\%app%form.pro
if NOT X%1==Xe show %user%\%app%form.pro
echo  
echo  ────────────────────────────
echo     [d] == Druck » %LPT%
echo     [i] == Initialisieren
echo     [a] == Abbruch » DOS
echo  ────────────────────────────
echo  
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
if errorlevel 2 copy ..\DRU\pro.%DRUCK% %LPT%
copy %user%\%app%form.pro  %LPT%
goto END

:MASK
if     X%1==Xe edit %user%\%app%mask.pro
if NOT X%1==Xe show %user%\%app%mask.pro
echo  
echo  ────────────────────────────
echo     [d] == Druck » %LPT%
echo     [i] == Initialisieren
echo     [a] == Abbruch » DOS
echo  ────────────────────────────
echo  
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
if errorlevel 2 copy ..\DRU\pro.%DRUCK% %LPT%
copy %user%\%app%mask.pro  %LPT%
goto END

:PRINT
if     X%1==Xe edit %user%\%app%print.pro
if NOT X%1==Xe show %user%\%app%print.pro
echo  
echo  ────────────────────────────
echo     [d] == Druck » %LPT%
echo     [i] == Initialisieren
echo     [a] == Abbruch » DOS
echo  ────────────────────────────
echo  
\%smd%DOS\ask "i/d/a " d,i,a
if errorlevel 3 goto end
if errorlevel 2 copy ..\DRU\pro.%DRUCK% %LPT%
\%smd%DOS\ask "i/d/a " i,d,a
copy %user%\%app%print.pro %lpt%
goto END

:END
set APP=%APTMP%

