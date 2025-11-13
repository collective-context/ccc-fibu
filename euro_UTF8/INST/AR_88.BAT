@echo off
if exist ar_01.exe del ar_01.exe
if exist ar_01.lzh del ar_01.lzh
lharc a /x ar_01.lzh !

lharc a /x ar_01.lzh X:\euro\form\AU\*.*

lharc a /x ar_01.lzh x:\euro\dat\d01\ST1100.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\ST1200.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\ST1400.BTR

lharc a /x ar_01.lzh x:\euro\dat\d01\VS1110.BTR

lharc a /x ar_01.lzh x:\euro\dat\d01\1993\AU2100.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\AU2101.BTR

lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1110.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1210.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1300.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1330.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1700.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\ST1800.BTR
lharc a /x ar_01.lzh x:\euro\dat\d01\1993\SYP_50.BTR

lharc a /x ar_01.lzh F:\euro\exe\au2100.exe
lharc a /x ar_01.lzh F:\euro\exe\au4200.exe
lharc a /x ar_01.lzh F:\euro\exe\aumask.043
lharc a /x ar_01.lzh X:\euro\dat\auform.043
lharc a /x ar_01.lzh F:\euro\hlp\au*.043

lharc s /x ar_01.lzh
if exist ar_01.lzh del ar_01.lzh


lharc a /x ar_01.lzh F:\euro\exe\au2100.exe
lharc a /x ar_01.lzh F:\euro\exe\au4200.exe
lharc a /x ar_01.lzh F:\euro\exe\aumask.043
lharc a /x ar_01.lzh X:\euro\dat\auform.043

lharc s /x ar_01.lzh
if exist ar_01.lzh del ar_01.lzh


lharc a /x eu_01.lzh F:\euro\mask\st\stm121
