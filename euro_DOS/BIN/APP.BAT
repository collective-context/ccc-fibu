@echo off
edit \euro\include\app_%1.h
if exist %LW%:\euro\include\app_%1.h echo xcopy \euro\include\app_%1.h %LW%:\euro\include
if exist %LW%:\euro\include\app_%1.h xcopy \euro\include\app_%1.h %LW%:\euro\include

