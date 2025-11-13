@echo off
if _%1==_     map root v:=h:\%app%
if NOT _%1==_ map root v:=h:\%1

