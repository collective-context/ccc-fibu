@echo off
cls
set sA=%ID%
set LPT=LPT1
if NOT %ART%==FORM goto start
if NOT exist \euro\form\%ID%\%ID%f%1.M%2 goto start
  \euro\exe\read \euro\form\%ID%\%ID%f%1.M%2 %USER%\work.001
  edit %USER%\work.001
  goto END

:start
  if X%1==Xh  goto help
  if X%1==Xhe goto help
  if X%1==Xho goto %1
  if X%1==Xbe goto %1
  if X%1==Xba goto %1
  if X%1==Xf  goto %1
  if X%1==Xm  goto %1
  if %ART% == BEI   goto BEISPIEL
  if %ART% == HOT   goto HOTLINE
  if %ART% == FORM  goto FORM
  if %ART% == MASK  goto MASK
  if %ART% == BATCH goto BATCH
  echo %ART% ist falsche Umgebungsvariable
  goto END

:BE
 SHIFT
:BEISPIEL
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  \euro\exe\read \euro\form\%ID%\%ID%b%1.M03 %USER%\work.001
  edit %USER%\work.001
  goto END

:F
 SHIFT
:FORM
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  if X%2==X goto NORMAL

  \euro\exe\read \euro\form\%ID%\%ID%f%1.M%2 %USER%\work.001
  edit %USER%\work.001
  goto END

:NORMAL
  \euro\exe\read \euro\form\%ID%\%ID%f%1.M%MAN% %USER%\work.001
  edit %USER%\work.001
  goto END

:M
 SHIFT
  :MASK
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  \euro\exe\read \euro\mask\%ID%\%ID%m%1 %USER%\work.001
  edit %USER%\work.001
  goto END

:BA
 SHIFT
  :BATCH
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  \euro\exe\read \euro\BAT\%1.B %USER%\work.001
  edit %USER%\work.001
  goto END

:HO
 SHIFT
  :HOTLINE
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  \euro\exe\read \euro\HOT\%ID%%DAT%.%1  %USER%\work.001
  edit %USER%\work.001
  call dru.bat HOT %1
  goto END

:help
  echo        Syntax: re [h/he] [m/f/be/ba/ho] [ID] «Source» [MAN]
  echo DOS-    »ART«: MASK, FORM, BEI, BATCH oder HOT
  echo Werte   »ID« : Identifikation für Masken-, Formular- u. Beispielsource.
  echo         »MAN«: Mandantenbezeichnung für Formularsource.
  echo ──────────────────────────────────────────────────────────────────────
  echo Beispiele: »ART==MASK«, »ID==ST« und »MAN==03«.     
  echo Eingabe:   re m    110     = Filtern von ..\MASK\ST\STM110.
  echo            re      110     = Filtern von ..\MASK\ST\STM110.
  echo            re   fi 111     = Filtern von ..\MASK\FI\FIM111.
  echo            re m fi 111     = Filtern von ..\MASK\FI\FIM111.
  echo  
  echo            re f    1100    = Filtern von ..\FORM\ST\STF1100.M03
  echo            re f FI 1100    = Filtern von ..\FORM\FI\FIF1100.M03
  echo            re f FI 1100 01 = Filtern von ..\FORM\FI\FIF1100.M01
  echo  
  echo Beispiele: »ART==FORM«, »ID==ST« und »MAN==03«.     
  echo            re      1100    = Filtern von ..\FORM\ST\STF1100.M03
  echo            re      1100 01 = Filtern von ..\FORM\ST\STF1100.M01
  echo            re   FI 1100    = Filtern von ..\FORM\FI\FIF1100.M03
  echo            re   FI 1100 01 = Filtern von ..\FORM\FI\FIF1100.M01
  echo  
  echo            re h         = Ausgabe dieses Hilfstextes.
  echo  
  goto end

:HOT
copy %user%\work.001  \EURO\HOT\%ID%m%1
goto abbruch

:END
set ID=%sA%

echo  
echo  ────────────────────────────────────
echo     [d] == Druck » %LPT%
echo     [s] == Support » ..\HOT\%ID%m%1
echo     [i] == Initialisieren
echo     [a] == Abbruch » DOS
echo  ────────────────────────────────────
echo  
ask "d/s/i/a " d,s,i,a
if errorlevel 4 goto abbruch
if errorlevel 3 goto init
if errorlevel 2 goto hot
copy %user%\work.001  %LPT%
call ff
goto abbruch

:init
copy \EURO\DRU\pro.%DRUCK% %LPT%
copy %user%\work.001  %LPT%
call ff

:ABBRUCH

