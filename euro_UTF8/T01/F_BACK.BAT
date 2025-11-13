cls
@echo off
SET JAHR=1991

echo  
echo del \euro\dat\d01\%JAHR%.BAK\*.*
echo  
del \euro\dat\d01\%JAHR%.BAK\*.*
ncopy \euro\dat\d01\%JAHR%\*.* \euro\dat\d01\%JAHR%.BAK
echo  

