cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

if %1==MASK goto mask
if %1==Mask goto mask
if %1==mask goto mask
if %1==FORM goto form
if %1==Form goto form
if %1==form goto form
if %1==PRINT goto print
if %1==Print goto print
if %1==print goto print
if %1==BATCH goto batch
if %1==Batch goto batch
if %1==batch goto batch
if %1==BEI   goto bei
if %1==Bei   goto bei
if %1==bei   goto bei
if %1==HOT   goto hot
if %1==Hot   goto hot
if %1==hot   goto hot
goto fehler

:mask
  set art=MASK
  goto meldung

:form
  set art=FORM
  goto meldung

:print
  set art=PRINT
  goto meldung

:batch
  set art=BATCH
  goto meldung

:bei
  set art=BEI
  goto meldung

:hot
  set art=HOT
  goto meldung

:fehler
  echo  
  echo Fehler »%1«    Werte: ART=%art%   (ID=%id% / APP=%app% / AP2=%ap2%)
  echo ────────────────────────────────────────────────────────────────────
  echo Syntax    : art «Wahl»          oder: art «s/h» (für show bzw. help)
  echo für «Wahl»: »mask, form, bei, batch oder print«
  echo  
  echo Beispiel  : ap mask
  echo  
  goto end

:meldung
  echo  
  echo Werte: ART=%art%   (ID=%id% / APP=%app% / AP2=%ap2%)
  echo ───────────────────────────────────────────────────────
  echo  

:end

