&TITEL       = "ST  Liste Word"

&FORMULAR    = 6600

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110
&DATEI (2) = MASK, VS1110

&KOPF (0)
Strasse;Plz_Ort;Name1;Name2;Name3;Name4;Name5;Telefon;Anrede;FaxName;Ort;Telefax
&END

&KOPF (1)
Kopf 1
----------------------------------------
&END

&RUMPF (0)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ┬á@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@ ~0.STRASSE ~0.PLZ_ORT ~0.NAME
&END


&RUMPF (1)
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", \
"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" \
~1._FAX_NAME ~1._FAX_NR ~0.BRANCHE
&END

&REM ~0.TELEFON ~0.BRIEFANR_1 ~1._FAX_NAME ~0.PLZ_ORT ~0.TELEFAX ~1._FAX_NR


&FUSS (0)
>>>>>>>>>  Seiten-Ende  (FUSS 0) <<<<<<<<<<<
&END



&TITEL       = "ST  Liste Word"

&FORMULAR    = 6600

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110
&DATEI (2) = MASK, VS1110

&KOPF (0)
Plz_Ort;Strasse;Name1;Name2;Name3;Name4;Name5;Telefon;Anrede;FaxName;Ort;Telefax;FaxNr
&END

&KOPF (1)
Kopf 1
----------------------------------------
&END

&RUMPF (0)
┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©┬á\
~1._ORT ~1._STRASSE_1 ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~1._NAME_5  \
~0.TELEFON ~0.BRIEFANR_1 ~0.PLZ_ORT ~0.TELEFAX ~1._FAX_NR ~1._FAX_NAME
&END

&REM ~0.TELEFON ~0.BRIEFANR_1 ~1._FAX_NAME ~0.PLZ_ORT ~0.TELEFAX ~1._FAX_NR

&REM ┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©┬á ~1._ORT ~1._STRASSE_1 ~1._NAME_1 ~1._NAME_2 \
&REM            ~1._NAME_3 ~1._NAME_4 ~1._NAME_5 ~0.TELEFON ~0.BRIEFANR_1 \
&REM            ~0.NAME ~0.PLZ_ORT ~0.TELEFAX

&FUSS (0)
>>>>>>>>>  Seiten-Ende  (FUSS 0) <<<<<<<<<<<
&END


&RUMPF (0)
┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©┬á ~0.STRASSE ~0.PLZ_ORT ~0.NAME ~1._NAME_1 \
           ~1._NAME_3 ~1._NAME_4 ~1._NAME_5 ~0.TELEFON ~0.BRIEFANR_1 \
           ~0.NAME ~0.PLZ_ORT ~0.TELEFAX
&END
