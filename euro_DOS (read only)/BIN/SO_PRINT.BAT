echo off
if X%LPT%==X set LPT=LPT1
if X%1==Xa goto auto

:single
copy BILD0%1.TXT %LPT%
goto ende

:auto
if X%1==Xa shift

if exist BILD0%1.TXT goto kopie
:frage
  if X%1==X goto ende
  echo ÿ
  echo Fehler: Datei ¯BILD0%1.TXT® nicht vorhanden!
  echo ÿ
  ask "Batch-Job beenden j/n ",jn
    if errorlevel 2 goto weiter
    if errorlevel 1 goto ende

:weiter
  shift
  goto auto

:kopie
  copy BILD0%1.TXT %LPT%
  shift
  goto auto

:error
echo ÿ
echo ¯¯¯ Fehler aufgetreten ®®®
echo ÿ
pause

:ende

