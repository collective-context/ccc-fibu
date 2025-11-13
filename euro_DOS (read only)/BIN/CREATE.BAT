echo off
butil -reset

if not exist \euro\mask\0%App%%1 goto nachricht
goto anfang

:nachricht
echo \euro\mask\0%App%%1 nicht vorhanden!
goto ende

:anfang
if not exist \euro\mask\0%App%%1 goto ende
butil -create \euro\dat\0%App%%1.btr \euro\mask\0%App%%1
echo ----------------------------------------------------------------
echo    FÅr \euro\mask\0%App%%1  -CREATE erfolgreich durchgefÅhrt!
echo ----------------------------------------------------------------
shift
pause
goto anfang
:ende


REM butil -create ST1100.BTR ST1100
REM
REM Inhalt von ST1100:
REM
REM record=620 variable=n key=2 page=1024 allocation=100
REM
REM position=1 length=20 duplicates=n modifiable=n type=string alternate=n
REM null=y value=0 segment=n
REM
REM position=21 length=30 duplicates=y modifiable=y type=string alternate=n
REM null=n segment=n
REM
REM ... kreiert ST1100.BTR


