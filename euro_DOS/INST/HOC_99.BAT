@echo off
if exist ar_01.exe del hoc_99.exe
if exist hoc_99.lzh del hoc_99.lzh
lharc a /x hoc_99.lzh !


lharc a /x hoc_99.lzh M:\euro\dat\syform.btr
lharc a /x hoc_99.lzh M:\euro\dat\syprint.btr

lharc a /x hoc_99.lzh M:\euro\dat\auform.043
lharc a /x hoc_99.lzh M:\euro\dat\kaform.043

lharc a /x hoc_99.lzh F:\euro\form\ka\ka*.m
lharc a /x hoc_99.lzh F:\euro\form\au\au*.m



lharc s /x hoc_99.lzh
if exist hoc_99.lzh del hoc_99.lzh


lharc a /x hoc_99.lzh F:\euro\exe\au2100.exe
lharc a /x hoc_99.lzh F:\euro\exe\au4200.exe
lharc a /x hoc_99.lzh F:\euro\exe\ka1100.exe
lharc a /x hoc_99.lzh F:\euro\exe\ka4100.exe
lharc a /x hoc_99.lzh F:\euro\exe\sy1100.exe
lharc a /x hoc_99.lzh F:\euro\exe\sy4100.exe
lharc a /x hoc_99.lzh F:\euro\exe\st1100.exe
lharc a /x hoc_99.lzh F:\euro\exe\st4100.exe
