%DEFA%
cd \euro\exe
echo off

if X%1==X   goto help
if X%1==Xco goto compiler
if X%1==Xed edit \euro\ini\C.B
if X%1==Xhe goto help
goto start

:start
  if %1==t   cd %USER%
  if %1==e   cd \euro\EXE
  if %1==h   cd \euro\HLP
  if %1==d   cd \euro\DAT

  if %1==b   cd \euro\BIN
  if %1==c   cd \euro\C
  if %1==dat cd \euro\DAT
  if %1==dem cd \euro\DEMO
  if %1==sam cd \euro\SAMPLES

  if %1==cas cd \case\C
  if %1==dos cd \case\DOS
  if %1==i   cd \case\ISAM
  if %1==s   cd \case\SAA
  goto end

:compiler
  @cls
  set sA=C
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\bin
  if exist TEMP del TEMP
  if exist %sA%.COM ren %sA%.COM TEMP
  copy \euro\ini\%sA%.B
  \euro\bin\bat2exe.com  %sA%.B
  if exist %sA%.B del %sA%.B
  if NOT exist %sA%.COM goto error_2
  echo ÿ
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo   Datei: ¯\euro\bin\%sA%.COM® erstellt.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  goto end

:error_1
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo   Fehler: Source ¯\euro\ini\%sA%.B® fehlt.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  goto end

:error_2
  ren TEMP %sA%.COM
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo   Fehler: Datei ¯\euro\bin\%sA%.COM konnte nicht erstellt werden.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  goto end

:help
  echo ÿ
  echo Syntax     : c ®Kennzeichen-Verzeichnis¯            oder: c [he/ed/co]
  echo Verzeichnis: t/e/h/dat/c/b  (Tnn/EXE/HLP/Dnn/C/BIN)
  echo Verzeichnis: s/i/dos        (SAA/ISAM/DOS)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo c ®KZ¯ = CD -¯ Verzweigt ins gewnschte Verzeichnis.
  echo c      = Ausgabe dieses Hilfstextes.
  echo c he   = Ausgabe dieses Hilfstextes.
  echo c ed   = Editieren des Batch-Sources.
  echo c co   = Compilieren des Batch-Sources.
  echo ÿ
  goto end

:end

