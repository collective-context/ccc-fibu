echo off
copy %USER%\euroinst %USER%\euroinst.tmp

cd \euro\exe
  if X%1==X  copy %USER%\euroinst.103 %USER%\euroinst
  if X%1==Xp copy %USER%\euroinst.103 %USER%\euroinst
  if X%1==X1 copy %USER%\euroinst.103 %USER%\euroinst
  if X%1==X2 copy %USER%\euroinst.203 %USER%\euroinst
  if X%1==X3 copy %USER%\euroinst.303 %USER%\euroinst
  if X%1==X8 copy %USER%\euroinst.803 %USER%\euroinst

:start
  if X%1==Xp EU1100 4500E
  if X%1==Xp goto ende

  EU1100 4300E

  REM set aptmp=%app%
  REM set app=lo
  REM call test 130
  REM set app=%aptmp%

:ende
copy %USER%\euroinst.tmp %USER%\euroinst

