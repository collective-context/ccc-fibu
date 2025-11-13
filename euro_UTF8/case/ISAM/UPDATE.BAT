@echo off
    SET OUT=CON
REM SET OUT=LPT1
REM SET OUT=PROTO.TXT
rem bstop

if exist %1.obj del %1.obj
if NOT X%2==X goto start
echo  
echo Liybary-Modell angeben. Z.B. für Large: »update %1 L«
goto ende

:start
prompt Arbeit:

cls
echo on
    cl      -Zpi -Ox   -G3 -Gt -Ge -A%2 -c -W4 %1.cpp >> %OUT%
@echo off
REM cl      -Zi  -Od       -Gt -Gs -A%2 -c -W4 %1.cpp == mit Debug-Information
REM cl           -Ox       -Gt -Gs -A%2 -c -W4 %1.cpp == auch auf Geschwindigkeit
REM cl           -Os       -Gt -Gs -A%2 -c -W4 %1.cpp == nur auf Geschwindigkeit
REM cl           -Oals     -Gt -Gs -A%2 -c -W4 %1.cpp == besser für Code-Größe=Lieferversion


pause
if %OUT%==con goto lib
if %OUT%==CON goto lib
if %OUT%==Con goto lib
edit %OUT%
rem pause

:lib
echo Das war das Kompilieren im %2 - Modell.
echo %2 - Modell: Jetzt wird das Update der LIB durchgeführt.
echo on
lib \euro\LIB\eur_m80%2.lib - %1.obj + %1.obj; >> %OUT%
echo off
rem ask "DEL *.OBJ [j/n] " j,n
rem  if errorlevel 2 goto ende
rem  if errorlevel 1 del *.obj

echo Einzelbibliothek-Update wurde durchgeführt.

if %OUT%==con goto dos
if %OUT%==CON goto dos
if %OUT%==Con goto dos

 :edit
  pause
   edit %OUT%

 :dos
  rem libcopy \euro\LIB\eur_m80%2.lib
  echo on

:ende
@prompt $p$g

