@echo off
if exist ea_01.exe del ea_01.exe
if exist ea_01.lzh del ea_01.lzh
lharc a /x ea_01.lzh !

lharc a /x ea_01.lzh V:\euro\dat\kaform.043
lharc a /x ea_01.lzh V:\euro\dat\syp_01.btr
lharc a /x ea_01.lzh V:\euro\dat\syp_05.btr
lharc a /x ea_01.lzh X:\euro\dat\d01\ka0150.btr
lharc a /x ea_01.lzh X:\euro\dat\d03\ka0150.btr
lharc a /x ea_01.lzh X:\euro\dat\d01\1996\ka1500.btr
lharc a /x ea_01.lzh X:\euro\dat\d03\1996\ka1500.btr

lharc a /x ea_01.lzh V:\euro\hlp\kahlp??.043
lharc a /x ea_01.lzh V:\euro\exe\ka1100.exe
lharc a /x ea_01.lzh V:\euro\exe\ka4100.exe
lharc a /x ea_01.lzh V:\euro\exe\kamask.043

lharc s /x ea_01.lzh
if exist ea_01.lzh del ea_01.lzh

