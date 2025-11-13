@echo off
if exist temp del temp
if NOT X%2==X goto start
echo  
echo Liybary-Modell angeben. Z.B. für Large: »update %1 L«
goto ende

:start
prompt Arbeit: 

:lib
echo %2 - Modell: Jetzt wird das Update der LIB durchgeführt.
echo on
lib \euro\LIB\saa_m70%2.lib - %1.obj  >> %OUT%
@echo off
pause Einzelbibliothek-Update wurde durchgeführt.
prompt $p$g

if %OUT%==con goto dos
if %OUT%==CON goto dos
if %OUT%==Con goto dos

 :edit
  pause
   edit %OUT%

 :dos
  libcopy \euro\LIB\saa_m70%2.lib

 :ende
  echo  
  echo on


