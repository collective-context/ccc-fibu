@echo off
cls
if exist \euro\hot\*.bak del \euro\hot\*.bak
if _%1==_s goto meldung
if _%1==_h goto fehler
if _%1==_d goto dir
if _%1==_l goto dir_last
if _%1==_a goto dir_alle
if _%1==_p goto pack

set hot=%1
  if _%1==_ goto fehler
  goto meldung

:fehler
  echo  
  echo Werte    : ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  echo Syntax    : hot [JJMM]           oder: hot [s/h]   (show/help)
  echo für [JJMM]: Jahr/Monat                 hot [d/l/a] (dir/last/all)
  echo                                        hot [p]     (pack with LHARC)
  echo  
  echo Beispiele: »hot 9312«, »hot 9302« oder »hot l« ...
  echo  
  goto down

:meldung
  echo  
  echo Werte:  ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  echo  
  goto down

:dir
  echo Werte:  ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  if exist \euro\HOT\%ID%_*.* dir \euro\HOT\%ID%_*.* | sort | more
  goto down

:dir_last
  echo Werte:  ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  if exist \euro\HOT\%ID%_%HOT%.* dir \euro\HOT\%ID%_%HOT%.* | sort | more
  goto down

:dir_alle
  echo Werte:  ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  if exist \euro\HOT\??_*.* dir \euro\HOT\??_*.* | sort | more
  pause > NULL
  if exist \euro\HOT\???_*.* dir \euro\HOT\???_*.* | sort | more
  goto down

:pack
  echo Werte:  ID=%id%  HOT=%hot%   (ART=%art% / LPT=%lpt%)
  echo ─────────────────────────────────────────────────────────────────
  if exist \euro\HOT\%ID%_%HOT%.* dir \euro\HOT\%ID%_%HOT%.* | sort | more
  echo  
  \DOS\ask "Dateien mit LHARC packen? [j/n] " j,n
  echo  
  if errorlevel 2 goto down

if exist \euro\HOT\_%ID%%HOT%.lzh del \euro\HOT\_%ID%%HOT%.lzh
if exist \euro\HOT\_%ID%ALL.lzh   del \euro\HOT\_%ID%ALL.lzh
if exist _%ID%%HOT%.exe del _%ID%%HOT%.exe
if exist _%ID%ALL.exe   del _%ID%ALL.exe
lharc a    \euro\HOT\_%ID%%HOT%.lzh \euro\HOT\!
lharc a    \euro\HOT\_%ID%ALL.lzh   \euro\HOT\!

lharc a    \euro\HOT\_%ID%%HOT%.lzh \euro\HOT\%ID%_%HOT%.*
lharc a    \euro\HOT\_%ID%ALL.lzh   \euro\HOT\%ID%_*.*

lharc s    \euro\HOT\_%ID%%HOT%.lzh
lharc s    \euro\HOT\_%ID%ALL.lzh

echo  
\DOS\ask "Dateien in die WINware Mailbox legen? [j/n] " j,n
echo  
if errorlevel 2 goto down

copy _%ID%%HOT%.exe h:\max\file\HOTLINE\%ID%_%HOT%.exe
copy _%ID%ALL.exe h:\max\file\HOTLINE\%ID%_ALL.exe

:down
if exist \euro\HOT\_%ID%%HOT%.lzh del \euro\HOT\_%ID%%HOT%.lzh
if exist \euro\HOT\_%ID%ALL.lzh del \euro\HOT\_%ID%ALL.lzh
if exist _%ID%%HOT%.exe del _%ID%%HOT%.exe
if exist _%ID%ALL.exe   del _%ID%ALL.exe


  pause > NULL
  goto meldung
