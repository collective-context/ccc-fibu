echo off
\msdos\brequest /D:22000
cls
echo  
echo ┌───────────────────────────────────────────────┐
echo │ [a] == VS - Konvertierung » Word-Datei        │
echo │ [b] == VS - Formular Vertriebsst.-Stammblatt  │
echo │ [c] == FI - Formular Verrechnungs-Scheck      │
echo │ [d] == ST - Liste Kunden nach Kurzbezeichnung │
echo │ [e] == ST - Liste Kunden nach Kundennummer    │
echo │ [f] == ST - Liste Kunden-Adressen             │
echo │ [g] == ST - Etikett Kunden-Adressen           │
echo │ [h] == ST - Etikett BH-Ordner Beschriftung    │
echo │ [i] == KA - Kassa-Buch                        │
echo │ [j] == SY - Daten-Lexikon                     │
echo │ [k] == AU - Angebote, Aufträge, Rechnungen .. │
echo └───────────────────────────────────────────────┘
echo  
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