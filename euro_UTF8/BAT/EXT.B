cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set ext=%1
  if X%1==X goto fehler
  goto meldung

:fehler
  echo  
  echo Werte    : EXT=%ext%     (MAN=%man% / ART=%art%)
  echo ──────────────────────────────────────────────────────────────────────
  echo Syntax   : ext [EXT]              oder: ext «s/h» (für show bzw. help)
  echo für [EXT]: Ländernummer für Masken- und Formularsource.
  echo  
  echo Beispiele: »ext 043«, »ext 049«, »ext 039« ...
  echo  
  goto end

:meldung
  echo  
  echo Werte    : EXT=%ext%     (MAN=%man% / ART=%art%)
  echo ──────────────────────────────────────────────────────────────────────
  echo  

:end
