@REM [M][W]ork-File sichern oder einlesen.
@echo off
cls
if X%1==Xb goto backup
if X%1==Xr goto restore
if X%1==Xs goto show
if X%1==Xe goto edit
if X%1==Xd goto delete
if X%1==Xf goto files
if X%1==Xco goto compiler
if X%1==Xed edit \euro\ini\MW.B
if X%1==Xhe goto help
goto help

:backup
  if NOT exist %user%\m.%2 goto ba_start
  echo  
  \euro\bin\ask "Datei %user%\m.%2 schon vorhanden, überschreiben [j]/[n]/[s]how ",j,n,s
  if errorlevel 3 goto ba_show
  if errorlevel 2 goto end
  if errorlevel 1 goto ba_start

  :ba_show
  show %user%\m.%2
  echo  
  \euro\bin\ask "Datei %user%\m.%2 schon vorhanden, überschreiben [j]/[n] ",j,n
  if errorlevel 2 goto end
  if errorlevel 1 goto ba_start

  :ba_start
    copy %user%\m.tmp %user%\m.%2
  goto end

:restore
  if NOT exist %user%\m.%2 goto re_end
  if NOT exist %user%\m.tmp goto re_start

  show %user%\m.tmp
  echo  
  \euro\bin\ask "Datei %user%\m.tmp ist vorhanden, überschreiben [j]/[n] ",j,n
  if errorlevel 2 goto end

  :re_start
    copy %user%\m.%2 %user%\m.tmp
  goto end

  :re_end
    echo  
    echo Datei %user%\m.%2 ist nicht vorhanden!
    echo  
    pause
    echo  
    goto end


:show
:edit
  if exist %user%\m.%2 goto sh_start
  echo  
  echo Datei %user%\m.%2 ist nicht vorhanden!
  echo  
  pause
  echo  
  goto end

  :sh_start
    if X%1==Xs show %user%\m.%2
    if X%1==Xe edit %user%\m.%2
    goto end

:delete
  if exist %user%\m.%2 goto de_start
  echo  
  echo Datei %user%\m.%2 ist nicht vorhanden!
  echo  
  pause
  echo  
  goto end

  :de_start
    show %user%\m.%2
    \euro\bin\ask "Datei löschen! Sind Sie sicher [j]/[n] ",j,n
    if errorlevel 2 goto end
    del %user%\m.%2
    goto end

:files
  if     X%2==X dir %user%\m.*  /O-D /P
  if NOT X%2==X dir %user%\m.%2 /O-D /P
  goto end

:compiler
  set sA=MW
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
  echo [M]-Editor [W]ork-File: mw  b/r/s/e/d »EXT« oder [he/ed/co]
  echo »EXT« = Dreistelliges File-Extension, z.B.: 001, 002 ...
  echo ──────────────────────────────────────────────────────────────────────
  echo mw  b »EXT« = Backup  von »M.TMP« auf  »M.001« wenn »EXT«==001.
  echo mw  r »EXT« = Restore von »M.001« auf  »M.TMP« wenn »EXT«==001.
  echo mw  s »EXT« = Show    von »M.001« wenn »EXT«==001.
  echo mw  e »EXT« = Edit    von »M.001« wenn »EXT«==001.
  echo mw  d »EXT« = Delete  von »M.TMP« wenn »EXT«==TMP.
  echo mw  f [EXT] = Files, zeigt Dateien mit »M.0??« wenn »EXT«==0??.
  echo  
  echo mw he = Ausgabe dieses Hilfstextes.
  echo mw ed = Editieren des Batch-Sources.
  echo mw co = Compilieren des Batch-Sources.
  echo  
  goto end

:end
  echo  


