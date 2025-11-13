@echo off
copy readme.kpf readme.txt
lharc a:*.exe >> readme.txt
type readme.fus >> readme.txt
copy readme.txt a:

show a:readme.txt
rem chkdsk a:
rem pause
scandisk a:
dir a:
