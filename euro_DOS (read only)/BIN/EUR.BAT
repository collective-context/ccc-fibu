@echo off
edit \euro\include\eur_%1.h
if exist %LW%:\euro\include\eur_%1.h echo xcopy \euro\include\eur_%1.h %LW%:\euro\include
if exist %LW%:\euro\include\eur_%1.h xcopy \euro\include\eur_%1.h %LW%:\euro\include

