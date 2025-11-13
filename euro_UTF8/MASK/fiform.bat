@echo off
REM Fibu LK_DATA              LKFMASK
REM [├À FI 246]  [┬©LKF 247]    FIMASK oder Prefix      Source: MASK       FORM
    %1 %2  FI3100 0100EL   ├À┬© Verarbeiten Buchungen   FI4310,4311,4312
rem %1 %2  FI3110 0200LE   ├À┬© L├Âscht alle Fibu-Salden FI4320,4321
rem %1 %2  FI3110 0300EL   ├À┬© Neues Jahr anlegen/ EBK FI4340,4321
rem %1 %2  FI3110 0350EL   ├À┬© Aufruf von FI3110 0300E FI4340,4321
rem %1 %2  FI3110 0400EL   ├À┬© Saldw. EBK FI3110 0300E FI4340,4321
rem %1 %2  FI3110 0420EL   ├À┬© EBK aus VorjahresSalden FI4342,4321,4322
rem %1 %2  FI3100 0500LL    ┬© Ver. Rechnungsbuchungen FI4350,4351,4352
rem %1 %2  FI3110 0600EL   ├À┬© AFA verbuchen           FI4360

rem %1 %2  FI4100 0300EL   ├À┬© Liste Kontoklassen      FI4030,030         FI0300   0011
rem %1 %2  FI4100 0301EL   ├À┬© Liste KtoKl. jetzt/fr├╝. FI4030,030         FI0301   0011
rem %1 %2  FI4100 0302EL   ├À┬© Liste KtoKl. fr├╝her/je. FI4030,030         FI0302   0011
rem %1 %2  FI4100 1100EL   ├À┬© Liste Kunden/Kunden-Nr  FI4110             FI1100   0000
rem %1 %2  FI4100 1110EL   ├À┬© Liste Kunden/Kurzbez.   FI4111             FI1101   0010
rem %1 %2  FI4100 1120EL   ├À┬© Adressliste Kunden      FI4112,110         FI1110   0000
rem %1 %2  FI4100 1200EL   ├À┬© Liste Lieferanten/Li-Nr FI4120             FI1200   0000
rem %1 %2  FI4100 1210EL   ├À┬© Liste Lief./Kurzbez.    FI4121             FI1201   0000
rem %1 %2  FI4100 1220EL   ├À┬© Adressliste Lieferanten FI4122,120         FI1210   0000
rem %1 %2  FI4100 1300EL   ├À┬© Liste Sachkonten        FI4130,130         FI1300   0011
rem %1 %2  FI4100 1400E       Liste Banken            FI4155             FI1550   0000
rem %1 %2  FI4100 1600E    ├À┬© Liste Anlagen           FI4160             FI1600   0010
rem %1 %2  FI4100 1700E       Liste Kostenstellen     FI4170             FI1700   0000
rem %1 %2  FI4100 1710E       Liste Kostenst/-Arten   FI4171             FI1710   0000

rem %1 %2  FI4100 2100EL   ├À┬© Buchungserf.-Protokoll  FI4210             FI2100   0010
rem %1 %2  FI4100 2110EL   ├À┬© Druck Kontrolldatei     FI4211             FI2101   0010
rem %1 %2  FI4100 2105EL   ├À┬© Prot. Soll/Haben intern FI4210             FI2105   0010
rem %1 %2  FI4100 2200EE   ├À┬© Protokoll Rechnungsbu.  FI4250             FI2500   0010
rem %1 %2  FI4100 2205EE   ├À┬© Prot. Rechbuch. intern  FI4250             FI2505   0010
rem %1 %2  FI4100 2300E       Prot. Eigenverbrauch    FI4254             FI2540   0010

rem %1 %2  FI4100 3100EL   ├À┬© Journal              ┬½- FI4315             FI3100   0010
rem %1 %2  FI4100 3102EL   ├À┬© Kontrolle Journal    ┬½- FI4315             FI3102   0010
rem %1 %2  FI4100 3105EL   ├À┬© Journal intern          FI4315             FI3105   0010
rem %1 %2  FI4100 3200EL   ├À┬© Journal nach Kto-Nr./ZS FI4232             FI3110   0010
    %1 %2  fi4100 3300EE   ├À┬© Kontobl├ñtter         ┬½- FI4230             FI3200   0010
rem %1 %2  FI4110 3310EB   ├À┬© Monitor-Kontobl├ñtter ┬½- FI4231             FI3210   0010
rem %1 %2  FI4100 3400EL   ├À┬© Sachkontens./Journal ┬½- FI4232             FI3220   0011

rem %1 %2  FI4100 4100E    Ôûá  OP-Liste Kunden         FI4114,4116        FI4140   0011
rem %1 %2  FI4100 4110EE   ├À┬© Saldenliste Kunden      FI4115,4116        FI4110   0011
rem %1 %2  FI4100 4130E    Ôûá  Mahnungen Kunden        FI4113,4116        FI4130   0030
rem %1 %2  ST4100 1800E       Mahnungen               ST4410             ST2100   0030
rem %1 %2  FI4100 4200E    Ôûá  OP-Liste Lieferanten    FI4124,4126        FI4150   0011
rem %1 %2  FI4100 4210EL   ├À┬© Saldenliste Lieferanten FI4125,4126        FI4120   0011
rem %1 %2  FI4100 4230E    Ôûá  Zahlungsvorschlagliste  FI4123,4116        FI4230   0011
rem %1 %2  FI4100 4300EL   ├À┬© Sachkontensalden/Stamm  FI4132,4136        FI4100   0011
rem %1 %2  FI4100 4301EL   ├À┬© USt-Voranmeldung        FI4132,4136        FI4100   0011
rem %1 %2  FI4100 4310EL   ├À┬© Erfolgs-/Bestandskonten FI4134,4136        FI4105,4106 0011
rem %1 %2  FI4110 4320EL      Kostenvergleich         FI4135,4137        FI4160   0011
rem %1 %2  FI4100 4400EL   ├À┬© Kapitalflu├ƒrechnung     FI4140,4141        FI1400   0011
rem %1 %2  FI4110 5000E    ├À  Jahresauswertungen 1)   FI4150,150         FI1500   0000
rem %1 %2  FI4110 5001E    ├À  Jahresauswertungen 2)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 5002E    ├À  Jahresauswertungen 3)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 500aE    ├À  Jahresauswertungen 4)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 5010E    ├À  Stamm Jahresausw.       FI4145,150         FI1505   0010
rem %1 %2  FI4110 5100E    ├À  Bilanz alt              FI4151,4152        FI1510   0000
rem %1 %2  FI4100 6300EL   ├À┬© Schattensaldo           FI4432,4136        FI4400   0011
REM           Ôöé     Ôöé                                                               Ôöé
REM Programm ÔöÇÔöÿ     ÔööÔöÇÔöÇ Steuerkennzeichen                        Formular-Nummer ÔöÇÔöÇÔöÇÔöÿ


REM %1 %2  FI3100 0300E       Monatsabschlu├ƒ          FI4330,4321
REM ┬╗┬á┬½ == ASCII 0
