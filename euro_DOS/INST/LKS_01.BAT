@echo off
if exist lks_01.exe del lks_01.exe
if exist lks_01.lzh del lks_01.lzh
lharc a /x lks_01.lzh !

lharc a /x lks_01.lzh F:\euro\exe\lks1100.exe
lharc a /x lks_01.lzh F:\euro\exe\lks4100.exe
lharc a /x lks_01.lzh F:\euro\exe\lksmask.043
lharc a /x lks_01.lzh F:\euro\dat\lksform.043
lharc a /x lks_01.lzh F:\euro\hlp\st*.043

lharc s /x lks_01.lzh
if exist lks_01.lzh del lks_01.lzh
