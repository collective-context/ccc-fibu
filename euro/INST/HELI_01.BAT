@echo off
if exist heli_01.exe del heli_01.exe
if exist heli_01.lzh del heli_01.lzh
lharc a /x heli_01.lzh !

lharc a /x heli_01.lzh X:\dos\search.exe
lharc a /x heli_01.lzh X:\euro\t01\menu.exe
lharc a /x heli_01.lzh X:\euro\t01\f_back.bat
lharc a /x heli_01.lzh X:\euro\t01\d_back.bat
lharc a /x heli_01.lzh X:\euro\t01\f_rest.bat
lharc a /x heli_01.lzh X:\euro\t01\d_rest.bat

lharc a /x heli_01.lzh F:\euro\exe\fi3100.exe
lharc a /x heli_01.lzh F:\euro\exe\au2100.exe

lharc s /x heli_01.lzh
if exist heli_01.lzh del heli_01.lzh
