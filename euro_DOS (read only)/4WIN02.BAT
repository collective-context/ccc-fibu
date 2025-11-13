@echo off
net use z: /delete
net use lpt1: /delete
REM net use z: \\172.16.20.111\d$
net use z: \\172.16.20.2\win_d
net use lpt1: \\www\laser4
z:
set user=\euro\t02
CD %USER%

\EURO\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
copy pw0.BAT pw.BAT
PW.BAT

