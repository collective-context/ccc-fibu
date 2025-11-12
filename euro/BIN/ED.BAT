@echo off
cls
set sA=%ID%
set sB=%EXT%
if %EXT%==043 set EXT=
if NOT %ART%==FORM goto start
if NOT exist \euro\form\%EXT%\%ID%\%ID%f%1.M%2 goto start
  m \euro\form\%EXT%\%ID%\%ID%f%1.M%2
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
  m \euro\form\%EXT%\%ID%\%ID%b%1.M03
  goto END

:F
 SHIFT
:FORM
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  if X%2==X goto NORMAL

  m \euro\form\%EXT%\%ID%\%ID%f%1.M%2
  goto END

:NORMAL
  m \euro\form\%EXT%\%ID%\%ID%f%1.M%MAN%
  goto END

:M
 SHIFT
  :MASK
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  m \euro\mask\%EXT%\%ID%\%ID%m%1
  goto END

:BA
 SHIFT
  :BATCH
  if NOT X%2==X set ID=%1
  if NOT X%2==X SHIFT
  m \euro\BAT\%1.B
  goto END

:HO
 SHIFT
  :HOTLINE
  if NOT _%2==_ set ID=%1
  if NOT _%2==_ SHIFT
  m \euro\HOT\%EXT%\%ID%_%HOT%.%1
  call dru.bat HOT %1
  goto END

:help
  echo        Syntax: ed [h/he] [m/f/be/ba/ho] [ID] ®Source¯ [MAN]
  echo DOS-    ¯ART®: MASK, FORM, BEI, BATCH oder HOT
  echo Werte   ¯ID® : Identifikation fr Masken-, Formular- u. Beispielsource.
  echo         ¯MAN®: Mandantenbezeichnung fr Formularsource.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Beispiele: ¯ART==MASK®, ¯ID==ST® und ¯MAN==03®.    ÿ
  echo Eingabe:   ed m    110     = Editieren von ..\MASK\ST\STM110.
  echo            ed      110     = Editieren von ..\MASK\ST\STM110.
  echo            ed   fi 111     = Editieren von ..\MASK\FI\FIM111.
  echo            ed m fi 111     = Editieren von ..\MASK\FI\FIM111.
  echo ÿ
  echo            ed f    1100    = Editieren von ..\FORM\ST\STF1100.M03
  echo            ed f FI 1100    = Editieren von ..\FORM\FI\FIF1100.M03
  echo            ed f FI 1100 01 = Editieren von ..\FORM\FI\FIF1100.M01
  echo ÿ
  echo Beispiele: ¯ART==FORM®, ¯ID==ST® und ¯MAN==03®.    ÿ
  echo            ed      1100    = Editieren von ..\FORM\ST\STF1100.M03
  echo            ed      1100 01 = Editieren von ..\FORM\ST\STF1100.M01
  echo            ed   FI 1100    = Editieren von ..\FORM\FI\FIF1100.M03
  echo            ed   FI 1100 01 = Editieren von ..\FORM\FI\FIF1100.M01
  echo ÿ
  echo            ed h         = Ausgabe dieses Hilfstextes.
  echo ÿ
  goto end

:END
set ID=%sA%
set EXT=%sB%

