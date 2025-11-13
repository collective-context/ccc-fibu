cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set app=%1
set ap2=%2
  if X%1==X goto fehler
  goto meldung

:fehler
  echo  
  echo Werte    : APP=%app%  AP2=%ap2%   (ID=%id% / ART=%art%)
  echo ─────────────────────────────────────────────────────────────────
  echo Syntax   : ap [APP] [APP]     oder: ap «s/h» (für show bzw. help)
  echo für [APP]: Applikationsname für Programme, Masken oder Formulare.
  echo  
  echo Beispiele: »ap SY«, »ap FI« oder »ap LKF« ...
  echo  
  goto end

:meldung
  echo  
  echo Werte:  APP=%app%  AP2=%ap2%   (ID=%id% / ART=%art%)
  echo ─────────────────────────────────────────────────────────────────
  echo  

:end
