echo off
cls
if X%1==X  \euro\exe\euroinst.exe i
if X%1==Xi \euro\exe\euroinst.exe i
if X%1==Xd \euro\exe\euroinst.exe d
if X%1==Xh goto fehler
goto end

:fehler
echo ÿ
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo   Erlaubte Eingabe ¯i [i][d]®, Fehler ¯%1®
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿ
echo   Steht fr: ¯Installation, Datum®
echo ÿ
goto end

:end
echo ÿ

