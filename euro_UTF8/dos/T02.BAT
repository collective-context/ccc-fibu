@echo off
set path=%path%;v:\euro\bin
cls
echo v:\dos\t02.bat
rem pause
prompt $p$g
set defa=V:
set user=%DEFA%\euro\t02
rem set path=d:\winnt35\system32;d:\winnt35;c:\dos

rem set MSVC=YES
    set MSVC=NO
rem set TOOLROOTDIR=e:\MSVC32S
set TOOLROOTDIR=e:\MSVC15

set LW=V
cd %USER%

if X%1==X   goto start
if X%1==Xc  goto compiler
if X%1==Xco goto compiler
if X%1==Xe  edit \euro\ini\T02.B
if X%1==Xed edit \euro\ini\T02.B
if X%1==Xh  goto help
goto end

:start
  REM set comspec=%DEFA%\%D_VER%DOS\command.com
  set HOT=9308
  set LIB=%LW%:\euro\lib;e:\msvc15\lib;e:\msvc15\mfc\lib
  if %MSVC%==YES set LIB=%LW%:\euro\lib;H:\MSVC20\lib;H:\MSVC20\mfc\lib
  set INCLUDE=%LW%:\euro\include;v:\euro\c;e:\msvc15\include;e:\msvc15\mfc\include
rem   if %MSVC%==YES set INCLUDE=%LW%:\euro\include;v:\euro\c;H:\MSVC20\include;H:\MSVC20\mfc\include
      if %MSVC%==YES set INCLUDE=e:\MSVC32S\INCLUDE;e:\MSVC15\mfc\include;
  set TMP=%LW%:\
  set TEMP=%LW%:\
  set ARCTEMP=%LW%:
  set INIT=%user%
  set AP2=
  set APTMP=
  set APP=ST
  set ID=ST
  set MAN=
  set EXT=043
  set ART=MASK
  set station=2
  set DOC=DOC
  set BUG=bc
  set SRC=%DEFA%\euro\c;%DEFA%\case\saa;%DEFA%\case\isam;%DEFA%\case\dos;%DEFA%\CASE\C
  set OUT=CON
  set LPT=PRINT
  SET DRUCK=KYO
  REM 8, oder 16
  SET EU_TST=
  SET MSC=CPP
  SET FD=H:\FD

  SET HELPFILES=e:\MSVC15\HELP\*.HLP
  IF %MSVC%==YES SET HELPFILES=e:\MSVC32S\HELP\*.HLP;e:\MSVC15\HELP\*.HLP;
  REM REM PATH %path%;%user%;%DEFA%\euro\bin;%ROOT%\%D_VER%dos;

SET PATH=c:\winnt35\system32;c:\winnt35;e:\MSVC15\BIN;v:\EURO\BIN;c:\DOS;%USER%;v:\euro\exe
if %MSVC%==YES SET PATH=%path%;e:\MSVC32S\BIN;H:\MSVC20\BIN;v:\EURO\BIN;c:\DOS;%USER%;v:\euro\exe
rem SET PATH=%path%;e:\MSVC15\BIN;v:\EURO\BIN;C:\DOS;%USER%;
rem if %MSVC%==YES SET PATH=%path%;e:\MSVC32S\BIN;H:\MSVC20\BIN;v:\EURO\BIN;c:\DOS;%USER%;
  %DEFA%

  REM 3.BAT
  REM KYO.BAT
  REM STAR.BAT
  REM IBM.BAT
  REM STOP.BAT
  goto end

:compiler
  set sA=T02
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\t02
  if exist TEMP del TEMP
  if exist %sA%.COM ren %sA%.COM TEMP
  copy \euro\ini\%sA%.B
  \euro\bin\bat2exe.com  %sA%.B
  copy %USER%\%sA%.COM \euro\bin
  if exist %sA%.B del %sA%.B
  if NOT exist %sA%.COM goto error_2
  goto end

:error_1
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo   Fehler: Source ¯\euro\ini\%sA%.B® fehlt.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  goto end

:error_2
  ren TEMP %sA%.COM
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo   Fehler: Datei ¯%user%\%sA%.COM konnte nicht erstellt werden.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  goto end

:help
  echo Syntax: t02 [h] [e] [c]
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo t02   = Festlegen der DOS-Variablen und wechseln ins User-Verzeichnis.
  echo t02 h = Ausgabe dieses Hilfstextes.
  echo t02 e = Editieren des Batch-Sources.
  echo t02 c = Compilieren des Batch-Sources.
  echo ÿ
  goto end

:end

