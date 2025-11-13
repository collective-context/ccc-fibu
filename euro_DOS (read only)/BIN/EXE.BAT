echo off
del %1.tmp
ren %1.exe %1.tmp
exepack %1.tmp %1.exe
if not exist %1.exe ren %1.tmp %1.exe
pause
