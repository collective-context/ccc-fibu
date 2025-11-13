set TEMP=%MAN%
set MAN=01

echo off
brequest /D:22000
butil -reset
del *.%Ext%
ncopy F:\euro\DAT\D%MAN%\%App%form.%Ext%

:anfang
if not exist \euro\form\%ID%f%1.M03 goto ende
\euro\exe\stfc \euro\form\%ID%F%1.M03 %App%form.%Ext% %1 %App%form.pro
shift
goto anfang

:ende
ncopy %App%form.%Ext% F:\euro\DAT\D%MAN%
REM del *.%Ext%
set MAN=%TEMP%

