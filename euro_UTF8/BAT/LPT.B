cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set lpt=%1
  if %1==null  set lpt=NULL
  if %1==Null  set lpt=NULL
  if %1==con   set lpt=CON
  if %1==Con   set lpt=CON
  if %1==print set lpt=PRINT
  if %1==Print set lpt=PRINT
  if %1==lpt1  set lpt=LPT1
  if %1==Lpt1  set lpt=LPT1
  if %1==lpt2  set lpt=LPT2
  if %1==Lpt2  set lpt=LPT2
  if %1==lpt3  set lpt=LPT3
  if %1==Lpt3  set lpt=LPT3
  if %1==com1  set lpt=COM1
  if %1==Com1  set lpt=COM1
  if %1==com2  set lpt=COM2
  if %1==Com2  set lpt=COM2
  if X%1==X goto fehler
  goto meldung

:fehler
  echo  
  echo Werte       : LPT=%lpt%    (OUT=%out%,   DRUCK=%druck%)
  echo ──────────────────────────────────────────────────────────────────────
  echo Syntax      : lpt «Device»        oder: lpt «s/h» (für show bzw. help)
  echo Für «Device»: Dateiname oder lpt1, lpt2, lpt3, com1, com2.
  echo  
  echo Beispiele   : »lpt PRINT« oder »lpt lpt1« ...
  echo  
  goto end

:meldung
  echo  
  echo Werte:   LPT=%lpt%    (OUT=%out%,   DRUCK=%druck%)
  echo ─────────────────────────────────────────────────────────────────
  echo  

:end
