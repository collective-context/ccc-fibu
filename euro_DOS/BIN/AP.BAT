cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set app=%1
set ap2=%2
  if X%1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte    : APP=%app%  AP2=%ap2%   (ID=%id% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax   : ap [APP] [APP]     oder: ap ®s/h¯ (fr show bzw. help)
  echo fr [APP]: Applikationsname fr Programme, Masken oder Formulare.
  echo ÿ
  echo Beispiele: ¯ap SY®, ¯ap FI® oder ¯ap LKF® ...
  echo ÿ
  goto end

:meldung
  echo ÿ
  echo Werte:  APP=%app%  AP2=%ap2%   (ID=%id% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ

:end
