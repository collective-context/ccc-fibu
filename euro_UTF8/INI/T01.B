@echo off
cls
prompt $p$g
set user=C:\euro\t01
set LW=D
cd %USER%

if X%1==X   goto start
if X%1==Xc  goto compiler
if X%1==Xco goto compiler
if X%1==Xe  edit \euro\ini\T01.B
if X%1==Xed edit \euro\ini\T01.B
if X%1==Xh  goto help
goto end

:start
  set LIB=%LW%:\euro\lib;C:\mstools\lib;C:\mstools\mfc\lib
  set INCLUDE=%LW%:\euro\include;f:\euro\c;C:\mstools\include;C:\mstools\mfc\include
  set TMP=%LW%:\
  set TEMP=%LW%:\
  set INIT=%user%
  set AP2=
  set APTMP=
  set APP=ST
  set ID=ST
  set MAN=
  set EXT=043
  set ART=MASK
  set DOC=DOC
  set OUT=CON
  set LPT=PRINT
  SET DRUCK=KYO
  SET EU_TST=
  SET MSC=C

  SET HELPFILES=C:\MSTOOLS\HELP\*.HLP
  SET PATH=C:\MSTOOLS\BIN;C:\EURO\BIN;C:\DOS;%USER%;

  goto end

:compiler
  set sA=T01
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\t01
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
  echo Syntax: t01 [h] [e] [c]
  echo ──────────────────────────────────────────────────────────────────────
  echo t01   = Festlegen der DOS-Variablen und wechseln ins User-Verzeichnis.
  echo t01 h = Ausgabe dieses Hilfstextes.
  echo t01 e = Editieren des Batch-Sources.
  echo t01 c = Compilieren des Batch-Sources.
  echo  
  goto end

:end

