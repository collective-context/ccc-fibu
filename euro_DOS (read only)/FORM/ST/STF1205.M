&TITEL       = "ST  DATEI Word mit Briefanrede"

&FORMULAR    = 10
&FOOT_H      = 2

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110

&KOPF (0)
Anrede;Name1;Name2;Name3;Name4;Strasse;Plz_Ort
&END

&KOPF (1)
Kopf 1
----------------------------------------
&END

&RUMPF (0)
÷;÷;÷;÷;÷;÷;÷ - ÷ ~0.BRIEFANR_1 ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~0.STRASSE ~0.LAND ~0.PLZ_ORT
&END

&FUSS (0)
>>>>>>>>>  Seiten-Ende  (FUSS 0) <<<<<<<<<<<
&END



&REM
&REM ÷;÷;÷;÷ ÷;;÷;÷ - ÷ ~0.BRIEFANR_1 ~0.ANREDE ~0.NAME ~0.ZUSATZ ~0.POSITION_1 ~0.STRASSE ~0.LAND ~0.PLZ_ORT
&REM
