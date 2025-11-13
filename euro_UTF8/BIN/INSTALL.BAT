echo off
cls
echo  
echo Installationshinweis: Sie müssen auf Laufwerk A: stehen!
echo ───────────────────────────────────────────────────────────────────────── 
echo Die Installationsroutine legt auf der Festplatte selbständig das
echo Verzeichnis »\EURO« an und kopiert die Diskette. 
echo  
echo Richtige Syntax: »INSTALL Laufwerksname«  Beispiel: »INSTALL C«
echo  
echo Wenn Sie die Diskette auf Laufwerk %1: installieren wollen, dann drücken
echo Sie jetzt eine beliebige Taste.            Oder: Abbruch ist [CTRL]+[C]!
echo  
goto %1

:C
:D
:E
:F
:G
:H

pause
md %1:\dos
md %1:\euro
md %1:\euro\T01
md %1:\euro\HLP
md %1:\euro\EXE
md %1:\euro\DAT
md %1:\euro\DAT\D01
md %1:\euro\DAT\D02
md %1:\euro\DAT\D03
copy a:\arc.exe %1:\euro\exe
if not exist %1:\euro\exe\arc.exe goto error

%1:\euro\exe\arc x a:\MENU.ROO %1:\*.*
%1:\euro\exe\arc x a:\MENU.DOS %1:\dos\*.*
%1:\euro\exe\arc x a:\MENU.HLP %1:\euro\HLP\*.*
%1:\euro\exe\arc x a:\MENU.DAT %1:\euro\DAT\*.*
%1:\euro\exe\arc x a:\MENU.D01 %1:\euro\DAT\D01\*.*
%1:\euro\exe\arc x a:\MENU.D01 %1:\euro\DAT\D02\*.*
%1:\euro\exe\arc x a:\MENU.D01 %1:\euro\DAT\D03\*.*
%1:\euro\exe\arc x a:\MENU.PR2 %1:\euro\EXE\*.*

goto nachricht

:error
echo  
echo Fehler: Kopie auf »%1:« nicht möglich!
echo  
echo Richtige Syntax: »INSTALL C«
echo 
goto ende 

:nachricht
echo  
echo Ende: Diskette auf Laufwerk »%1:« fertig installiert!
echo  
echo  

:ende

