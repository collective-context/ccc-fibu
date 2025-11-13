@echo off
if exist ange_01.exe del ange_01.exe
if exist ange_01.lzh del ange_01.lzh
lharc a /x ange_01.lzh !

lharc a /x ange_01.lzh F:\euro\c\ba*.obj
lharc a /x ange_01.lzh F:\euro\c\ma*.obj
lharc a /x ange_01.lzh F:\euro\c\db*.obj
lharc a /x ange_01.lzh F:\euro\c\dl*.obj

lharc a /x ange_01.lzh F:\euro\c\hof*.c
lharc a /x ange_01.lzh F:\euro\c\hof*.
lharc a /x ange_01.lzh F:\euro\mask\hof\hofm*.
lharc a /x ange_01.lzh F:\euro\form\hof\hoff*.m

lharc a /x ange_01.lzh F:\euro\lib\???_M70L.LIB

lharc s /x ange_01.lzh
if exist ange_01.lzh del ange_01.lzh
