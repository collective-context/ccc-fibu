REM lh brequest /D:22000
REM @echo off
if X%1==XDOC goto SET
if X%1==XDoc goto SET
if X%1==Xdoc goto SET
if X%1==XPFL goto SET
if X%1==XPfl goto SET
if X%1==Xpfl goto SET
if X%1==Xs   goto SET
if X%1==Xh   goto SET
if X%1==X    goto SET

if X%DOC%==X   SET DOC=DOC
if X%LPT%==X   SET LPT=LPT1
if X%DRUCK%==X SET DRUCK=KYO

echo off
if %1==e goto start
if %2==/e goto start

:anfang
if not exist ..\hlp\%app%%DOC%%1.txt goto ende
set temp=%2
echo ..\exe\doc ..\hlp\%app%%DOC%%1.txt %app%%DOC%%1.pro %temp%
     ..\exe\doc ..\hlp\%app%%DOC%%1.txt %app%%DOC%%1.pro %temp%
REM bc ..\exe\doc ..\hlp\%app%%DOC%%1.txt %app%%DOC%%1.pro %2
goto edit

:start
shift

:edit
if not exist %app%%DOC%%1.pro goto ende
if %LPT%==NULL goto ende
echo ÿ
echo ÿ
pause
REM g:\word5\word %app%%DOC%%1.pro
edit %app%%DOC%%1.pro
REM copy /b %app%%DOC%%1.pro \euro\doc\%app%%DOC%%1.txt
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ [d] == Druck ¯ %LPT%  ³
echo ³ [a] == Abbruch ¯ DOS ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo ÿ
\%smd%DOS\ask "a/d " d,a
if errorlevel 2 goto ende
if errorlevel 1 goto druck

:druck
if %DRUCK%==KYO  goto druck_kyo
if %DRUCK%==STAR goto druck_hp
if %DRUCK%==IBM goto druck_ibm
goto druck_error

:druck_kyo
  copy f:\euro\dru\ff.prn %LPT%
  copy f:\euro\dru\doc_2.kyo %LPT%
  copy /b %app%%DOC%%1.pro %LPT%
  copy f:\euro\dru\ff.prn %LPT%
  goto druck_ende

:druck_hp
:druck_ibm
  copy /b %app%%DOC%%1.pro %LPT%
  goto druck_ende

:druck_error
  echo Error: Der Drucker mit der Bezeichnung ¯%DRUCK%® ist nicht bekannt!
  echo ÿ
  pause
  echo ÿ

:druck_ende
shift

goto anfang

:SET
if X%1==Xs goto meldung
if X%1==Xh goto fehler

if X%1==XDOC set DOC=DOC
if X%1==XDoc set DOC=DOC
if X%1==Xdoc set DOC=DOC
if X%1==XPFL set DOC=PFL
if X%1==XPfl set DOC=PFL
if X%1==Xpfl set DOC=PFL
  if X%1==X goto fehler
  goto meldung

:fehler
  echo ÿ
  echo Werte    : DOC=%doc%     (MAN=%man% / ART=%art% / EXT=%ext%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Syntax   : doc [DOC]              oder: doc ®s/h¯ (fr show bzw. help)
  echo fr [DOC]: DOC fr Dokumentation oder PFL fr Pflichtenheft.
  echo     oder : Die Nummer der Druckanweisungsdatei !!!
  echo ÿ
  echo Beispiele: ¯doc DOC®, ¯doc PFL®, ¯DOC 01® ...
  echo ÿ
  goto ende

:ende
:meldung
  echo ÿ
  echo Werte    : DOC=%doc%     (MAN=%man% / ART=%art% / EXT=%ext%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ



