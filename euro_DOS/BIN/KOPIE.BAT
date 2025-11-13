echo off

if not exist \euro\mask\0%1 goto nachricht
goto anfang

:nachricht
echo \euro\mask\0%1 nicht vorhanden!
goto ende

:anfang
copy \euro\mask\0%1 \euro\mask\0%20
echo ---------------------------------------------------------------
echo    Kopie von \euro\mask\0%1 auf \euro\mask\0%2 fertig!
echo ---------------------------------------------------------------


:ende



