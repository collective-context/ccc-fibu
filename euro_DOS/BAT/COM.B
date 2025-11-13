@cls
rem @echo off

set So=\euro\BAT
cd %So%
echo User: Ї%user%®  /  Source-File: Ї%So%\%1.B®
:anfang
  if exist %So%\%1.B goto compiler

:frage
  if X%1==X  goto end
  echo я
  echo Datei %So%\%1.B nicht vorhanden!
  echo я
  ask "Batch-Job beenden j/n ",j,n
  if errorlevel 2 goto weiter
  if errorlevel 1 goto end

:weiter
  shift
  goto anfang

:compiler
  if exist %So%\%1.COM del %So%\%1.COM
  if exist \euro\bin%1.COM del \euro\bin\%1.COM
  echo ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
  echo Compilieren: Ї%So%\%1.B®
  \euro\bin\bat2exe.com  %1.B

  if NOT exist %So%\%1.COM goto fehler
     @copy %So%\%1.COM \euro\bin\%1.COM >> NUL
     @del %So%\%1.COM
     goto shift

  :fehler
  echo ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
  echo   Fehler in Source %So%\%1.B
  echo ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
  pause

:shift
  shift
  goto anfang

:end
  echo я

