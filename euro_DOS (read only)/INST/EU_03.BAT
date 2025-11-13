@echo off
REM Nur Formular „nderbar
if exist EU_03.exe del EU_03.exe
if exist EU_03.lzh del EU_03.lzh
lharc a /x EU_03.lzh !

    lharc a /x eu_03.lzh f:\euro\form\sy\syf*.m
    lharc a /x eu_03.lzh f:\euro\form\st\stf*.m
    lharc a /x eu_03.lzh f:\euro\form\fi\fif*.m
    lharc a /x eu_03.lzh f:\euro\form\ka\kaf*.m
    lharc a /x eu_03.lzh f:\euro\form\au\auf*.m
    lharc a /x eu_03.lzh f:\euro\form\eu\euf*.m

    lharc a /x eu_03.lzh f:\euro\exe\asc2euro.exe
    lharc a /x eu_03.lzh f:\euro\exe\fc.exe
    lharc a /x eu_03.lzh f:\euro\exe\edit.com

rem lharc a /x eu_03.lzh f:\euro\exe\eu1100.exe
rem lharc a /x eu_03.lzh f:\euro\exe\eu4100.exe
rem lharc a /x eu_03.lzh f:\euro\exe\eumask.043

    lharc a /x eu_03.lzh f:\euro\dat\euform.043
    lharc a /x eu_03.lzh f:\euro\dat\a10110.seq
    lharc a /x eu_03.lzh f:\euro\dat\a10120.seq
    lharc a /x eu_03.lzh f:\euro\dat\a10130.seq
    lharc a /x eu_03.lzh f:\euro\dat\a10110.lex
    lharc a /x eu_03.lzh f:\euro\dat\a10120.lex
    lharc a /x eu_03.lzh f:\euro\dat\a10130.lex
    lharc a /x eu_03.lzh f:\euro\dat\basis.lex

rem lharc a /x eu_03.lzh f:\euro\hlp\eu*.043

rem lharc a /x eu_03.lzh f:\euro\exe\bildega.exe
rem lharc a /x eu_03.lzh f:\euro\exe\bildword.exe
    lharc a /x eu_03.lzh f:\euro\exe\euroinst.exe
rem lharc a /x eu_03.lzh f:\euro\exe\katalog.exe


lharc s /x EU_03.lzh
if exist EU_03.lzh del EU_03.lzh


@echo off
if exist EU_03.exe del EU_03.exe
if exist EU_03.lzh del EU_03.lzh
lharc a /x EU_03.lzh !

lharc a /x eu_03.lzh f:\euro\exe\bildega.exe
lharc a /x eu_03.lzh f:\euro\exe\bildword.exe
lharc a /x eu_03.lzh f:\euro\exe\euroinst.exe
lharc a /x eu_03.lzh f:\euro\exe\katalog.exe

lharc a /x eu_03.lzh f:\euro\bin\t02.bat
lharc a /x eu_03.lzh f:\euro\bin\ed.bat
lharc a /x eu_03.lzh f:\euro\bin\mm.bat
lharc a /x eu_03.lzh f:\euro\bin\mf.bat
lharc a /x eu_03.lzh f:\euro\mask\st\stm110
lharc a /x eu_03.lzh f:\euro\form\st\stf1100.m

lharc a /x eu_03.lzh f:\euro\exe\asc2euro.exe
lharc a /x eu_03.lzh f:\euro\exe\fc.exe
lharc a /x eu_03.lzh f:\euro\exe\mc.exe

lharc a /x eu_03.lzh f:\euro\exe\eu1100.exe
lharc a /x eu_03.lzh f:\euro\exe\eu4100.exe
lharc a /x eu_03.lzh f:\euro\exe\eumask.043

lharc a /x eu_03.lzh f:\euro\dat\euform.043
lharc a /x eu_03.lzh f:\euro\dat\a10110.seq
lharc a /x eu_03.lzh f:\euro\dat\a10110.lex
lharc a /x eu_03.lzh f:\euro\dat\basis.lex

lharc a /x eu_03.lzh f:\euro\hlp\eu*.043

lharc s /x EU_03.lzh
if exist EU_03.lzh del EU_03.lzh



