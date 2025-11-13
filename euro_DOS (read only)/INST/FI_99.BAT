@echo off
if exist fi_99.exe del fi_99.exe
if exist fi_99.lzh del fi_99.lzh
lharc a /x fi_99.lzh !

    lharc a /x fi_99.lzh v:\euro\dat\fiform.043
    lharc a /x fi_99.lzh v:\euro\hlp\fi*.043

    lharc a /x fi_99.lzh v:\euro\exe\fi1100.exe
    lharc a /x fi_99.lzh v:\euro\exe\fi1500.exe
    lharc a /x fi_99.lzh v:\euro\exe\fi3100.exe
    lharc a /x fi_99.lzh v:\euro\exe\fi3110.exe
    lharc a /x fi_99.lzh v:\euro\exe\fi4100.exe
    lharc a /x fi_99.lzh v:\euro\exe\fi4110.exe

    lharc a /x fi_99.lzh v:\euro\exe\fimask.043

lharc s /x fi_99.lzh
if exist fi_99.lzh del fi_99.lzh
