&TITEL     = "ST  DRUCK Telefonliste Lieferanten"

&FORMULAR    = 10
&FOOT_H      = 1

&DATEI (0) = MASK, ST1200
&DATEI (1) = MASK, ST4120

>>TELEFON-LISTE LIEFERANTEN
>>Anrede Name
>>Ansprechpartner: Anrede, Titel, Zuname
>>Straáe
>>Land - PLZ/Ort                 Telefon

&KOPF (0)
^ÿ                                                            ~SM
ÿ                                                 ÚÄÄÄ¿
ÿ                                                 ³   ³
ÿ                       ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   ÀÄÄÄÙ
ÿ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Adresse                            ³ Telefon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
&END

&RUMPF (0)
ÿÿÿÿ                               ³ÿÿÿ
ïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïï ³                          ~1._NAME_1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.ZUSATZ
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~1._NAME_2
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³ @@@@@@@@@@@@@@@@@@       ~1._ORT ~0.TELEFON
&END

&RUMPF (1)
ÿÿÿÿ                               ³ÿÿÿ
ïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïï ³                          ~1._NAME_1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~1._NAME_2
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³ @@@@@@@@@@@@@@@@@@       ~1._ORT ~0.TELEFON
&END

&RUMPF (2)
ÿÿÿÿ                               ³ÿÿÿ
ïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïï ³                          ~1._NAME_1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.ZUSATZ
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³ @@@@@@@@@@@@@@@@@@       ~1._ORT ~0.TELEFON
&END

&RUMPF (3)
ÿÿÿÿ                               ³ÿÿÿ
ïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïï ³                          ~1._NAME_1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³                          ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ³ @@@@@@@@@@@@@@@@@@       ~1._ORT ~0.TELEFON
&END

&RUMPF (4)
ÿÿÿÿ                               ³ÿÿÿ
&END

&RUMPF (5)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ^    ~NO
&END

&FUSS (0)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ^    ~NO
&END

