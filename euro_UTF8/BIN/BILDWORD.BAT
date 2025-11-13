echo off
REM SET TMP_A=ASCII
SET TMP_A=WORD
if X%1==Xa goto auto

:single
F:\euro\exe\bildword %1 %1 %TMP_A%
if X%1==X F:\euro\exe\bildword
if errorlevel 3 goto error
if errorlevel 2 goto frage
if errorlevel 1 goto error
goto ende

:auto
if X%1==Xa shift

:compiler
  if X%1==X goto ende
  F:\euro\exe\bildword %1 %1 %TMP_A%
  if errorlevel 3 goto error
  if errorlevel 2 goto frage
  if errorlevel 1 goto error
  shift
  goto auto

:frage
  if X%1==X goto ende
  echo  
  echo Fehler: Bild Nummer »%1« nicht vorhanden!
  echo  
  ask "Batch-Job beenden j/n ",jn
    if errorlevel 2 goto weiter
    if errorlevel 1 goto ende

:weiter
  shift
  goto auto

:error
echo  
echo »»» Fehler aufgetreten «««
echo  
pause

:ende

