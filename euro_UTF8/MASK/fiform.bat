@echo off
REM Fibu LK_DATA              LKFMASK
REM [÷ FI 246]  [¸LKF 247]    FIMASK oder Prefix      Source: MASK       FORM
    %1 %2  FI3100 0100EL   ÷¸ Verarbeiten Buchungen   FI4310,4311,4312
rem %1 %2  FI3110 0200LE   ÷¸ Löscht alle Fibu-Salden FI4320,4321
rem %1 %2  FI3110 0300EL   ÷¸ Neues Jahr anlegen/ EBK FI4340,4321
rem %1 %2  FI3110 0350EL   ÷¸ Aufruf von FI3110 0300E FI4340,4321
rem %1 %2  FI3110 0400EL   ÷¸ Saldw. EBK FI3110 0300E FI4340,4321
rem %1 %2  FI3110 0420EL   ÷¸ EBK aus VorjahresSalden FI4342,4321,4322
rem %1 %2  FI3100 0500LL    ¸ Ver. Rechnungsbuchungen FI4350,4351,4352
rem %1 %2  FI3110 0600EL   ÷¸ AFA verbuchen           FI4360

rem %1 %2  FI4100 0300EL   ÷¸ Liste Kontoklassen      FI4030,030         FI0300   0011
rem %1 %2  FI4100 0301EL   ÷¸ Liste KtoKl. jetzt/frü. FI4030,030         FI0301   0011
rem %1 %2  FI4100 0302EL   ÷¸ Liste KtoKl. früher/je. FI4030,030         FI0302   0011
rem %1 %2  FI4100 1100EL   ÷¸ Liste Kunden/Kunden-Nr  FI4110             FI1100   0000
rem %1 %2  FI4100 1110EL   ÷¸ Liste Kunden/Kurzbez.   FI4111             FI1101   0010
rem %1 %2  FI4100 1120EL   ÷¸ Adressliste Kunden      FI4112,110         FI1110   0000
rem %1 %2  FI4100 1200EL   ÷¸ Liste Lieferanten/Li-Nr FI4120             FI1200   0000
rem %1 %2  FI4100 1210EL   ÷¸ Liste Lief./Kurzbez.    FI4121             FI1201   0000
rem %1 %2  FI4100 1220EL   ÷¸ Adressliste Lieferanten FI4122,120         FI1210   0000
rem %1 %2  FI4100 1300EL   ÷¸ Liste Sachkonten        FI4130,130         FI1300   0011
rem %1 %2  FI4100 1400E       Liste Banken            FI4155             FI1550   0000
rem %1 %2  FI4100 1600E    ÷¸ Liste Anlagen           FI4160             FI1600   0010
rem %1 %2  FI4100 1700E       Liste Kostenstellen     FI4170             FI1700   0000
rem %1 %2  FI4100 1710E       Liste Kostenst/-Arten   FI4171             FI1710   0000

rem %1 %2  FI4100 2100EL   ÷¸ Buchungserf.-Protokoll  FI4210             FI2100   0010
rem %1 %2  FI4100 2110EL   ÷¸ Druck Kontrolldatei     FI4211             FI2101   0010
rem %1 %2  FI4100 2105EL   ÷¸ Prot. Soll/Haben intern FI4210             FI2105   0010
rem %1 %2  FI4100 2200EE   ÷¸ Protokoll Rechnungsbu.  FI4250             FI2500   0010
rem %1 %2  FI4100 2205EE   ÷¸ Prot. Rechbuch. intern  FI4250             FI2505   0010
rem %1 %2  FI4100 2300E       Prot. Eigenverbrauch    FI4254             FI2540   0010

rem %1 %2  FI4100 3100EL   ÷¸ Journal              «- FI4315             FI3100   0010
rem %1 %2  FI4100 3102EL   ÷¸ Kontrolle Journal    «- FI4315             FI3102   0010
rem %1 %2  FI4100 3105EL   ÷¸ Journal intern          FI4315             FI3105   0010
rem %1 %2  FI4100 3200EL   ÷¸ Journal nach Kto-Nr./ZS FI4232             FI3110   0010
    %1 %2  fi4100 3300EE   ÷¸ Kontoblätter         «- FI4230             FI3200   0010
rem %1 %2  FI4110 3310EB   ÷¸ Monitor-Kontoblätter «- FI4231             FI3210   0010
rem %1 %2  FI4100 3400EL   ÷¸ Sachkontens./Journal «- FI4232             FI3220   0011

rem %1 %2  FI4100 4100E    ■  OP-Liste Kunden         FI4114,4116        FI4140   0011
rem %1 %2  FI4100 4110EE   ÷¸ Saldenliste Kunden      FI4115,4116        FI4110   0011
rem %1 %2  FI4100 4130E    ■  Mahnungen Kunden        FI4113,4116        FI4130   0030
rem %1 %2  ST4100 1800E       Mahnungen               ST4410             ST2100   0030
rem %1 %2  FI4100 4200E    ■  OP-Liste Lieferanten    FI4124,4126        FI4150   0011
rem %1 %2  FI4100 4210EL   ÷¸ Saldenliste Lieferanten FI4125,4126        FI4120   0011
rem %1 %2  FI4100 4230E    ■  Zahlungsvorschlagliste  FI4123,4116        FI4230   0011
rem %1 %2  FI4100 4300EL   ÷¸ Sachkontensalden/Stamm  FI4132,4136        FI4100   0011
rem %1 %2  FI4100 4301EL   ÷¸ USt-Voranmeldung        FI4132,4136        FI4100   0011
rem %1 %2  FI4100 4310EL   ÷¸ Erfolgs-/Bestandskonten FI4134,4136        FI4105,4106 0011
rem %1 %2  FI4110 4320EL      Kostenvergleich         FI4135,4137        FI4160   0011
rem %1 %2  FI4100 4400EL   ÷¸ Kapitalflußrechnung     FI4140,4141        FI1400   0011
rem %1 %2  FI4110 5000E    ÷  Jahresauswertungen 1)   FI4150,150         FI1500   0000
rem %1 %2  FI4110 5001E    ÷  Jahresauswertungen 2)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 5002E    ÷  Jahresauswertungen 3)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 500aE    ÷  Jahresauswertungen 4)   FI4150,150         FI1500   0010
rem %1 %2  FI4110 5010E    ÷  Stamm Jahresausw.       FI4145,150         FI1505   0010
rem %1 %2  FI4110 5100E    ÷  Bilanz alt              FI4151,4152        FI1510   0000
rem %1 %2  FI4100 6300EL   ÷¸ Schattensaldo           FI4432,4136        FI4400   0011
REM           │     │                                                               │
REM Programm ─┘     └── Steuerkennzeichen                        Formular-Nummer ───┘


REM %1 %2  FI3100 0300E       Monatsabschluß          FI4330,4321
REM » « == ASCII 0
