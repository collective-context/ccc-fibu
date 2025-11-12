@echo off
copy exe.exe sy_02.exe

lharc a /x sy_02.exe \euro\t01\menu.exe
lharc a /x sy_02.exe \euro\exe\sy1100.exe
lharc a /x sy_02.exe \euro\exe\sy4100.exe
lharc a /x sy_02.exe \euro\exe\symask.043

lharc a /x sy_02.exe \euro\exe\error.043
lharc a /x sy_02.exe \euro\hlp\sy*.043

lharc a /x sy_02.exe \euro\dat\syform.043
lharc a /x sy_02.exe \euro\dat\sypara.btr
lharc a /x sy_02.exe \euro\dat\syp_01.btr
lharc a /x sy_02.exe \euro\dat\syp_02.btr


