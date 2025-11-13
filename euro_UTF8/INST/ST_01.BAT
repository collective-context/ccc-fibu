@echo off
if exist st_01.exe del st_01.exe
if exist st_01.lzh del st_01.lzh
lharc a /x st_01.lzh !

lharc a /x st_01.lzh V:\euro\exe\st1100.exe
lharc a /x st_01.lzh V:\euro\exe\st4100.exe
lharc a /x st_01.lzh V:\euro\exe\stmask.043
lharc a /x st_01.lzh V:\euro\dat\stform.043
lharc a /x st_01.lzh V:\euro\dat\auform.043
lharc a /x st_01.lzh V:\euro\hlp\st*.043

lharc s /x st_01.lzh
if exist st_01.lzh del st_01.lzh
