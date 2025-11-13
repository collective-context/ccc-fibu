echo off
if X%DOC%==X SET DOC=DOC

if not exist ..\hlp\%App%%DOC%%1.txt goto nachricht
goto anfang

:nachricht
echo ..\hlp\%App%%DOC%%1.txt nicht vorhanden!
pause
edit ..\hlp\%App%%DOC%%1.txt
goto ende

:anfang
if not exist ..\hlp\%App%%DOC%%1.txt goto ende
    edit ..\hlp\%app%%DOC%%1.txt
REM g:\word\word ..\hlp\%app%%DOC%%1.txt
shift

goto anfang
:ende



