: MAKELIB.BAT
:
: Mit dieser Batch-Datei können leicht und einfach Komplett-
: bibliotheken für die euroSOFT-CASEbox erstellt werden.
: MAKELIB erhält als Argument einen Buchstaben, der das Speicher-
: modell definiert, für das die Bibliothek erstellt werden soll.
:
:         MAKELIB S, M, C, L
:
: Die Angabe des Speichermodells muß in Grossbuchstaben erfolgen.
:
: Die Batch-Datei setzt voraus, das alle Source-Dateien im
: aktuellen Verzeichnis sind und die Deklarationsdateien über
: die Umgebungsvariable INCLUDE erreichbar sind.
: Mit dieser Batch-Datei werden die C-Dateien aufgebaut.
: Wenn Sie eine geänderte Version der Assembler-Datei vio.asm mit
: aufnehmen wollen, müssen Sie den entsprechenden Aufruf ändern.
: Der C 5.1 Compiler erzeugt beim Kompilieren der Datei dl_laden.cpp
: eine interne Fehlermeldung. Aus diesem Grund wird diese Datei für
: das Compact- und Large Speichermodell mit dem QCL-Compiler aufgebaut.
@echo off
REM SET OUT=CON
REM SET OUT=LPT1
    SET OUT=PROTO.TXT
REM früher:cl -Zpi -Oals -G2 -Gt -Gs -A%1 -c -W4 db_*.cpp
rem bstop
prompt $h$h
echo Makelib.BAT
echo Batch-Datei zur Erstellung einer Bibliothek für die C-Toolbox
echo  
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
echo Das Datenbankmodul Db_ wird kompiliert.
echo Das dauert jetzt etwas.
echo Bitte bleiben Sie dran
cl -Zpi -Ox   -G3 -Gt -Gs -A%1 -c -W4 db_*.cpp >> %OUT%
if errorlevel 1 goto comperror

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
  xcopy EUR_M80%1.lib \euro\LIB
  rem libcopy \euro\LIB\eur_m80%1.lib
prompt $p$g
m proto.txt

