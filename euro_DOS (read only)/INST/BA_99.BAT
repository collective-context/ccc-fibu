@echo off
if exist ba_99.exe del ba_99.exe
if exist ba_99.lzh del ba_99.lzh
lharc a /x ba_99.lzh !

    lharc a /x ba_99.lzh X:\autoexec.eur
    lharc a /x ba_99.lzh X:\config.eur

    lharc a /x ba_99.lzh X:\dos\btrieve.exe
    lharc a /x ba_99.lzh X:\dos\btrieve.net
    lharc a /x ba_99.lzh X:\dos\bstop.exe
    lharc a /x ba_99.lzh X:\dos\start.eur
    lharc a /x ba_99.lzh X:\dos\butil.exe
rem lharc a /x ba_99.lzh X:\dos\ask.exe
rem lharc a /x ba_99.lzh X:\dos\choice.com
    lharc a /x ba_99.lzh X:\dos\choice.txt
    lharc a /x ba_99.lzh X:\dos\show.com
    lharc a /x ba_99.lzh X:\dos\lharc.exe
    lharc a /x ba_99.lzh X:\dos\lharc.doc
rem lharc a /x ba_99.lzh X:\dos\search.exe

    lharc a /x ba_99.lzh X:\euro\t01\menu.exe
rem lharc a /x ba_99.lzh X:\euro\t01\menu.btr
    lharc a /x ba_99.lzh X:\euro\t01\pw*.bat
    lharc a /x ba_99.lzh X:\euro\t01\euroinst

rem lharc a /x ba_99.lzh X:\euro\t01\f_back.bat
rem lharc a /x ba_99.lzh X:\euro\t01\d_back.bat
rem lharc a /x ba_99.lzh X:\euro\t01\f_rest.bat
rem lharc a /x ba_99.lzh X:\euro\t01\d_rest.bat

rem lharc a /x ba_99.lzh X:\euro\t01\word.bat
rem lharc a /x ba_99.lzh X:\euro\t01\dos.bat
    lharc a /x ba_99.lzh X:\euro\t01\inst_01.043

    lharc a /x ba_99.lzh X:\euro\t01\euinst.bat
    lharc a /x ba_99.lzh X:\euro\t01\hi_*.bat
rem lharc a /x ba_99.lzh X:\euro\exe\ff.prn
rem lharc a /x ba_99.lzh X:\euro\t01\ff.bat

    lharc a /x ba_99.lzh v:\euro\hlp\sy*.043

    lharc a /x ba_99.lzh X:\euro\dat\syform.043
    lharc a /x ba_99.lzh X:\euro\dat\syform._tr
rem lharc a /x ba_99.lzh X:\euro\dat\syteil.btr
rem lharc a /x ba_99.lzh X:\euro\dat\syprint*.btr
rem lharc a /x ba_99.lzh X:\euro\dat\sypara.btr
rem lharc a /x ba_99.lzh X:\euro\dat\syp_01.btr
rem lharc a /x ba_99.lzh X:\euro\dat\syp_02.btr

    lharc a /x ba_99.lzh V:\euro\exe\german_u.acs
    lharc a /x ba_99.lzh V:\euro\exe\error.043
    lharc a /x ba_99.lzh V:\euro\exe\show.com
    lharc a /x ba_99.lzh V:\euro\exe\euroinst.exe
    lharc a /x ba_99.lzh V:\euro\exe\scroll.exe

    lharc a /x ba_99.lzh V:\euro\exe\showmem.exe
    lharc a /x ba_99.lzh V:\euro\exe\sy1100.exe
    lharc a /x ba_99.lzh V:\euro\exe\sy4100.exe
    lharc a /x ba_99.lzh V:\euro\exe\symask.043

    lharc a /x ba_99.lzh V:\euro\exe\st1100.exe
    lharc a /x ba_99.lzh V:\euro\exe\st4100.exe
    lharc a /x ba_99.lzh V:\euro\exe\stmask.043
    lharc a /x ba_99.lzh V:\euro\dat\stform.043
    lharc a /x ba_99.lzh X:\euro\dat\auform._43
    lharc a /x ba_99.lzh V:\euro\hlp\st*.043

lharc s /x ba_99.lzh
if exist ba_99.lzh del ba_99.lzh
