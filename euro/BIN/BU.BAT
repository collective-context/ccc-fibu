echo off
butil -reset

if not exist \euro\dat\0%App%%1.btr goto nachricht
goto anfang

:nachricht
echo \euro\dat\0%App%%1.btr nicht vorhanden!
goto ende

:anfang
if not exist \euro\dat\0%App%%1.btr goto ende
butil -stat \euro\dat\0%App%%1.btr
shift
pause
goto anfang
:ende

