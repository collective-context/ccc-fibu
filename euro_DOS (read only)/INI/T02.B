@echo off
cls
prompt $p$g
set user=%DEFA%\euro\t02
set LW=E
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
  set DAT=911203
  set LIB=%LW%:\euro\lib
rem t LIB=F:\euro\lib\;G:\mstools\lib;G:\mstools\mfc\lib
  set INCLUDE=%LW%:\euro\include;f:\euro\c;g:\mstools\include;g:\mstools\mfc\include
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
  set LPT=PRINT
  SET DRUCK=KYO
  REM 8, oder 16
  SET EU_TST=

  SET HELPFILES=G:\MSTOOLS\HELP\*.HLP
  REM REM PATH %user%;%DEFA%\euro\bin;%ROOT%\%D_VER%dos;f:\public;

  SET PATH=G:\MSTOOLS\BIN;F:\EURO\BIN;F:\%SMD%DOS;%USER%;F:\public;G:\windows
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

