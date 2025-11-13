@echo off
IF X%1==X goto alles

f:\dos\xcopy %1 d:\euro\lib
goto :ende

:alles
f:\dos\xcopy \euro\LIB\???_m70L.lib d:\euro\lib

REM    xcopy \euro\LIB\eur_m70L.lib d:\euro\lib
REM    xcopy \euro\LIB\saa_m70L.lib d:\euro\lib
REM    xcopy \euro\LIB\dos_m70L.lib d:\euro\lib

:ende
echo on
prompt $p$g

