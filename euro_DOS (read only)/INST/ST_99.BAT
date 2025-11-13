@echo off
if exist st_99.exe del st_99.exe
if exist st_99.lzh del st_99.lzh
lharc a /x st_99.lzh !

    lharc a /x st_99.lzh V:\euro\exe\st1100.exe
    lharc a /x st_99.lzh V:\euro\exe\st4100.exe
    lharc a /x st_99.lzh V:\euro\exe\stmask.043
    lharc a /x st_99.lzh V:\euro\dat\stform.043
    lharc a /x st_99.lzh V:\euro\hlp\st*.043

lharc s /x st_99.lzh
if exist st_99.lzh del st_99.lzh
