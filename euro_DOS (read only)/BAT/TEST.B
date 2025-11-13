@echo off
cls
echo ÿ
echo ÿ
echo ÿ
IF NOT X%sA%==X SET sA=
IF NOT X%sB%==X SET sB=
cd \euro\exe
  if X%1==X  goto art
  if  %1==?  goto help
  if  %1==e  goto end
  if  %1==b  goto bounds
  if  %1==c  goto code_view
  if  %1==m  goto start_1
  if  %1==f  goto start_1
  goto %ART%

:art
  if %ART%==MASK goto mask
  if %ART%==FORM goto form
  goto fehler

:bounds
  SET sA=bc
  SHIFT
  goto start_1

:code_view
  SET sA=cv
  SET sB=/E
  SHIFT

:start_1
  if X%1==X goto art
  if  %1==h goto help
  if  %1==c goto end
  if  %1==m goto start_2
  if  %1==f goto start_2
  goto %ART%

:start_2
  if X%2==X goto %1
  if %1==m goto mask_init
  if %1==f goto form_init
  goto fehler

:mask_init
  shift
  goto mask

:form_init
  shift
  goto form

:mask
  @echo %sA% %sB% EU1100 998 /%app% %1 %2 %3 %4 %5
  @echo off
  %sA% %sB% EU1100 998 /%app% %1 %2 %3 %4 %5
  goto end

:form
  @echo %sA% %sB% FI4100 998 /%app% %1 %2 %3 %4 %5
  @echo off
  %sA% %sB% FI4100 998 /%app% %1 %2 %3 %4 %5
  goto end

:fehler
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo Es ist ein Fehler aufgetreten! (1=%1,  ART=%art%,  APP=%app%)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ
  goto end

:help
  echo ÿ
  echo        Syntax:   t [b/c] [e/?] [m/f] [Steuerkennzeichen]
  echo DOS-Variablen:   ¯ART® und ¯APP®
  echo ÿ
  echo Beispiele:  (Vorsicht, Hilfstext ist nicht gltig!!!)
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo test [b/c] e       == CD ins EXE-Verzeichnis.
  echo test [b/c] ?       == CD ins EXE-Verzeichnis - Ausgabe dieses Hilfs-Textes.
  echo ÿ
  echo test [b/c]         == call \EURO\MASK\¯APP®MASK.BAT - ¯ART®==mask
  echo test [b/c]         == call \EURO\MASK\¯APP®FORM.BAT - ¯ART®==form
  echo test [b/c] m       == call \EURO\MASK\¯APP®MASK.BAT
  echo test [b/c] f       == call \EURO\FORM\¯APP®FORM.BAT
  echo ÿ
  echo test [b/c]   10    == ¯APP®1100.EXE 10              - ¯ART®==mask
  echo test [b/c]   1010L == ¯APP®4100.EXE 1010L           - ¯ART®==form
  echo test [b/c] m 10    == ¯APP®1100.EXE 10
  echo test [b/c] f 1010L == ¯APP®4100.EXE 1010L
  echo ÿ
  echo Mit ¯b® erfolgt der Start mit dem Bereichs-Prfprogamm (bounds-checking).
  echo Mit ¯c® erfolgt der Start mit dem Code-View-Debuger.
  echo ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  echo ÿ
  goto end

:end

