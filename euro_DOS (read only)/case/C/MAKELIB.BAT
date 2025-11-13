@echo off
REM SET OUT=CON
REM SET OUT=LPT1
    SET OUT=PROTO.TXT
REM frher:cl -Zpi -Oals -G2 -Gt -Gs -A%1 -c -W4 ba_*.cpp
rem bstop
prompt $h$h
echo Makelib.BAT
echo Batch-Datei zur Erstellung einer Bibliothek fr die C-Toolbox
echo ÿ
if exist proto.txt del proto.txt
:test1
if not "%1"=="A" goto test2
echo Speichermodell fehlt  S(mall), M(edium), C(ompact) oder L(arge)
pause
goto ende

:test2
if "%1"=="S" goto starten
if "%1"=="M" goto starten
if "%1"=="C" goto starten
if "%1"=="L" goto starten
echo Speichermodell fehlt  S(mall), M(edium), C(ompact) oder L(arge)
pause
goto ende

:starten

:ba
echo Das Basis-Modul wird kompiliert.
echo Das dauert jetzt seine Zeit.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 ba_*.cpp >> %OUT%

xcopy ba_*.obj \euro\c

:bb
echo Das Block-Modul wird kompiliert.
echo Das dauert jetzt seine Zeit.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 bb_*.cpp >> %OUT%
xcopy bb_*.obj \euro\c

:dl
echo Das Dialog-Modul wird kompiliert.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 dl_*.cpp >> %OUT%
xcopy dl_*.obj \euro\c

:db
echo Das Datenbank-Modul wird kompiliert.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 db_*.cpp >> %OUT%
xcopy db_*.obj \euro\c

:ma
echo Das Masken-Modul wird kompiliert.
echo Das dauert jetzt seine Zeit.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 ma_*.cpp >> %OUT%
xcopy ma_*.obj \euro\c

:fo
echo Das Formular-Modul wird kompiliert.
echo Das dauert jetzt seine Zeit.
echo Bitte bleiben Sie dran
cl -Zpi -Ox -G3 -Gt -Gs -A%1 -c -W4 fo_*.cpp >> %OUT%
xcopy fo_*.obj \euro\c

:ende
prompt $p$g
if exist proto.txt m proto.txt

