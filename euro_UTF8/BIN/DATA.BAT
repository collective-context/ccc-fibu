@echo off
cd \euro\exe
REM set temp=%id%
REM set id=euro
if X%1==Xd goto druck

:data
 if X%1==Xm \euro\exe\eu1100 01300
 if X%1==Xm goto ende

 if X%1==Xs \euro\exe\eu1100 0130S
 if X%1==Xs goto ende

 \euro\exe\eu1100 01000
goto ende

:druck
  SHIFT
  REM copy %USER%\euroinst %USER%\euroinst.tmp
  REM copy %USER%\euroinst.306 %USER%\euroinst

  if X%1==Xm \euro\exe\eu4100 05300
  if X%1==Xm goto kopie

  \euro\exe\eu4100 05000

  :kopie
  REM copy %USER%\euroinst.tmp %USER%\euroinst
goto ende

:ende
rem set id=%temp%
