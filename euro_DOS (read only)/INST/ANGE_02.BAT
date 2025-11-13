@echo off
if exist ange_02.exe del ange_02.exe
if exist ange_02.lzh del ange_02.lzh
lharc a /x ange_02.lzh !

lharc a /x ange_02.lzh f:\euro\exe\asc2euro.exe
lharc a /x ange_02.lzh f:\euro\exe\fc.exe
lharc a /x ange_02.lzh f:\euro\exe\mc.exe
lharc a /x ange_02.lzh f:\euro\exe\eurodata.btr

lharc s /x ange_02.lzh
if exist ange_02.lzh del ange_02.lzh
