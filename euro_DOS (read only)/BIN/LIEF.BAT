echo off
copy %USER%\euroinst %USER%\euroinst.tmp

cd \euro\exe
  if X%1==X goto start
  if X%1==X1 copy %USER%\euroinst.103 %USER%\euroinst
  if X%1==X2 copy %USER%\euroinst.203 %USER%\euroinst
  if X%1==X3 copy %USER%\euroinst.303 %USER%\euroinst
  if X%1==X8 copy %USER%\euroinst.803 %USER%\euroinst

:start
  ST1100 2200E
copy %USER%\euroinst.tmp %USER%\euroinst

