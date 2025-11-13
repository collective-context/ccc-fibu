@echo off
if x%1==x goto error

REM net use q: \\212.236.21.181\daten
REM net use lpt1: \\kb70\laser
REM net use lpt1: \\172.16.21.4\laser
REM net use lpt1: \\www\laser4
REM if exist s: net use s: /delete
REM net use s: \\www\s
net use lpt1: /delete
net use lpt2: /delete
REM net use lpt1: \\172.16.21.250\PS01-P1
net use lpt2: \\www\LJ4L
REM net use lpt1: \\NTKB\DeskJetC
rem pause

rem net use s: /delete
rem net use s: \\www\s
cls
@echo on
C:
cd \euro
@echo off
pause
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
echo á

:ende
