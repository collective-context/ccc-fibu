cls
@echo off
if X%1==Xs goto meldung
if X%1==Xh goto fehler

set ext=%1
  if X%1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte    : EXT=%ext%     (MAN=%man% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax   : ext [EXT]              oder: ext ®s/h¯ (fr show bzw. help)
  echo fr [EXT]: L„ndernummer fr Masken- und Formularsource.
  echo ÿ
  echo Beispiele: ¯ext 043®, ¯ext 049®, ¯ext 039® ...
  echo ÿ
  goto end

:meldung
  echo ÿ
  echo Werte    : EXT=%ext%     (MAN=%man% / ART=%art%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ

:end
