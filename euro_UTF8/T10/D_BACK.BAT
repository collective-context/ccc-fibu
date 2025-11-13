@echo off
cls
lharc a /x /r \euro\euroback.lzh \euro\dat\*.*
pause
backup i:\euro\euroback.lzh a:
pause

