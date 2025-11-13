&TITEL       = "ST  DATEI Word mit LAND-PLZ/ORT, Telefon"

&FORMULAR    = 10
&FOOT_H      = 1

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110

&KOPF (0)
Plz_Ort;Strasse;Name1;Name2;Name3;Name4;Name5;Telefon;Briefanrede
&END

&KOPF (1)
Kopf 1
----------------------------------------
&END

&RUMPF (0)
¸;¸;¸;¸;¸;¸;¸;¸;¸; ~1._ORT ~1._STRASSE_1 ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~1._NAME_5 ~0.TELEFON ~0.BRIEFANR_1
&END

&FUSS (0)
>>>>>>>>>  Seiten-Ende (FUSS 0) <<<<<<<<<<<
&END


Land;Plz_Ort;Name1;Name2;Name3;Name4;Name5;Telefon;Briefanrede
¸;¸;¸;¸;¸;¸;¸;¸;¸ ~0.LAND ~0.PLZ_ORT ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~1._NAME_5 ~0.TELEFON ~0.BRIEFANR_1
