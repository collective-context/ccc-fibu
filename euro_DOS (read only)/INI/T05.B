@cls
@echo off
prompt $p$g
set user=%DEFA%\euro\t05
cd %USER%

if X%1==X  goto start
if X%1==Xc goto compiler
if X%1==Xe edit \euro\ini\T05.B
if X%1==Xh goto help
goto end

:start
  set comspec=%USER%\command.com
  set LASER=G:\LASER
  set SMD=MS
  set ROOT=F:
  set DEFA=F:
  set LIB=E:\lib
  set INCLUDE=e:\include
  set TMP=e:\
  set TEMP=e:\
  set ARCTEMP=E:
  set INIT=%user%
  set AP2=
  set APTMP=
  set APP=FI
  set ID=FI
  set MAN=03
  set EXT=043
  set ART=MASK
  set station=2
  set DOC=PFL
  set BUG=bc
  set SRC=f:\euro\c;f:\euro\saa;f:\euro\isam;f:\euro\dos
  set OUT=CON
  set LPT=PRINT
  SET DRUCK=KYO

  PATH %user%;%ROOT%\euro\bin;%ROOT%\%SMD%dos;f:\public;
  %DEFA%

  REM 3.BAT
  REM KYO.BAT
  REM STAR.BAT
  REM IBM.BAT
  REM STOP.BAT
  goto end

:compiler
  set sA=T05
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\t05
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