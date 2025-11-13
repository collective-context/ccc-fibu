@echo off
    SET OUT=CON
rem SET OUT=LPT1
rem SET OUT=PROTO.TXT
rem bstop

if exist %1.obj del %1.obj
if NOT X%2==X goto start
echo ˇ
echo Liybary-Modell angeben. Z.B. fÅr Large: Øupdate %1 LÆ
goto ende

:start
prompt Arbeit:

cls
echo on
    cl      -Zpi -Ox   -G3 -Gt -Ge -A%2 -c -W4 %1.cpp >> %OUT%
@echo off
rem cl      -Zpi -Oals -G2 -Gt -Gs -A%2 -c -W3 %1.cpp >> %OUT%
rem cl      -Zpi -Oals -G2 -Gt -Gs -A%2 -c -W4 %1.cpp >> %OUT%
rem cl      -Zp  -Od   -G2 -Gt -Gs -A%2 -c -W4 %1.cpp >> %OUT%
REM cl      -Zi  -Od       -Gt -Gs -A%2 -c -W4 %1.cpp == mit Debug-Information
REM cl           -Ox       -Gt -Gs -A%2 -c -W4 %1.cpp == auch auf Geschwindigkeit
REM cl           -Os       -Gt -Gs -A%2 -c -W4 %1.cpp == nur auf Geschwindigkeit
REM cl           -Oals     -Gt -Gs -A%2 -c -W4 %1.cpp == besser fÅr Code-Grî·e=Lieferversion

pause
if %OUT%==con goto lib
if %OUT%==CON goto lib
if %OUT%==Con goto lib
edit %OUT%
pause

:lib
echo Das war das Kompilieren im %2 - Modell.
copy %1.obj \euro\c

 :ende
  echo ˇ
  echo Update wurde durchgefÅhrt.
  prompt $p$g
  echo ˇ

