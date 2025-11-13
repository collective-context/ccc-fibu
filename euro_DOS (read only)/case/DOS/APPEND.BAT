prompt Arbeit: 
echo on
cls
cl -qc -Zr -c -Ox -W1 -A%2 %1.c
echo off
:lib
echo Das war das Kompilieren im %2 - Modell.
echo %2 - Modell: Jetzt wird die Datei %1.OBJ an die LIB angehÑngt.
echo on
pause
lib \euro\LIB\saa_m51%2.lib + %1.obj;
REM lib \euro\LIB\eur_m51%2.lib + %1.obj;
del *.obj
pause Einzelbibliothek-Append ausgefÅhrt.
prompt $p$g
libcopy
echo on

