@echo off
if exist fi_88.exe del fi_88.exe
if exist fi_88.lzh del fi_88.lzh
lharc a /x fi_88.lzh !

rem lharc a /x fi_88.lzh f:\euro\dat\fiform.043
rem lharc a /x fi_88.lzh f:\euro\hlp\fi*.043
rem lharc a /x fi_88.lzh f:\euro\mask\fi\fim4113
rem lharc a /x fi_88.lzh f:\euro\form\fi\fif4130.m

rem lharc a /x fi_88.lzh f:\euro\exe\fi1100.exe
rem lharc a /x fi_88.lzh f:\euro\exe\fi1500.exe
rem lharc a /x fi_88.lzh f:\euro\exe\fi3100.exe
rem lharc a /x fi_88.lzh f:\euro\exe\fi3110.exe
    lharc a /x fi_88.lzh f:\euro\exe\fi4100.exe
rem lharc a /x fi_88.lzh f:\euro\exe\fi4110.exe

rem lharc a /x fi_88.lzh f:\euro\exe\fimask.043

lharc s /x fi_88.lzh
if exist fi_88.lzh del fi_88.lzh
