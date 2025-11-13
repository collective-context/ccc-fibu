cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set id=%1
  if X%1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte    : ID=%id%     (APP=%app% / AP2=%ap2% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax   : id [ID]                 oder: id ®s/h¯ (fr show bzw. help)
  echo fr [ID] : Identifikation des Programm-, Masken- oder Formular-Source.
  echo ÿ
  echo Beispiele: ¯id BA®, ¯id SY®, ¯id FI® oder ¯id LKF® ...
  echo ÿ
  goto end

:meldung
  echo ÿ
  echo Werte:  ID=%id%     (APP=%app% / AP2=%ap2% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ

:end
