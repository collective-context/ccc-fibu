cls
@echo off
SET JAHR=1991

echo ÿ
echo del \euro\dat\d01\%JAHR%.BAK\*.*
echo ÿ
del \euro\dat\d01\%JAHR%.BAK\*.*
ncopy \euro\dat\d01\%JAHR%\*.* \euro\dat\d01\%JAHR%.BAK
echo ÿ

