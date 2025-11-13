echo off
copy %USER%\euroinst %USER%\euroinst.tmp

cd \euro\exe
  set sA=%1
  SHIFT
  if X%1==X  copy %USER%\euroinst.106 %USER%\euroinst
  if X%1==X1 copy %USER%\euroinst.106 %USER%\euroinst
  if X%1==X2 copy %USER%\euroinst.206 %USER%\euroinst
  if X%1==X3 copy %USER%\euroinst.306 %USER%\euroinst
  if X%1==X8 copy %USER%\euroinst.806 %USER%\euroinst

:start
  if X%sA%==X  ST1100 6510E
  if X%sA%==Xk ST1100 6510E
  if X%sA%==Xl ST1100 6520E
  if X%sA%==Xm ST1100 6530E

:ende
  copy %USER%\euroinst.tmp %USER%\euroinst


  if X%1==Xa ST1100 1200E       ST  STAMM Lieferanten   M:120
