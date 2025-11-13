echo off
\%smd%dos\attrib -r %user%\euroinst
brequest /D:22000

if X%1 == X    goto start:
if X%1 == Xein goto ein:
if X%1 == Xaus goto aus:
if X%1 == Xnu goto norton:
echo Falsche Eingabe !
goto ende

:ein
copy %user%\euroinst.ein %user%\euroinst
goto ende

:aus
copy %user%\euroinst.aus %user%\euroinst
goto ende

:norton
copy %user%\euroinst e:
nu e:euroinst
copy e:euroinst %user%

:start
..\exe\sy1100 15

:ende
