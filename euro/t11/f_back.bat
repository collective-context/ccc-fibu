@echo off
cls
if exist \euro\euroback.lzh del \euro\euroback.lzh
lharc a /x /r \euro\euroback.lzh \euro\dat\*.*
pause

