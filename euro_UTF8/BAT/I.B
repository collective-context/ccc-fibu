echo off
cls
if X%1==X  \euro\exe\euroinst.exe i
if X%1==Xi \euro\exe\euroinst.exe i
if X%1==Xd \euro\exe\euroinst.exe d
if X%1==Xh goto fehler
goto end

:fehler
echo  
echo  ──────────────────────────────────────────
echo   Erlaubte Eingabe »i [i][d]«, Fehler »%1«
echo  ──────────────────────────────────────────
echo  
echo   Steht für: »Installation, Datum«
echo  
goto end

:end
echo  

