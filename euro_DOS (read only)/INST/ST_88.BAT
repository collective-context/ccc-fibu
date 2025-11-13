@echo off
if exist st_88.exe del st_88.exe
if exist st_88.lzh del st_88.lzh
lharc a /x st_88.lzh !

    lharc a /x st_88.lzh F:\euro\exe\st1100.exe
rem lharc a /x st_88.lzh F:\euro\exe\st4100.exe
rem lharc a /x st_88.lzh F:\euro\exe\stmask.043
rem lharc a /x st_88.lzh F:\euro\dat\stform.043
rem lharc a /x st_88.lzh F:\euro\hlp\st*.043

lharc s /x st_88.lzh
if exist st_88.lzh del st_88.lzh
