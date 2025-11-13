echo off
\msdos\brequest /D:22000
cls
echo ÿ
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ [a] == VS - Konvertierung ¯ Word-Datei        ³
echo ³ [b] == VS - Formular Vertriebsst.-Stammblatt  ³
echo ³ [c] == FI - Formular Verrechnungs-Scheck      ³
echo ³ [d] == ST - Liste Kunden nach Kurzbezeichnung ³
echo ³ [e] == ST - Liste Kunden nach Kundennummer    ³
echo ³ [f] == ST - Liste Kunden-Adressen             ³
echo ³ [g] == ST - Etikett Kunden-Adressen           ³
echo ³ [h] == ST - Etikett BH-Ordner Beschriftung    ³
echo ³ [i] == KA - Kassa-Buch                        ³
echo ³ [j] == SY - Daten-Lexikon                     ³
echo ³ [k] == AU - Angebote, Auftr„ge, Rechnungen .. ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
echo ÿ
\%smd%DOS\ask "a/b/c/d/e/f/g/h/i/j/k " a,b,c,d,e,f,g,h,i,j,k
if errorlevel 11 goto 11
if errorlevel 10 goto 10
if errorlevel  9 goto  9
if errorlevel  8 goto  8
if errorlevel  7 goto  7
if errorlevel  6 goto  6
if errorlevel  5 goto  5
if errorlevel  4 goto  4
if errorlevel  3 goto  3
if errorlevel  2 goto  2
if errorlevel  1 goto  1

:11
 \euro\exe\au2300
 goto end

:10 
 \euro\exe\st2300 7101
 goto end

:9
 \euro\exe\st2300 4001
 goto end

:8
 \euro\exe\st2300 0211
 goto end

:7
 \euro\exe\st2300 0103
 goto end

:6
 \euro\exe\st2300 0102
 goto end

:5
 \euro\exe\st2300 0101
 goto end

:4
 \euro\exe\st2300 0100
 goto end

:3
 \euro\exe\st2300 1201
 goto end

:2
 \euro\exe\st2300 0160
 goto end

:1
 \euro\exe\st2300 0170


:end
