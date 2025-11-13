echo off
cd \euro\hlp

if X%1==X goto label1
goto label2

:label1
echo  
if NOT EXIST %app%hlp10.%EXT% echo Datei nicht vorhanden: »%app%hlp10.%EXT%«
if NOT EXIST %app%hlp10.%EXT% goto ende
if exist HELP.%EXT% echo Löschen: »HELP.%EXT%«
if exist HELP.%EXT% echo  
echo Laden: »%app%hlp10.%EXT%«
echo  
pause
if exist HELP.%EXT% del HELP.%EXT%  
ren %app%hlp10.%EXT% HELP.%EXT%
goto start

:label2
echo  
if NOT EXIST %app%hlp%1.%EXT% echo Datei nicht vorhanden: »%app%hlp%1.%EXT%«
if NOT EXIST %app%hlp%1.%EXT% goto ende
if exist HELP.%EXT% echo Löschen: »HELP.%EXT%«
if exist HELP.%EXT% echo  
echo Laden: »%app%hlp%1.%EXT%«
echo  
pause
ren %app%hlp%1.%EXT% HELP.%EXT%
goto start


:start
pause
\euro\exe\ST1100 998 /EU 920

if X%1==X goto ren1
goto ren2

:ren1
ren HELP.%EXT% %app%hlp10.%EXT%
goto ende

:ren2
ren HELP.%EXT% %app%hlp%1.%EXT%

:ende
echo  
cd %user%
