@echo off
REM LK-SOURCE
if exist EU_02.exe del EU_02.exe
if exist EU_02.lzh del EU_02.lzh
lharc a /x EU_02.lzh !

lharc a /x eu_02.lzh f:\euro\mask\sy\sym*.
lharc a /x eu_02.lzh f:\euro\form\sy\syf*.m
lharc a /x eu_02.lzh f:\euro\mask\st\stm*.
lharc a /x eu_02.lzh f:\euro\form\st\stf*.m
lharc a /x eu_02.lzh f:\euro\mask\fi\fim*.
lharc a /x eu_02.lzh f:\euro\form\fi\fif*.m
lharc a /x eu_02.lzh f:\euro\mask\LKf\LKfm*.
lharc a /x eu_02.lzh f:\euro\form\LKf\LKff*.m


lharc a /x eu_02.lzh f:\euro\dat\euform.043
lharc a /x eu_02.lzh f:\euro\dat\a10110.seq
lharc a /x eu_02.lzh f:\euro\dat\a10120.seq
lharc a /x eu_02.lzh f:\euro\dat\a10130.seq
lharc a /x eu_02.lzh f:\euro\dat\a10110.lex
lharc a /x eu_02.lzh f:\euro\dat\a10120.lex
lharc a /x eu_02.lzh f:\euro\dat\a10130.lex
lharc a /x eu_02.lzh f:\euro\dat\basis.lex

lharc a /x eu_02.lzh f:\euro\exe\asc2euro.exe
lharc a /x eu_02.lzh f:\euro\exe\fc.exe
lharc a /x eu_02.lzh f:\euro\exe\mc.exe
lharc a /x eu_02.lzh f:\euro\exe\eurodata.btr
lharc a /x eu_02.lzh f:\euro\exe\edit.com

lharc a /x eu_02.lzh f:\euro\hlp\eu*.043

lharc a /x eu_02.lzh f:\euro\exe\bildega.exe
lharc a /x eu_02.lzh f:\euro\exe\bildword.exe
lharc a /x eu_02.lzh f:\euro\exe\euroinst.exe
lharc a /x eu_02.lzh f:\euro\exe\katalog.exe

lharc a /x eu_02.lzh f:\euro\exe\eu1100.exe
lharc a /x eu_02.lzh f:\euro\exe\eu4100.exe
lharc a /x eu_02.lzh f:\euro\exe\eumask.043


lharc s /x EU_02.lzh
if exist EU_02.lzh del EU_02.lzh


@echo off
if exist EU_02.exe del EU_02.exe
if exist EU_02.lzh del EU_02.lzh
lharc a /x EU_02.lzh !

lharc a /x eu_02.lzh f:\euro\exe\bildega.exe
lharc a /x eu_02.lzh f:\euro\exe\bildword.exe
lharc a /x eu_02.lzh f:\euro\exe\euroinst.exe
lharc a /x eu_02.lzh f:\euro\exe\katalog.exe

lharc a /x eu_02.lzh f:\euro\bin\t02.bat
lharc a /x eu_02.lzh f:\euro\bin\ed.bat
lharc a /x eu_02.lzh f:\euro\bin\mm.bat
lharc a /x eu_02.lzh f:\euro\bin\mf.bat
lharc a /x eu_02.lzh f:\euro\mask\st\stm110
lharc a /x eu_02.lzh f:\euro\form\st\stf1100.m

lharc a /x eu_02.lzh f:\euro\exe\asc2euro.exe
lharc a /x eu_02.lzh f:\euro\exe\fc.exe
lharc a /x eu_02.lzh f:\euro\exe\mc.exe

lharc a /x eu_02.lzh f:\euro\exe\eu1100.exe
lharc a /x eu_02.lzh f:\euro\exe\eu4100.exe
lharc a /x eu_02.lzh f:\euro\exe\eumask.043

lharc a /x eu_02.lzh f:\euro\dat\euform.043
lharc a /x eu_02.lzh f:\euro\dat\a10110.seq
lharc a /x eu_02.lzh f:\euro\dat\a10110.lex
lharc a /x eu_02.lzh f:\euro\dat\basis.lex

lharc a /x eu_02.lzh f:\euro\hlp\eu*.043

lharc s /x EU_02.lzh
if exist EU_02.lzh del EU_02.lzh


