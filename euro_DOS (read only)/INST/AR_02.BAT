@echo off
if exist ar_02.exe del ar_02.exe
if exist ar_02.lzh del ar_02.lzh
lharc a /x ar_02.lzh !

lharc a /x ar_02.lzh X:\euro\dat\auform.043

lharc s /x ar_02.lzh
if exist ar_02.lzh del ar_02.lzh
