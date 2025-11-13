echo off
if X%DRUCK%==X SET DRUCK=KYO
if X%LPT%==X   SET LPT=LPT1

cls
rem map g:=euro_02/sys:
 ask "Im [P]flichtenheft, [R]echnung oder [H]andbuch [n]ormal?  " p,r,h,n
    if errorlevel 4 goto end
    if errorlevel 3 goto hand
    if errorlevel 2 goto rechnung
    if errorlevel 1 goto pflicht

 :hand
   if %DRUCK%==KYO  copy f:\euro\dru\tohp.kyo %LPT%
   if %DRUCK%==STAR copy f:\euro\dru\tohp.sta %LPT%

   H:\fnt\hp_fcopy    5  h:\fnt\KY10CN.FNT  %LPT%
   H:\fnt\hp_fcopy    6  h:\fnt\KY12CN.FNT  %LPT%
   H:\fnt\hp_fcopy   55  h:\fnt\PC10CN.FNT  %LPT%
   H:\fnt\hp_fcopy  155  h:\fnt\PC10CB.FNT  %LPT%
   H:\fnt\hp_fcopy  255  h:\fnt\PC10CR.FNT  %LPT%
   H:\fnt\hp_fcopy   70  h:\fnt\PC07PN.FNT  %LPT%
   H:\fnt\hp_fcopy   80  h:\fnt\PC08PN.FNT  %LPT%
   H:\fnt\hp_fcopy  180  h:\fnt\PC08PB.FNT  %LPT%
   H:\fnt\hp_fcopy  280  h:\fnt\PC08PR.FNT  %LPT%
   H:\fnt\hp_fcopy   11  h:\fnt\PC03LN.FNT  %LPT%
   H:\fnt\hp_fcopy   21  h:\fnt\PC06LN.FNT  %LPT%
   goto end

   ask "Softfont-Version  [P]flichtenheft, [n]ormal oder [g]roá ?  " p,n,g
      H:
      if errorlevel 3 \fnt\docujet \fnt\fontlist
      if errorlevel 2 \fnt\docujet \fnt\fontlist.005
      if errorlevel 1 \fnt\docujet \fnt\fontlist.006
      F:
   goto end

 :rechnung
   if %DRUCK%==KYO  copy f:\euro\dru\tohp.kyo %LPT%
   if %DRUCK%==STAR copy f:\euro\dru\tohp.sta %LPT%
   H:\fnt\hp_fcopy    1  H:\fnt\HV180BPN.PCP %LPT%
   H:\fnt\hp_fcopy    2  H:\fnt\HV140BPN.PCP %LPT%
   goto end

 :pflicht
   if %DRUCK%==KYO  copy f:\euro\dru\tohp.kyo %LPT%
   if %DRUCK%==STAR copy f:\euro\dru\tohp.sta %LPT%

   REM H:\fnt\hp_fcopy    5  h:\fnt\KY10CN.FNT  %LPT%
   REM H:\fnt\hp_fcopy    6  h:\fnt\KY12CN.FNT  %LPT%
       H:\fnt\hp_fcopy   55  h:\fnt\PC10CN.FNT  %LPT%
   REM H:\fnt\hp_fcopy  155  h:\fnt\PC10CB.FNT  %LPT%
       H:\fnt\hp_fcopy  255  h:\fnt\PC10CR.FNT  %LPT%
   REM H:\fnt\hp_fcopy   70  h:\fnt\PC07PN.FNT  %LPT%
       H:\fnt\hp_fcopy   80  h:\fnt\PC08PN.FNT  %LPT%
   REM H:\fnt\hp_fcopy  180  h:\fnt\PC08PB.FNT  %LPT%
   REM H:\fnt\hp_fcopy  280  h:\fnt\PC08PR.FNT  %LPT%
   REM H:\fnt\hp_fcopy   11  h:\fnt\PC03LN.FNT  %LPT%
       H:\fnt\hp_fcopy   21  h:\fnt\PC06LN.FNT  %LPT%
   goto end

 :end
