@echo off
if exist sy_99.exe del sy_99.exe
if exist sy_99.lzh del sy_99.lzh
lharc a /x sy_99.lzh !

    lharc a /x sy_99.lzh X:\euro\t01\menu.exe
    lharc a /x sy_99.lzh V:\euro\hlp\sy*.043
    lharc a /x sy_99.lzh X:\euro\dat\syform.043

    lharc a /x sy_99.lzh V:\euro\exe\error.043
    lharc a /x sy_99.lzh V:\euro\exe\show.com
    lharc a /x sy_99.lzh V:\euro\exe\euroinst.exe
    lharc a /x sy_99.lzh V:\euro\exe\scroll.exe
    lharc a /x sy_99.lzh V:\euro\exe\showmem.exe

    lharc a /x sy_99.lzh V:\euro\exe\sy1100.exe
    lharc a /x sy_99.lzh V:\euro\exe\sy4100.exe
    lharc a /x sy_99.lzh V:\euro\exe\symask.043

lharc s /x sy_99.lzh
if exist sy_99.lzh del sy_99.lzh
