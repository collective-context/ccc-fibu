@echo off
if exist lo_01.exe del lo_01.exe
if exist lo_01.lzh del lo_01.lzh
lharc a /x lo_01.lzh !

lharc a /x lo_01.lzh F:\euro\exe\eu1100.exe
lharc a /x lo_01.lzh F:\euro\exe\eu4100.exe
lharc a /x lo_01.lzh F:\euro\exe\lomask.043
lharc a /x lo_01.lzh F:\euro\dat\loform.043
lharc a /x lo_01.lzh F:\euro\hlp\lo*.043

lharc s /x lo_01.lzh
if exist lo_01.lzh del lo_01.lzh
