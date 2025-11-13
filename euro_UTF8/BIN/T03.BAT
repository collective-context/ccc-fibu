@echo off
cls
prompt $p$g
set defa=F:
set user=%DEFA%\euro\t03

rem set MSVC=YES
 set MSVC=NO
set TOOLROOTDIR=F:\MSVC32S

set LW=E
cd %USER%

if X%1==X   goto start
if X%1==Xc  goto compiler
if X%1==Xco goto compiler
if X%1==Xe  edit \euro\ini\T03.B
if X%1==Xed edit \euro\ini\T03.B
if X%1==Xh  goto help
goto end

:start
  REM set comspec=%DEFA%\%D_VER%DOS\command.com
  set HOT=9308
  set LIB=%LW%:\euro\lib;g:\mstools\lib;g:\mstools\mfc\lib
  if %MSVC%==YES set LIB=%LW%:\euro\lib;F:\MSVCNT\lib;F:\MSVCNT\mfc\lib
  set INCLUDE=%LW%:\euro\include;f:\euro\c;g:\mstools\include;G:\mstools\mfc\include
rem   if %MSVC%==YES set INCLUDE=%LW%:\euro\include;f:\euro\c;F:\MSVCNT\include;F:\MSVCNT\mfc\include
      if %MSVC%==YES set INCLUDE=F:\MSVC32S\INCLUDE;G:\MSTOOLS\mfc\include;
  set TMP=%LW%:\
  set TEMP=%LW%:\
  set ARCTEMP=%LW%:
  set INIT=%user%
  set AP2=
  set APTMP=
  set APP=FI
  set ID=FI
  set MAN=
  set EXT=043
  set ART=MASK
  set station=2
  set DOC=DOC
  set BUG=bc
  set SRC=%DEFA%\euro\c;%DEFA%\case\saa;%DEFA%\case\isam;%DEFA%\case\dos;%DEFA%\CASE\C
  set OUT=CON

rem   set LPT=PRINT
  set LPT=LPT1

  SET DRUCK=KYO
  REM 8, oder 16
  SET EU_TST=
  SET MSC=CPP
  SET FD=H:\FD

  SET HELPFILES=G:\MSTOOLS\HELP\*.HLP
  IF %MSVC%==YES SET HELPFILES=F:\MSVC32S\HELP\*.HLP;G:\MSTOOLS\HELP\*.HLP;
  REM REM PATH %user%;%DEFA%\euro\bin;%ROOT%\%D_VER%dos;f:\public;

SET PATH=G:\MSTOOLS\BIN;F:\EURO\BIN;F:\DOS;%USER%;F:\public;G:\win;P:\DOS;G:\PM4
if %MSVC%==YES SET PATH=F:\MSVC32S\BIN;F:\MSVCNT\BIN;F:\EURO\BIN;F:\DOS;%USER%;F:\public;G:\win;P:\DOS;G:\PM4
rem SET PATH=G:\MSTOOLS\BIN;F:\EURO\BIN;F:\DOS;%USER%;F:\public;g:\win;P:\DOS;G:\PM4
rem if %MSVC%==YES SET PATH=F:\MSVC32S\BIN;F:\MSVCNT\BIN;F:\EURO\BIN;F:\DOS;%USER%;F:\public;G:\win;P:\DOS;G:\PM4
  %DEFA%

  REM 3.BAT
  REM KYO.BAT
  REM STAR.BAT
  REM IBM.BAT
  REM STOP.BAT
  goto end

:compiler
  set sA=T03
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\t03
  if exist TEMP del TEMP
  if exist %sA%.COM ren %sA%.COM TEMP
  copy \euro\ini\%sA%.B
  \euro\bin\bat2exe.com  %sA%.B
  copy %USER%\%sA%.COM \euro\bin
  if exist %sA%.B del %sA%.B
  if NOT exist %sA%.COM goto error_2
  goto end

:error_1
  echo ──────────────────────────────────────────────────────────────────────
  echo   Fehler: Source »\euro\ini\%sA%.B« fehlt.
  echo ──────────────────────────────────────────────────────────────────────
  goto end

:error_2
  ren TEMP %sA%.COM
  echo ──────────────────────────────────────────────────────────────────────
  echo   Fehler: Datei »%user%\%sA%.COM konnte nicht erstellt werden.
  echo ──────────────────────────────────────────────────────────────────────
  goto end

:help
  echo Syntax: t03 [h] [e] [c]
  echo ──────────────────────────────────────────────────────────────────────
  echo t03   = Festlegen der DOS-Variablen und wechseln ins User-Verzeichnis.
  echo t03 h = Ausgabe dieses Hilfstextes.
  echo t03 e = Editieren des Batch-Sources.
  echo t03 c = Compilieren des Batch-Sources.
  echo  
  goto end

:end
