&TITEL       = "ST  Liste Telefonbuch"

&FORMULAR    = 1012
&LF_N_ZEILEN = 3
&FOOT_H      = 1

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110
&DATEI (2) = MASK, VS1110

&KOPF (0)
┬á
&END

&RUMPF (0)
!R!FONT2;SLPI5.5;EXIT;@@@@@@@@@@&a38C@@@@@@@@&a55C┬á┬©;┬á┬á┬©;┬á┬á(u@┬ám@)┬á┬á┬©;┬á┬á┬©;┬á┬á┬© \
~0.MATCH_CODE ~0.KUNDE ~0.NAME ~0.BRANCHE ~0.UMSATZ_GR ~0.MITARB_GR ~0.STRASSE  \
~0.PLZ_ORT ~0.TELEFON
&END

&FUSS (0)
┬á
&END



^ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \

┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬©;┬© ~0.LAND ~0.PLZ_ORT ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~1._NAME_5 ~0.TELEFON ~0.BRIEFANR_1
┬©;┬©;┬©;┬©;┬© - ┬© ~1._NAME_1 ~1._NAME_2 ~1._NAME_3 ~1._NAME_4 ~0.STRASSE ~0.LAND ~0.PLZ_ORT
&REM
&REM

&TITEL    = "ST  Datei f├╝r Word-Serienbrief"
&FORMULAR = 10

&DATEI (0) = MASK, ST1100
>>---

>>Anrede;Name;Zusatz;Strasse;Land;Plz_Ort;
>>@@@;@@@;@@@;@@@;@@@;@@@  ~0.ANREDE ~0.NAME ~0.ZUSATZ ~0.STRASSE ~0.LAND ~0.PLZ_ORT



&LIRAND = 0
SCHACHT = 1
---
FKO
VA1
END
---
FRU
VA2
END
---
Seitenfuss mit Zahlungskonditionen und 4 Zeilen Werbetext
FFU
END
---
