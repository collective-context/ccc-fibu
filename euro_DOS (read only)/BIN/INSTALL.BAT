echo off
cls
echo ÿ
echo Installationshinweis: Sie mssen auf Laufwerk A:ÿstehen!
echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÿ
echo Die Installationsroutine legt auf der Festplatte selbst„ndig das
echo Verzeichnis ¯\EURO® an und kopiert die Diskette.ÿ
echo ÿ
echo Richtige Syntax: ¯INSTALL Laufwerksname®  Beispiel: ¯INSTALL C®
echo ÿ
echo Wenn Sie die Diskette auf Laufwerk %1: installieren wollen, dann drcken
echo Sie jetzt eine beliebige Taste.            Oder: Abbruch ist [CTRL]+[C]!
echo ÿ
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
echo ÿ
echo Fehler: Kopie auf ¯%1:® nicht m”glich!
echo ÿ
echo Richtige Syntax: ¯INSTALL C®
echo 
goto endeÿ

:nachricht
echo ÿ
echo Ende: Diskette auf Laufwerk ¯%1:® fertig installiert!
echo ÿ
echo ÿ

:ende

