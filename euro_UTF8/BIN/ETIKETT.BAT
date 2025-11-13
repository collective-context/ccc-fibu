echo off
\msdos\brequest /D:22000
cls
echo  
echo  
echo ┌──────────────────────────────┐
echo │ [a] == FIBU      (5,25 Zoll) │
echo │ [b] == AUFTRAG   (5,25 Zoll) │
echo │ [c] == TEXT      (5,25 Zoll) │
echo │ [d] == TOOLS     (5,25 Zoll) │
echo │ [e] == FIBU      ( 3,5 Zoll) │
echo │ [f] == AUFTRAG   ( 3,5 Zoll) │
echo │ [g] == TEXT      ( 3,5 Zoll) │
echo │ [h] == TOOLS     ( 3,5 Zoll) │
echo │ [i] == CASEbox   ( 3,5 Zoll) │
echo │ [j] == SICHERUNG ( 3,5 Zoll) │
echo └──────────────────────────────┘
echo  
\%smd%DOS\ask "a/b/c/d/e/f/g/h " a,b,c,d,e,f,g,h,i,j
if errorlevel 10 \euro\exe\st2300 0121
if errorlevel  9 \euro\exe\st2300 0120
if errorlevel  8 \euro\exe\st2300 0119
if errorlevel  7 \euro\exe\st2300 0118
if errorlevel  6 \euro\exe\st2300 0117
if errorlevel  5 \euro\exe\st2300 0116
if errorlevel  4 \euro\exe\st2300 0114
if errorlevel  3 \euro\exe\st2300 0113
if errorlevel  2 \euro\exe\st2300 0112
if errorlevel  1 \euro\exe\st2300 0111


