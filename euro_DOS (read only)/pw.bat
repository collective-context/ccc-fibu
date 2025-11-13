ECHO OFF
REM HAUPT.MNU
MENU 0 0 0 ST L001 AU L000 ****************************************************************************************************
IF ERRORLEVEL 1 GOTO ENDE
CD \euro\EXE
\euro\EXE\AU2100 0000E
CD \euro\t01
PW.BAT
:ENDE
