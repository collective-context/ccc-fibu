@echo off
if exist EU_01.exe del EU_01.exe
if exist EU_01.lzh del EU_01.lzh
lharc a /x EU_01.lzh !

    lharc a /x eu_01.lzh v:\euro\mask\sy\sym*.
    lharc a /x eu_01.lzh v:\euro\form\sy\syf*.m
    lharc a /x eu_01.lzh v:\euro\mask\st\stm*.
    lharc a /x eu_01.lzh v:\euro\form\st\stf*.m
rem lharc a /x eu_01.lzh v:\euro\mask\lkf\lkfm*.
rem lharc a /x eu_01.lzh v:\euro\form\lkf\lkff*.m
    lharc a /x eu_01.lzh v:\euro\mask\fi\fim*.
    lharc a /x eu_01.lzh v:\euro\form\fi\fif*.m
    lharc a /x eu_01.lzh v:\euro\mask\ka\kam*.
    lharc a /x eu_01.lzh v:\euro\form\ka\kaf*.m
    lharc a /x eu_01.lzh v:\euro\mask\au\aum*.
    lharc a /x eu_01.lzh x:\euro\form\au\auf*.org
    lharc a /x eu_01.lzh v:\euro\mask\eu\eum*.
    lharc a /x eu_01.lzh v:\euro\form\eu\euf*.m
    lharc a /x eu_01.lzh v:\euro\form\fko\fkof*.m
    lharc a /x eu_01.lzh v:\euro\mask\fko\fkom*.

    lharc a /x eu_01.lzh v:\euro\exe\asc2euro.exe
    lharc a /x eu_01.lzh v:\euro\exe\fc.exe
    lharc a /x eu_01.lzh v:\euro\exe\mc.exe
    lharc a /x eu_01.lzh v:\euro\exe\eurodata.btr
    lharc a /x eu_01.lzh x:\euro\exe\edit.com

    lharc a /x eu_01.lzh v:\euro\exe\asc2euro.exe
    lharc a /x eu_01.lzh v:\euro\exe\fc.exe
    lharc a /x eu_01.lzh v:\euro\exe\mc.exe

    lharc a /x eu_01.lzh v:\euro\exe\eu1100.exe
    lharc a /x eu_01.lzh v:\euro\exe\eu4100.exe
    lharc a /x eu_01.lzh v:\euro\exe\eumask.043

    lharc a /x eu_01.lzh x:\euro\bin\t02.bat
    lharc a /x eu_01.lzh x:\euro\bin\ed.bat
    lharc a /x eu_01.lzh x:\euro\bin\mm.bat
    lharc a /x eu_01.lzh x:\euro\bin\mf.bat

    lharc a /x eu_01.lzh v:\euro\dat\euform.043
    lharc a /x eu_01.lzh v:\euro\dat\a10110.seq
    lharc a /x eu_01.lzh v:\euro\dat\a10120.seq
    lharc a /x eu_01.lzh v:\euro\dat\a10130.seq
    lharc a /x eu_01.lzh v:\euro\dat\a10110.lex
    lharc a /x eu_01.lzh v:\euro\dat\a10120.lex
    lharc a /x eu_01.lzh v:\euro\dat\a10130.lex
    lharc a /x eu_01.lzh v:\euro\dat\basis.lex

    lharc a /x eu_01.lzh v:\euro\hlp\eu*.043

    lharc a /x eu_01.lzh v:\euro\exe\bildega.exe
    lharc a /x eu_01.lzh v:\euro\exe\bildword.exe
    lharc a /x eu_01.lzh v:\euro\exe\euroinst.exe
    lharc a /x eu_01.lzh v:\euro\exe\katalog.exe


lharc s /x EU_01.lzh
if exist EU_01.lzh del EU_01.lzh


rem @echo off
rem if exist EU_01.exe del EU_01.exe
rem if exist EU_01.lzh del EU_01.lzh
rem lharc a /x EU_01.lzh !
rem
rem lharc a /x eu_01.lzh v:\euro\exe\bildega.exe
rem lharc a /x eu_01.lzh v:\euro\exe\bildword.exe
rem lharc a /x eu_01.lzh v:\euro\exe\euroinst.exe
rem lharc a /x eu_01.lzh v:\euro\exe\katalog.exe

rem lharc a /x eu_01.lzh v:\euro\bin\t02.bat
rem lharc a /x eu_01.lzh v:\euro\bin\ed.bat
rem lharc a /x eu_01.lzh v:\euro\bin\mm.bat
rem lharc a /x eu_01.lzh v:\euro\bin\mf.bat
rem lharc a /x eu_01.lzh v:\euro\mask\st\stm110
rem lharc a /x eu_01.lzh v:\euro\form\st\stf1100.m

rem lharc a /x eu_01.lzh v:\euro\exe\asc2euro.exe
rem lharc a /x eu_01.lzh v:\euro\exe\fc.exe
rem lharc a /x eu_01.lzh v:\euro\exe\mc.exe

rem lharc a /x eu_01.lzh v:\euro\exe\eu1100.exe
rem lharc a /x eu_01.lzh v:\euro\exe\eu4100.exe
rem lharc a /x eu_01.lzh v:\euro\exe\eumask.043

rem lharc a /x eu_01.lzh v:\euro\dat\euform.043
rem lharc a /x eu_01.lzh v:\euro\dat\a10110.seq
rem lharc a /x eu_01.lzh v:\euro\dat\a10110.lex
rem lharc a /x eu_01.lzh v:\euro\dat\basis.lex

rem lharc a /x eu_01.lzh v:\euro\hlp\eu*.043

rem lharc s /x EU_01.lzh
rem if exist EU_01.lzh del EU_01.lzh

