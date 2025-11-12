echo off
if %ART% == FORM goto FORM
if %ART% == Form goto FORM
if %ART% == form goto FORM
if %ART% == MASK goto MASK
if %ART% == Mask goto MASK
if %ART% == mask goto MASK
echo %ART% ist falsche Umgebungsvariable
goto END

:FORM
\euro\exe\show ..\form\%ID%f%1.M%MAN%
goto END

:MASK
\euro\exe\show ..\mask\%ID%m%1
goto END

:END

