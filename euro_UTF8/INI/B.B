@echo off
cls
if X%1==Xc goto compiler
if X%1==Xco goto compiler
if X%1==Xe edit \euro\ini\B.B
if X%1==Xed edit \euro\ini\B.B
if X%1==Xh goto help
if X%1==Xhe goto help
goto start

:start
  bstop
  F:
rem  NOT X%1==Xk goto high
  if     X%1==Xh goto high

  if X%1==Xbt     \DOS\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
  if X%1==Xbt     goto end

     if X%DEFA%==XC: \DOS\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
     if X%DEFA%==XF: brequest /D:22000
  goto end

:high
  if X%DEFA%==XC: goto hi_lokal
  if X%DEFA%==XF: goto hi_netz

  :hi_lokal
    echo load \DOS\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
         load \DOS\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN

  if exist F:\public\logout.exe goto hi_netz
  goto end

  :hi_netz
    echo F:\DOS\386load size=45648 flexframe prog=brequest /D:22000
         %DEFA%\DOS\386load size=45648 flexframe prog=brequest /D:22000

  goto end

:compiler
  set sA=B
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\bin
  if exist TEMP del TEMP
  if exist %sA%.COM ren %sA%.COM TEMP
  copy \euro\ini\%sA%.B
  \euro\bin\bat2exec.com  %sA%.B
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
  echo   Fehler: Datei »\euro\bin\%sA%.COM konnte nicht erstellt werden.
  echo ──────────────────────────────────────────────────────────────────────
  goto end

:help
  echo Syntax: b [h] [e] [c]
  echo ──────────────────────────────────────────────────────────────────────
  echo b    = Starten von Btrieve.
  echo b he = Ausgabe dieses Hilfstextes.
  echo b ed = Editieren des Batch-Sources.
  echo b co = Compilieren des Batch-Sources.
  echo  
  goto end

:end

