echo off
butil -reset
del  e:\test\*.btr
del  e:\test\*.049
copy \euro\dat\*.btr e:\test
copy \euro\exe\*.049 e:\test
xtree e:

