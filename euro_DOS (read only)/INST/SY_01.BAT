@echo off
if exist sy_01.exe del sy_01.exe
if exist sy_01.lzh del sy_01.lzh
lharc a /x sy_01.lzh !

    lharc a /x sy_01.lzh X:\autoexec.eur
    lharc a /x sy_01.lzh X:\config.eur

    lharc a /x sy_01.lzh X:\dos\btrieve.exe
    lharc a /x sy_01.lzh X:\dos\bstop.exe
    lharc a /x sy_01.lzh X:\dos\start.eur
    lharc a /x sy_01.lzh X:\dos\butil.exe
REM lharc a /x sy_01.lzh X:\dos\ask.exe
REM lharc a /x sy_01.lzh X:\dos\choice.com
    lharc a /x sy_01.lzh X:\dos\choice.txt
    lharc a /x sy_01.lzh X:\dos\show.com
    lharc a /x sy_01.lzh X:\dos\lharc.exe
    lharc a /x sy_01.lzh X:\dos\lharc.doc
REM lharc a /x sy_01.lzh X:\dos\search.exe

    lharc a /x sy_01.lzh X:\euro\t01\menu.exe
    lharc a /x sy_01.lzh X:\euro\t01\menu.btr
    lharc a /x sy_01.lzh X:\euro\t01\pw*.bat
    lharc a /x sy_01.lzh X:\euro\t01\euroinst

    lharc a /x sy_01.lzh X:\euro\t01\f_back.bat
    lharc a /x sy_01.lzh X:\euro\t01\d_back.bat
    lharc a /x sy_01.lzh X:\euro\t01\f_rest.bat
    lharc a /x sy_01.lzh X:\euro\t01\d_rest.bat

    lharc a /x sy_01.lzh X:\euro\t01\word.bat
    lharc a /x sy_01.lzh X:\euro\t01\dos.bat
    lharc a /x sy_01.lzh X:\euro\t01\inst_01.043

    lharc a /x sy_01.lzh X:\euro\t01\euinst.bat
    lharc a /x sy_01.lzh X:\euro\t01\hi_*.bat
    lharc a /x sy_01.lzh X:\euro\exe\ff.prn
    lharc a /x sy_01.lzh X:\euro\t01\ff.bat

    lharc a /x sy_01.lzh V:\euro\hlp\sy*.043

    lharc a /x sy_01.lzh X:\euro\dat\syform.043
    lharc a /x sy_01.lzh X:\euro\dat\syform.btr
    lharc a /x sy_01.lzh X:\euro\dat\syteil.btr
    lharc a /x sy_01.lzh X:\euro\dat\syprint*.btr
    lharc a /x sy_01.lzh X:\euro\dat\sypara.btr
    lharc a /x sy_01.lzh X:\euro\dat\syp_01.btr
    lharc a /x sy_01.lzh X:\euro\dat\syp_02.btr

    lharc a /x sy_01.lzh V:\euro\exe\german_u.acs
    lharc a /x sy_01.lzh V:\euro\exe\error.043
    lharc a /x sy_01.lzh V:\euro\exe\show.com
    lharc a /x sy_01.lzh V:\euro\exe\euroinst.exe
    lharc a /x sy_01.lzh V:\euro\exe\scroll.exe

    lharc a /x sy_01.lzh V:\euro\exe\showmem.exe
    lharc a /x sy_01.lzh V:\euro\exe\sy1100.exe
    lharc a /x sy_01.lzh V:\euro\exe\sy4100.exe
    lharc a /x sy_01.lzh V:\euro\exe\symask.043

lharc s /x sy_01.lzh
if exist sy_01.lzh del sy_01.lzh
