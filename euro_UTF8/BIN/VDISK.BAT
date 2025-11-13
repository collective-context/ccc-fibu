echo off
REM md e:\TEST
md e:\euro
md e:\euro\LIB
md e:\euro\INCLUDE
REM md e:\BIN
REM md e:\TMP

if X%1==Xk goto klein

rem f:\dos\xcopy g:\mstools\LIB\LLIBCE.LIB    e:\euro\LIB
rem f:\dos\xcopy g:\mstools\LIB\OLDNAMES.LIB  e:\euro\LIB
f:\dos\xcopy \euro\LIB\???_M70l.LIB       e:\euro\LIB
rem os\xcopy \euro\LIB\saa_M70l.LIB       e:\euro\LIB
rem os\xcopy \euro\LIB\eur_M70l.LIB       e:\euro\LIB
rem os\xcopy \euro\LIB\dos_M70l.LIB       e:\euro\LIB

:klein
f:\dos\xcopy \euro\INCLUDE\*.h            e:\euro\INCLUDE
rem f:\dos\xcopy g:\mstools\INCLUDE\*.h       e:\euro\INCLUDE

rem xcopy g:\mstools\INCLUDE\SYS\*.h      e:\euro\INCLUDE\SYS

