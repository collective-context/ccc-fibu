: MAKELIB.BAT
:
: Mit dieser Batch-Datei k”nnen leicht und einfach Komplett-
: bibliotheken fr die euroSOFT-CASEbox erstellt werden.
: MAKELIB erh„lt als Argument einen Buchstaben, der das Speicher-
: modell definiert, fr das die Bibliothek erstellt werden soll.
:
:         MAKELIB S, M, C, L
:
: Die Angabe des Speichermodells muá in Grossbuchstaben erfolgen.
:
: Die Batch-Datei setzt voraus, das alle Source-Dateien im
: aktuellen Verzeichnis sind und die Deklarationsdateien ber
: die Umgebungsvariable INCLUDE erreichbar sind.
: Mit dieser Batch-Datei werden die C-Dateien aufgebaut.
: Wenn Sie eine ge„nderte Version der Assembler-Datei vio.asm mit
: aufnehmen wollen, mssen Sie den entsprechenden Aufruf „ndern.
: Der C 5.1 Compiler erzeugt beim Kompilieren der Datei dl_laden.cpp
: eine interne Fehlermeldung. Aus diesem Grund wird diese Datei fr
: das Compact- und Large Speichermodell mit dem QCL-Compiler aufgebaut.
@echo off
REM SET OUT=CON
REM SET OUT=LPT1
    SET OUT=PROTO.TXT
REM frher:cl -Zpi -Oals -G2 -Gt -Gs -A%1 -c -W4 S_*.cpp
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
if not X%2==X goto %2
echo Das String-Modul S_ wird kompiliert.
echo Bitte bleiben Sie dran
cl -Zpi -Ox   -G3 -Gt -Gs -A%1 -c -W4 S_*.cpp >> %OUT%
if errorlevel 1 goto comperror

:dos
echo Das Dos-Modul D_ wird kompiliert.
echo Bitte bleiben Sie dran
cl -Zpi -Ox   -G3 -Gt -Gs -A%1 -c -W4 D_*.cpp >> %OUT%
if errorlevel 1 goto comperror

rem :formular
rem echo Das Formular-Modul FO_ wird kompiliert.
rem echo Bitte bleiben Sie dran
rem cl -Zpi -Ox   -G3 -Gt -Gs -A%1 -c -W4 FO_*.cpp >> %OUT%
rem if errorlevel 1 goto comperror

:lib
echo Das war das Kompilieren
echo Jetzt wird die LIB aufgebaut
if not exist *.lib goto lib_weiter
ask "DEL *.LIB [j/n] " j,n
  if errorlevel 2 goto ende
  if errorlevel 1 del *.lib

:lib_weiter
lib @objekt
if errorlevel 1 goto liberror

ask "DEL *.OBJ [j/n] " j,n
  if errorlevel 2 goto ende
  if errorlevel 1 del *.obj

echo Einzelbibliothek wurde erstellt.
goto ende

:comperror
echo Fehler bei der Kompilierung.
pause
goto ende

:liberror
echo Fehler bei der Erstellung der Einzelbibliothek.
pause

:ende
  xcopy DOS_M80%1.lib \euro\LIB
  rem libcopy \euro\LIB\dos_m80%1.lib
prompt $p$g
m proto.txt

