@cls
@echo off

if X%1==X?  goto frage
if X%1==Xe  goto ega
if X%1==Xv  goto vga
if X%1==Xc  goto cga
if X%1==Xh  goto hercules
if X%1==Xco goto compiler
if X%1==Xed edit \euro\ini\VIDEO.B
if X%1==Xhe goto help
goto end

:frage
      vgamode
  REM vmode ?
  goto end

:ega
      vgamode E
  REM vmode EGA
  goto end

:vga
      vgamode V
  REM vmode VGA
  goto end

:cga
      vgamode C
  REM vmode CGA
  goto end

:hercules
  vmode HERCULES
  goto end

:compiler
  set sA=VIDEO
  if NOT exist \euro\ini\%sA%.B goto error_1
  cd \euro\bin
  if exist TEMP del TEMP
  if exist %sA%.COM ren %sA%.COM TEMP
  copy \euro\ini\%sA%.B
  \euro\bin\bat2exe.com  %sA%.B
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
  echo Syntax: video [he/ed/co] [e/v/c/h/?]
  echo ──────────────────────────────────────────────────────────────────────
  echo video ?     = Zeigt die Möglichkeiten von video.
  echo video e/v/c = Umschalten auf EGA, VGA oder CGA-Mode.
  echo video h     = Umschalten auf HERCULES-Mode.
  echo video he    = Ausgabe dieses Hilfstextes.
  echo video ed    = Editieren des Batch-Sources.
  echo video co    = Compilieren des Batch-Sources.
  echo  
  goto end

:end


