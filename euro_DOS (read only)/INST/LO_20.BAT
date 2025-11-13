@echo off
if exist lo_20.exe del lo_20.exe
if exist lo_20.lzh del lo_20.lzh
lharc a /x lo_20.lzh !

lharc a /x lo_20.lzh F:\euro\exe\st1100.exe
lharc a /x lo_20.lzh F:\euro\exe\eu1100.exe
lharc a /x lo_20.lzh F:\euro\exe\eu4100.exe
lharc a /x lo_20.lzh F:\euro\exe\stmask.043
lharc a /x lo_20.lzh F:\euro\exe\lomask.043
lharc a /x lo_20.lzh F:\euro\dat\loform.043
lharc a /x lo_20.lzh F:\euro\hlp\st*.043
lharc a /x lo_20.lzh F:\euro\hlp\lo*.043

lharc s /x lo_20.lzh
if exist lo_20.lzh del lo_20.lzh
