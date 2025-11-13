@echo off
if "%1"=="" goto start
set app=LI
goto %1

:start

:1
set id=LI
call mm 110 110 120 121 4110


:2
set id=SY
call mm 036


