@echo off
cd %user%
:INTERN
if not exist menu.btr goto ERROR
if not exist menu.org goto LIEFER
if exist menu.lie goto LIEFER
ren menu.btr menu.lie
ren menu.org menu.btr
goto ENDE

:LIEFER
if not exist menu.btr goto ERROR
if not exist menu.lie goto ERROR
if exist menu.org goto ERROR
ren menu.btr menu.org
ren menu.lie menu.btr
goto ENDE

:ERROR
echo Fehler!!! - Vorhandene MenÅdateien:
echo ˇ
if exist MENU.BTR echo MENU.BTR
if exist MENU.ORG echo MENU.ORG
if exist MENU.LIE echo MENU.LIE


:ENDE

