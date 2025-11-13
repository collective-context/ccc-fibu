cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set id=%1
  if X%1==X goto fehler
  goto meldung

:fehler
  echo  
  echo Werte    : ID=%id%     (APP=%app% / AP2=%ap2% / ART=%art%)
  echo ──────────────────────────────────────────────────────────────────────
  echo Syntax   : id [ID]                 oder: id «s/h» (für show bzw. help)
  echo für [ID] : Identifikation des Programm-, Masken- oder Formular-Source.
  echo  
  echo Beispiele: »id BA«, »id SY«, »id FI« oder »id LKF« ...
  echo  
  goto end

:meldung
  echo  
  echo Werte:  ID=%id%     (APP=%app% / AP2=%ap2% / ART=%art%)
  echo ──────────────────────────────────────────────────────────────────────
  echo  

:end
