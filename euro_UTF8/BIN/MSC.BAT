@echo off
    SET OUT=CON
REM SET OUT=LPT1
rem SET OUT=PROTO.TXT

cls
rem bstop
if X%MSC%==X set MSC=CPP
set sA=L
set sB=

rem set STACK=30000
rem set STACK=10000
    set STACK=8000
rem set STACK=7200
rem set STACK=6500
rem set STACK=5500

if X%1==Xm set STACK=15000
if X%1==Xg set STACK=30000
if X%1==Xm SHIFT
if X%1==Xg SHIFT

if X%1==Xs set sA=S
if X%1==Xs SHIFT

REM set So=G:\euro\DEMO
REM set So=
    set So=\euro\C
rem set So=\case\C

rem /* cd %LW%:\euro\lib */
cd %So%
if exist %USER%\TEMP del %USER%\TEMP

          set LiAn=%App%%2
if X%2==X set LiAn=%App%%1

 echo User: ¯%user%®  /  Source-File: ¯%So%\%ID%%1.%MSC%® / ¯%LiAn%®
 echo ÿ
   if %sA%==S set LIB=F:\euro\lib
   if %sA%==S set CoOp=  -Zpi -Otials  -G2    -Gs -A%sA% -c -W4
   if %sA%==S set LiOp=/NOD /E /F
   if %sA%==S goto frage

rem set CoOp=-Zpi -Ol1s -G2 -Gt -Ge -A%sA% -c -W4
rem set CoOp=-Zpi -Od   -G3 -Gt -Ge -A%sA% -c -W4
    set CoOp=-Zpi -Ox   -G3 -Gt -Ge -A%sA% -c -W4

   \euro\bin\ask "[O]hne Optimierung, mit [D]ebuginfo oder [W]/[L]iefer-Version? " d,l,o,w
rem f errorlevel 4 set CoOp=-Mq -Zpi -Oals -G2 -Gt120 -Ge -A%sA% -c -W4
   if errorlevel 4 set LiOp=/NOD /E /F /OL /SE:300 /ST:%STACK%
   if errorlevel 4 goto frage

   if errorlevel 3 set CoOp=-Zpi -Ot   -G3 -Gt -Ge -A%sA% -c -W4
   if errorlevel 3 set LiOp=/NOD /E /F /OL /SE:300 /ST:%STACK%
   if errorlevel 3 goto frage

   if errorlevel 2 set CoOp=-Zpi -Ox   -G3 -Gt -Ge -A%sA% -c -W4
   if errorlevel 2 set LiOp=/NOD /E /F /OL /SE:300 /ST:%STACK%
   if errorlevel 2 goto frage

   if errorlevel 1 set sB=BC4+
rem f errorlevel 1 set CoOp=-Zpi -Od -G2 -Gt -Ge -A%sA% -c -W4
   if errorlevel 1 set LiOp=/NOD /CO /OL /MA /SE:300 /ST:%STACK%
   if errorlevel 1 goto frage
   REM -Zg=New Style Prototype
   REM -Zp=Set structure packing        -Ze=Enable MS language extensions
   REM -Zi=Code View ein                -Ox=-Gs und folgende vier Optionen:
   REM -Oa lockere sonstige Kontrollen, -Oi=Generieren interne Routienen ein
   REM -Ol Schleifenoptimierung ein,    -Ot=gr”áte Geschwindigkeit
   REM -Oq P-Code ein
   REM -Od=Optimierung aus,             -Ge=Aufruf der Stack-Prfung
   REM -Gt=Daten in neues Segment       -Gs=Nimmt Stack-Checkingroutinen zurck
   REM -G2=Generiert 80286 Code
   REM -G3=Generiert 80386 Code

 :frage
 echo ÿ
 \euro\bin\ask "Nur [c]ompilieren oder nur [l]inken, oder [b]eides? " c,l,b
   if errorlevel 3 goto beides
   if errorlevel 2 goto linker
   if errorlevel 1 goto compiler

 :beides
 echo ÿ
 if exist %ID%%1.obj del %ID%%1.obj
 REM  cl -EM %CoOp%  %So%\%ID%%1.%MSC% >> %OUT%
 echo cl     %CoOp%  %So%\%ID%%1.%MSC% >> %OUT%
      cl     %CoOp%  %So%\%ID%%1.%MSC% >> %OUT%

 :linker
 rem echo if "%So%"=="\case\C" copy %So%\%ID%%1.obj \euro\c
 if "%So%"=="\case\C" copy %So%\%ID%%1.obj \euro\c
 if "%So%"=="\case\C" cd \euro\C
 if "%So%"=="\case\C" set So=\euro\C
 echo ÿ
 echo Linken im %sA%-Modell.ÿ
   if %sA%==h set sA=L
   if %sA%==H set sA=L

REM set link=%LiOp%
 echo ÿ
set sC=..\EXE\%LiAn%.EXE,..\exe\,@%USER%\%LW%_M80_%sA%.LNK;
echo on
link %LiOp% %sB%@%So%\%LiAn% %sC%
rem link %LiOp% %sB%@%So%\%LiAn% %sC%>TEMP
echo off
rem link %LiOp% %sB%@%So%\%LiAn% ..\EXE\%LiAn%.EXE,+..\exe\,@%USER%\%LW%_M80_%sA%.LNK;>>%OUT%
rem   link %LiOp% @%So%\%LiAn% ..\EXE\%LiAn%.EXE, ,@%USER%\%LW%_M80_%sA%.LNK, %So%\%LiAn%.DEF;
   goto end

 :compiler
 echo ÿ
 if exist %ID%%1.obj del %ID%%1.obj
 rem  cl -EM %CoOp%  %So%\%ID%%1.%MSC%  >> %OUT%
 echo cl     %CoOp%  %So%\%ID%%1.%MSC%  >> %OUT%
      cl     %CoOp%  %So%\%ID%%1.%MSC%  >> %OUT%
      goto end

 :end
 echo ÿ
  rem cd %user%
  if %OUT%==CON goto dos

 :edit
 pause
 m %OUT%

 :dos
