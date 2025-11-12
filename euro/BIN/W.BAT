echo off
cd \euro\exe
  if X%1==Xh   goto hilfe
  if X%1==Xm   SY1100 10000    ùö Mandantenstamm          M:041
  if X%1==Xman SY1100 10000    ùö Mandantenstamm          M:041
  if X%1==Xkhk SY1100 11000    ù  Mand.Stamm šbern. KHK   M:041,043
  if X%1==Xpro SY1100 15000    ù  Anlegen Programmparam.  M:025
  if X%1==Xmen SY1100 16000    ù  Meninstallation        M:020,021
  if X%1==Xdin SY1100 17000    ù  Drucker-Installation    M:030,031,032
  if X%1==Xd   SY1100 18000    ùö Drucker-Wahl            M:035
  if X%1==Xfos SY1100 20000    ù  Formular speziell       M:033
  if X%1==Xfoa SY1100 25000    ù  Formular allgemein      M:034
  if X%1==Xmod SY1100 11000    ù  Modem-Parameter         M:028
  goto ende

:hilfe
  echo w m   SY1100 10000    ùö Mandantenstamm          M:041
  echo w man SY1100 10000    ùö Mandantenstamm          M:041
  echo w khk SY1100 11000    ù  Mand.Stamm šbern. KHK   M:041,043
  echo w pro SY1100 15000    ù  Anlegen Programmparam.  M:025
  echo w men SY1100 16000    ù  Meninstallation        M:020,021
  echo w din SY1100 17000    ù  Drucker-Installation    M:030,031,032
  echo w d   SY1100 18000    ùö Drucker-Wahl            M:035
  echo w fos SY1100 20000    ù  Formular speziell       M:033
  echo w foa SY1100 25000    ù  Formular allgemein      M:034
  echo w mod SY1100 11000    ù  Modem-Parameter         M:028

:ende

