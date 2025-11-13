ECHO OFF
REM HAUPT.MNU
MENU 0 0 0 ST L001 ST L899 ****************************************************************************************************
IF ERRORLEVEL 1 GOTO ENDE
@echo off
cls
set sA=\euro\lzh
set sB=d01
set sC=1997
set sD=\euro\dat

v:
del %sA%\%sB%_%sC%.lzh
lharc a /x %sA%\%sB%_%sC%.lzh %sD%\%sB%\fi*.btr
lharc a /x %sA%\%sB%_%sC%.lzh %sD%\%sB%\%sC%\*.btr
pause

REM Kundenversion:
REM --------------
REM @echo off
REM cls
REM if exist \euro\euroback.lzh del \euro\euroback.lzh
REM lharc a /x /r \euro\euroback.lzh \euro\dat\*.*
REM pause
CD \euro\t02
PW.BAT
:ENDE
