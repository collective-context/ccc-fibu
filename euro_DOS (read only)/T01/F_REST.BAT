cls
@echo off
SET JAHR=1991

echo ÿ
echo del \euro\dat\d01\%JAHR%\*.*
echo ÿ
del \euro\dat\d01\%JAHR%\*.*
ncopy \euro\dat\d01\%JAHR%.BAK\*.* \euro\dat\d01\%JAHR%
echo ÿ

