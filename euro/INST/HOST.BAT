@echo off
if exist host.exe del host.exe
if exist host.lzh del host.lzh
REM lharc a /x host.lzh !
    lharc a    host.lzh !

REM lharc a /x host.lzh H:\HOST\*.*
    lharc a    host.lzh H:\HOST\*.*

REM lharc s /x host.lzh
    lharc s    host.lzh
if exist host.lzh del host.lzh
