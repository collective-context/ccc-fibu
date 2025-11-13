@echo off
%DEFA%
cls
echo  
echo  
echo  
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
  if %ART%==MASK goto m
  if %ART%==FORM goto f
  goto fehler

:bounds
  SET sA=bchk
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
  echo %sA% %sB% %APP%1100 %1
  %sA% %sB% %APP%1100 %1
  goto end

:form
  echo %sA% %sB% %APP%4100 %1
  %sA% %sB% %APP%4100 %1
  goto end

:m
  @echo \EURO\MASK\%APP%MASK.BAT %sA% %sB%
  @echo off
    call \EURO\MASK\%APP%MASK.BAT %sA% %sB%
  REM %1 %2 uab1100 20     * »Ändern Betriebe         M:120,121,122,123
  goto end

:f
  @echo \EURO\MASK\%APP%FORM.BAT %sA% %sB%
  @echo off
    call \EURO\MASK\%APP%FORM.BAT %sA% %sB%
  goto end

:fehler
  echo ─────────────────────────────────────────────────────────────────────────
  echo Es ist ein Fehler aufgetreten! (1=%1,  ART=%art%,  APP=%app%)
  echo ─────────────────────────────────────────────────────────────────────────
  echo  
  goto end

:help
  echo  
  echo        Syntax:   t [b/c] [e/?] [m/f] [Steuerkennzeichen]
  echo DOS-Variablen:   »ART« und »APP«
  echo  
  echo Beispiele:
  echo ─────────────────────────────────────────────────────────────────────────
  echo t [b/c] e       == CD ins EXE-Verzeichnis.
  echo t [b/c] ?       == CD ins EXE-Verzeichnis - Ausgabe dieses Hilfs-Textes.
  echo  
  echo t [b/c]         == call \EURO\MASK\»APP«MASK.BAT - »ART«==mask
  echo t [b/c]         == call \EURO\MASK\»APP«FORM.BAT - »ART«==form
  echo t [b/c] m       == call \EURO\MASK\»APP«MASK.BAT
  echo t [b/c] f       == call \EURO\FORM\»APP«FORM.BAT
  echo  
  echo t [b/c]   10    == »APP«1100.EXE 10              - »ART«==mask
  echo t [b/c]   1010L == »APP«4100.EXE 1010L           - »ART«==form
  echo t [b/c] m 10    == »APP«1100.EXE 10
  echo t [b/c] f 1010L == »APP«4100.EXE 1010L
  echo  
  echo Mit »b« erfolgt der Start mit dem Bereichs-Prüfprogamm (bounds-checking).
  echo Mit »c« erfolgt der Start mit dem Code-View-Debuger.
  echo ─────────────────────────────────────────────────────────────────────────
  echo Siehe auch: TE.BAT - Damit kann die CALL-Batchdatei editiert werden.
  echo  
  goto end

:end

