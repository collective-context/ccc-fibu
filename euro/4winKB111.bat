@echo off
REM if exist d: net use d: /delete
net use lpt2: /delete
REM net use v: \\www\win_d
REM net use lpt2: \\kb70\laser
net use lpt2: \\172.16.1.61\Kyocera
rem net use lpt1: \\172.16.22.210\lexmark
REM net use lpt1: \\www\laser4
REM net use lpt1: \\win98\laser
pause

d:
set user=\euro\t11
CD %USER%

rem pause
\EURO\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
copy pw0.BAT pw.BAT
PW.BAT
