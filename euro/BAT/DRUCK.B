cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set lpt=%1
  if %1==kyo   set druck=KYO
  if %1==Kyo   set druck=KYO
  if %1==sta   set druck=STA
  if %1==Sta   set druck=STA
  if %1==ibm   set druck=IBM
  if %1==IBM   set druck=IBM
  if X%1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte     : DRUCK=%druck%    (LPT=%lpt%,   OUT=%out%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax    : druck ®Wahl¯        oder: druck ®s/h¯ (fr show bzw. help)
  echo Fr ®Wahl¯: Drucker-Extention mit drei Buchstaben.
  echo ÿ
  echo Beispiele : ¯druck KYO® oder ¯STA®, ¯IBM®, ¯HP®, ...
  echo ÿ
  goto end

:meldung
  echo ÿ
  echo Werte:   DRUCK=%druck%    (LPT=%lpt%,   OUT=%out%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ

:end
