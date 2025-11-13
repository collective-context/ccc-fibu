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
  echo  
  echo Werte     : DRUCK=%druck%    (LPT=%lpt%,   OUT=%out%)
  echo ──────────────────────────────────────────────────────────────────────
  echo Syntax    : druck «Wahl»        oder: druck «s/h» (für show bzw. help)
  echo Für «Wahl»: Drucker-Extention mit drei Buchstaben.
  echo  
  echo Beispiele : »druck KYO« oder »STA«, »IBM«, »HP«, ...
  echo  
  goto end

:meldung
  echo  
  echo Werte:   DRUCK=%druck%    (LPT=%lpt%,   OUT=%out%)
  echo ─────────────────────────────────────────────────────────────────
  echo  

:end
