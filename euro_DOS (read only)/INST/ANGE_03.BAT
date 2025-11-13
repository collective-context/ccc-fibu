@echo off
if exist ange_03.exe del ange_03.exe
if exist ange_03.lzh del ange_03.lzh
lharc a /x ange_03.lzh !

lharc a /x ange_03.lzh F:\euro\exe\eurodata.btr

lharc a /x ange_03.lzh F:\euro\c\eu*.c
lharc a /x ange_03.lzh F:\euro\c\eu*.
lharc a /x ange_03.lzh F:\euro\mask\ze\zem*.

lharc s /x ange_03.lzh
if exist ange_03.lzh del ange_03.lzh
