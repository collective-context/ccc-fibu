ECHO OFF
REM HAUPT.MNU
MENU 0 0 4 ST L001 ************************************************************************************************************
IF ERRORLEVEL 1 GOTO ENDE
@echo off
exitemu

CD \euro\t11
BTRIEVE /U:22 /P:2048 /T:\euro\EXE\BTRIEVE.TRN > NUL
PW.BAT
:ENDE
