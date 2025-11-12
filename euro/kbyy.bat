@echo off
if x%1==x goto error

REM net use q: \\212.236.21.181\daten
REM net use lpt1: \\kb70\laser
REM net use lpt1: \\172.16.21.4\laser
REM net use lpt1: \\www\laser4
REM if exist s: net use s: /delete
REM net use s: \\www\s
REM net use lpt1: /delete
REM net use lpt1: \\172.16.21.66\laser
REM net use lpt1: \\www\laser4
rem net use lpt1: /delete
rem net use lpt1: \\172.16.21.2\LJ4L


S:
cd \euro
bstop
BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
set user=\euro\t%1
CD %USER%

copy pw0.BAT pw.BAT
PW.BAT
goto ende

:error
echo Bitte die Arbeitsplatznummer mitangeben!
pause
echo  

:ende
