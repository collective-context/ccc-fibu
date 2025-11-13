@echo off

if _%1==_  goto syntax
if _%1==_h goto syntax
if NOT _%2==_  goto bbs
xcopy G:\inst\%1.exe a:install.exe /v
call readme
goto ende

:bbs
REM HOTLINE
if _%2==_h echo copy G:\inst\%1.exe H:\max\file\hotline\%1.exe
if _%2==_h xcopy G:\inst\%1.exe H:\max\file\hotline\%1.exe /v
if _%2==_h goto edit 

rem UPDATE
if _%2==_u echo copy G:\inst\%1.exe H:\max\file\newup\%1.exe
if _%2==_u xcopy G:\inst\%1.exe H:\max\file\newup\%1.exe /v
if _%2==_u goto edit
goto ende

:edit
copy ! !.tmp
copy readme.roo !
lharc G:\inst\%1.exe >> !
type !! >> !
edit !

if _%2==_h lharc a H:\max\file\hotline\%1.exe !
if _%2==_u lharc a H:\max\file\newup\%1.exe !

copy !.tmp !
goto ende

:syntax
echo  
echo Syntax:  inst «Quelle» [BBS:Ziel]
echo Beispiel inst sy_01
echo Beispiel inst sy_01 h   ---» kopiert auf H:\max\file\hotline
echo Beispiel inst sy_01 u   ---» kopiert auf H:\max\file\newup
echo  

:ende

