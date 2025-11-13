REM @echo off
set sA=%APP%
if NOT X%AP2%==X set APP=%AP2%
if X%LPT%==X SET LPT=LPT1
if X%1==Xm goto mask
if X%1==Xf goto form
goto %ART%

:mask
\euro\exe\katalog \euro\exe\%APP%MASK.%EXT% %USER%\KATALOG.PRO
goto start

:form
IF X%MAN%==X \euro\exe\katalog %DEFA%\euro\dat\%APP%FORM.%EXT% %USER%\KATALOG.PRO
IF X%MAN%==X goto start
\euro\exe\katalog %DEFA%\euro\dat\D%MAN%\%APP%FORM.%EXT% %USER%\KATALOG.PRO

:start
edit %USER%\katalog.pro
echo  
echo  ────────────────────────────
echo     [d] == Druck » %LPT%
echo     [a] == Abbruch » DOS
echo  ────────────────────────────
echo  
\%smd%DOS\ask "a/d " d,a
if errorlevel 2 goto abbruch
if errorlevel 1 goto druck

:druck
  if     %LPT%==PRINT copy %USER%\katalog.pro %USER%\PRINT
  if NOT %LPT%==PRINT copy %USER%\katalog.pro %LPT%

:abbruch
set APP=%sA%

