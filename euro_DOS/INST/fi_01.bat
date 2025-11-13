@echo off
if exist fi_01.exe del fi_01.exe
if exist fi_01.lzh del fi_01.lzh
lharc a /x fi_01.lzh !

lharc a /x fi_01.lzh x:\euro\dat\d01\ST1100.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\ST1200.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\FI1115.BT$
lharc a /x fi_01.lzh x:\euro\dat\d01\FI1115.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\FI1215.BT$
lharc a /x fi_01.lzh x:\euro\dat\d01\FI1215.BTR

lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI0130.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI0150.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI1110.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI1210.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI1310.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI1500.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\FI1600.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\ST1110.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\ST1210.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\ST1300.BTR
lharc a /x fi_01.lzh x:\euro\dat\d01\1995\SYP_50.BTR

lharc a /x fi_01.lzh V:\euro\dat\fiform.043

lharc a /x fi_01.lzh V:\euro\hlp\fihlp10.043
lharc a /x fi_01.lzh V:\euro\hlp\fiindex.043
lharc a /x fi_01.lzh V:\euro\hlp\fiprog.043

lharc a /x fi_01.lzh V:\euro\exe\fi1100.exe
lharc a /x fi_01.lzh V:\euro\exe\fi1500.exe
lharc a /x fi_01.lzh V:\euro\exe\fi3100.exe
lharc a /x fi_01.lzh V:\euro\exe\fi3110.exe
lharc a /x fi_01.lzh V:\euro\exe\fi4100.exe
lharc a /x fi_01.lzh V:\euro\exe\fi4110.exe

lharc a /x fi_01.lzh V:\euro\exe\fimask.043

lharc s /x fi_01.lzh
if exist fi_01.lzh del fi_01.lzh


rem @echo off
rem if exist fi_01.exe del fi_01.exe
rem if exist fi_01.lzh del fi_01.lzh
rem lharc a /x fi_01.lzh !

rem lharc a /x fi_01.lzh V:\euro\exe\fi3100.exe

rem lharc s /x fi_01.lzh
rem if exist fi_01.lzh del fi_01.lzh
