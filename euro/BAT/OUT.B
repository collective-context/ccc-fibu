cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set out=%1
  if %1==Temp  set out=%USER%\TEMP
  if %1==temp  set out=%USER%\TEMP
  if %1==con   set out=CON
  if %1==Con   set out=CON
  if %1==lpt1  set out=LPT1
  if %1==Lpt1  set out=LPT1
  if %1==lpt2  set out=LPT2
  if %1==Lpt2  set out=LPT2
  if %1==lpt3  set out=LPT3
  if %1==Lpt3  set out=LPT3
  if %1==com1  set out=COM1
  if %1==Com1  set out=COM1
  if %1==com2  set out=COM2
  if %1==Com2  set out=COM2
  if X1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte       : OUT=%out%    (LPT=%lpt%,  DRUCK=%druck%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax      : out ®Device¯       oder: out ®s/h¯ (fr show bzw. help)
  echo Fr ®Device¯: Dateiname oder lpt1, lpt2, lpt3, com1, com2.
  echo ÿ
  echo Beispiele   : ¯out PRINT® oder ¯out lpt1® ...
  echo ÿ
  goto end

:meldung
  echo ÿ
  echo Werte:   OUT=%out%    (LPT=%lpt%,  DRUCK=%druck%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ

:end
