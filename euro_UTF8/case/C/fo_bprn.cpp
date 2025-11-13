// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Fo_Print()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Fo_Print()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD Fo_Print (apWkbMask[], pswMask, apstrRecord[], apstrMaskText[][25],
              apTextBox[][99], pswField, pstrOldString, pstrNewString,
              pswKeyCode, awBlocks[][2], awScroll[][4], awRecLen[]);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   apWkbMask[]        Zeiger auf Array für die Maskenattribute.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die
zentrale Schnittstelle zum Masken-Interpreter für den »Maskentyp 1«. Die
Funktion positioniert den Feldcursor an die angegebene Stelle innerhalb der
angegebenen Maske. Die Funktion kann erst aufgerufen werden, nachdem der
Interpreter mit der Funktion »M_Init()« initialisiert wurde.

Der Anwender kann dieses Feld editieren. Der Vorschlagswert ergibt sich aus
dem alten Feldinhalt, wie er im angegebenen Datensatz »apstrRecord[]« steht.

Verläßt der Anwender das Feld durch ein Sonderfunktions-Taste, die für den
Feld-Editor keine Bedeutung hat, so erfolgt nach einer erfolgreichen Syntax
prüfung und Format-Aufbereitung der Rücksprung in den aufrufenden Programm-
teil. Zuvor wird der vom Anwender eingegebene Wert im Datensatz an der dem
Feld zugewiesenen Stelle (siehe Offset-Anweisung im Masken-Compiler) abge-
stellt (s.u.).

Die Syntaxprüfung des Interpreters ergibt sich aus der Maskendefinition
(siehe u.a. Range-, Choise- und Datum-Anweisung des Maskencompilers). Der
Anwender kann das Feld erst dann verlasasen, wenn er einen syntaktisch
richtigen Wert eingegeben hat. Über die semantische Gültigkeit des Wertes
muß der Applikationsprogrammierer entscheiden.

Nach dem Rücksprung kann der Applikationsprogrammierer nun für dieses Feld
individuelle Apprüfungen vornehmen. Anhand des übergebenen Key-Codes kann
er feststellen mit welcher Sonderfunktionstaste der Anwender das Feld ver-
lassen hat.

Bei einem erneuten Aufruf von »M_Input()« mit einem unveränderten
Key-Code (aus dem letzten Aufruf), führt der Interpreter zuerst die Aktion
durch, die mit dem Key-Code verbunden ist (sofern der Key-Code für den
Interpreter eine Bedeutung hat), bevor er das aus dieser Interpretation
resultierende Feld betritt.

Hat zum Beispiel der Anwender das Feld mit der Enter-Taste verlassen und der
Applikationsprogrammierer die für ihn erforderlichen Abprüfungen durchgeführt
und ruft er »M_Input()« erneut auf, so interpretiert der Maskeninterpreter
zuerst den Key-Code (in diesem Fall den entsprechenden Wert für die Enter-
Taste).

Die Enter-Taste hat für den Interpreter die Bedeutung, den Feldcursor auf
das nächste Feld laut Sequenz-Angabe vorzurücken. Ausgangspunkt ist dabei,
das dem Interpreter angegebene Feld. Hat der Applikationsprogrammierer die
Feldnummer seit dem letzten Aufruf nicht verändert, so ist sie identisch mit
der Feldnummer des letzten Feldes, welches der Anwender gerade verlassen hat.

Nach Interpretation des Key-Codes befindet sich der Feld-Editor im resul-
tierenden Feld (im Beispiel das nächste Feld lt. Sequenz-Angabe).

Der Applikationsprogrammierer kann auf diese Weise sowohl die Ausgangs-Feld-
nummer, als auch den Key-Code zur Steuerung des Interpreters, manipulieren
und somit den Feldcursor auf eine individuelle Weise steuern.

Wurde seit dem letzten Aufruf von »M_Input()« die Maskennummer verändert,
so blättert der Interpreter zuerst auf die angegebene Maske. Die angegebene
Feldnummer bezieht sich immer auf die gewählte Maske. Ist die angegebene
Feldnummer nicht auf der angegeben Maske, so wird ein Fehler gemeldet.

Hat der Anwender das Feld mit der Ctrl-PgUp oder Ctrl-PgDn Taste verlassen,
so erfolgt der Rücksprung aus »M_Input()« mit dem entsprechenden Key-Code.

Wird »M_Input()« nun erneut mit unverändertem Key-Code aufgerufen, so
interpretiert der Masken-Interpreter zuerst die Ctrl-PgUp oder Ctrl-PgDn
Funktion, was zur Folge hat, daß entweder eine Maske vor oder zurückge-
blättert wird.

Beim Blättern wird der Feldcursor an die Stelle gesetzt, aus der die Maske
zuvor verlassen wurde. Wurde die Maske noch nit betreten, wird der Feldcursor
an die erste Stelle laut Sequenz-Angabe gesetzt. Dies unabhängig von der
Feldnummer, die der Applikationsprogrammierer angegeben hat.

Will der Applikationsprogrammierer, daß beim Betätigen von Ctrl-PgUp oder
Ctrl-PgDn auf ein bestimmtes Feld der Maske gesprungen wird, so muß er den
Ctrl-PgUp/PgDn-Key-Code abfangen, die Maskennummer selbstständig verändern,
die gewünschte Feldnummer setzen und anschließend »M_Input()« mit einem
Key-Code aufrufen, der für den Interpreter keine Bedeutung hat (z.B. 0).

»M_Input()« darf nur aufgerufen werden, wenn die angegebene Maske vom
»Maskentyp 1« ist.

Versorgung:
   Eingang:
   char *equal_part[];
      Ist ein String-Feld, welches die Equivalenz_Angaben zu eventuell vor-
      handenen Choise-Elementen beinhaltet. Sind laut Maskendefinition
      Choise-Elemente vorhanden so werden die Equivalenz-Angaben zusätzlich
      zu den einzelen Choise-Elementen im Auswahl-Fenster angezeigt. Damit
      ist es zum Beispiel möglich die Equivalenz-Werte einer Codierung aus
      einer Parameter-Datei zu lesen.
      Für den Fall, daß ein Feld mit der Dychoise-Anweisung belegt wurde,
      stehen in equal_part[] die eigentlichen Auswahlmöglichkeiten aufge-
      zählt. In diesem Fall gibt es also keine Equivalenz-Angaben.
      Achtung: Ist equal_part[] kleiner dimensioniert als die anzahl der
               Auswahlmöglichkeiten laut Maskendefinition, so erscheinen im
               Auswahl-fenster nur so viele Elemente, wie equal_part[]
               dimensioniert ist.
   Ein-/Ausgang:
   int mask_number;
      Beinhaltet die Nummer der Maske, die aktuell angewählt werden soll.
      Ist die Nummer nicht identisch mit der Nummer der aktuell ange-
      zeigten Maske, so wird auf die angegebene Maske vor oder zurück-
      geblättert. Vor dem ersten Aufruf von »M_Input()« ist die aktuelle
      Maske die erste in »awMaskSequenz_m« angegebene Maske.
      Beim Rücksprung in den aufrufenden Programmteil, beinhaltet
      mask_number die aktuell eingestellte Maske. Dieser Wert unterscheidet
      sich insbesondere dann vom Eingangswert, wenn ein PgUp- oder PgDn-
      Funktionscode interpretiert wurde.
   int application_field_number;
      Beinhaltet die Nummer des Feldes (relativ zur angegebenen Maske), das
      aktuell angewählt werden soll. Die Feldnummer bezeichnet dabei die
      Applikations-Feldnummer der jeweiligen Maske. Die Applikations-
      Feldnummer ist identisch mit der Feldnummer des Masken-Compilers,
      solange keine Nummer-Anweisung gegeben wurde. Wurde einem Feld über
      eine Nummer-Anweisung eine andere , applikationsabhängige, Feld-
      nummer zugewiesen, so ist das gewünschte Feld durch diese Appli-
      kations-Nummer anzusprechen.
      Beim Rücksprung in den aufrufenden Programmteil beinhaltet
      application_field_number die Nummer des Feldes, daß der Anwender
      gerade verlassen hat. Der Rückgabewert unterscheidet sich meist vom
      Eingangswert, außer wenn beim Aufruf der Funktion ein Key-Code ange-
      geben wurde, der für den Interpreter keine Bedeutung hat. (z.B. wenn
      eine individuelle Abprüfung negativ ausfiel und der Anwender einen
      neuen Wert für das gleiche Feld angeben muß).
   char *record[];
      Ist das Feld der Datensätze, die durch das Maskentool manipuliert
      werden sollen. Der Offset des Feldes innerhalb des Records ist in der
      Maskendefinition hinterlegt (siehe Offset-Anweisung im Masken-
      Compiler).
      Beim Rücksprung in den aufrufenden Programmteil enthält record[] immer
      den vom Anwender eingegebenen Wert, auch dann, wenn er u.U. falsch ist.
      D.h., daß der Applikationsprogrammierer den alten Feldwert selbst
      wieder eintragen muß, sobald sich eine inidividuelle Abprüfung als
      negativ herausgestellt hat (siehe old_string).
      In record[] muß für jeden Datensatz ein ausreichend langer String ange-
      geben werden, sodaß das Feld mit der größten Offset-Nummer noch Platz
      hat.
   int key_code;
      Beinhaltet die Funktionscodes für den Masken-Interpreter. Bei Aufruf
      von M_Input () wird der Funktions-Code interpretiert und die der
      Bedeutung entsprechende Funktion ausgeführt, ehe der Feld-Editor
      aufgerufen wird.
      Beim Rücksprung aus M_Input () gibt der Funktions-Code die
      Sonderfunktions-Taste an, mit dem der Anwender den Feldeditor ver-
      lassen hat.
   Ausgang:
   int record_number;
      Gibt den Index für den Parameter record[] an, der für das gerade
      verlassene Feld Gültigkeit hat. Diese Angabe kommt aus der Masken-
      definiton (Offset-Anweisung im Masken-Compiler).
   int record_offset;
      Gibt den Offset des gerade verlassenen Feldes innerhalb von
      record[record_number] an. Diese Angabe kommt aus der Maskendefinition
      (Offset-Anweisung im Masken-Compiler).
   char *old_string;
      Gibt den alten Feldinhalt aus record[record_number] an, der dort vor
      verlassen des Feldes durch den Anwender gestanden hat. Diesen Wert
      muß der Applikationsprogrammierer wieder in den Datensatz einbauen,
      falls eine individuelle Abprüfung negativ ausfällt, da der Inter-
      preter auf jeden Fall den vom Anwender eingegebenen Wert im Daten-
      satz ablegt.
   char *new_string;
      Gibt den neuen Feldinhalt aus record[record_number] an, der nach
      verlassen des Feldes durch den Anwender dort eingesetzt wurde. Hat
      der Anwender den Feldinhalt nicht verändert, so ist new_string gleich
      old_string.
      Sowohl new_string als auch old_string geben nicht unbedingt den vom
      Benutzer eingegebenen Wert wieder, sondern den Feldinhalt aus
      record[record_number] der je nach maskentyp konvertiert worden sein
      kann (siehe Packed-, Datum- und Choise-Anweisung im Masken-Compiler).


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#define NDEBUG 1            /* Mit Check kompiliern == Zeile unter REMARKS  */

#define fPutC(cWert,  pfDevice)   if(pFkb->wVonSeite<=pFkb->wAktSeite && pFkb->wBisSeite>=pFkb->wAktSeite) fputc(cWert,  pfDevice)
#define fPutS(acWert, pfDevice)   if(pFkb->wVonSeite<=pFkb->wAktSeite && pFkb->wBisSeite>=pFkb->wAktSeite) fputs(acWert, pfDevice)

#define T_put(STR_OUT) if(boTestModus_g||boMonitor_g==2) \
  fprintf(pfDevice, STR_OUT, pFkb->wAktZeile)
#define T_cat(STR_OUT) if(boTestModus_g||boMonitor_g==2) \
  strcat(strHide_m, chi(pFkb->wAktZeile+wAktZeile_m, STR_OUT))
#define IF(pstrWo, pstrWas) if(!strcmp(pstrWo, pstrWas))

#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <eur_tool.h>

STATIC VOID iSteuerKz(PFKB, PPKB, FILE *, SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];		     /* Farbpalette und Index auf    */
IMPORT SWORD wCSInd_g;
IMPORT MENUSTATUS MS_g;
IMPORT BOOL boTestModus_g;
IMPORT SLONG lPrintModus_g;	   /* z.B.: Probedruck	   */
IMPORT BOOL boMonitor_g;
IMPORT SWORD wMausda_g;
IMPORT BOOL boBeepen_g;

IMPORT CHAR	strExt_g[4];	      // Extention für Landescode
IMPORT CHAR	strMan_g[3];	      // Mandaten-Nummer
IMPORT CHAR	strDat_g[81];	      // Daten-Verzeichnis

IMPORT PSSTR pstrDataBuffer_g;
IMPORT PWKB pWkbInfo_g;

// IMPORT PTEXTBOX apTextBox_g[UPB_MASK][99];  //Eingabe-Felder wegen TBpt()

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD cdecl SetData   (PSSTR, PSWORD, PSWORD, PSWORD, PSBYTE, PSBYTE, PSBYTE, PSSTR);
STATIC	SWORD Read_Print_Info (PSSTR, PFKB, PPPKB);
STATIC	SWORD Print_Text      (PSSTR[], PFKB, PPKB, PPKB[], PSWORD, FILE *, SWORD);
STATIC	SWORD i_ChangexFF     (PSSTR, PSSTR[], PFKB, PPKB[], FILE *);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE DATEN                               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL BOOL boInhalt_m=NO;
MGLOBAL BOOL boHide_m=AUS;
IMPORT	BOOL boRekursiv_g;
MGLOBAL BOOL boNoLineFeed_m=NO;
MGLOBAL SWORD wRand_m;
MGLOBAL CHAR strHide_m[10000];
MGLOBAL SWORD wAktZeile_m;
MGLOBAL SWORD wZeilenVorLF_m;
MGLOBAL BOOL boHeadFoot_m;
GLOBAL	BOOL boFF_g=NEIN;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_Print(PSSTR apstrRecord[], FILE *pfDevice, SWORD wBereich,
	     PFKB pFkb, PPKB apPkb[], PSSTR pF, SWORD wL)
{
SWORD wEnd=0;
/* CHAR strFile[TB_MAX];
sprintf(strFile,"%s\\%s.F00", getenv("USER"), getenv("APP"));
Ut_PrintFile(strFile, "a+", "\n...Bereich(%3d), Zeile(%4d), Seite(%3d), "
	   "File(%10s), Line(%5d).",
	   wBereich, pFkb->wAktZeile, pFkb->wAktSeite, pF, wL); */

if(boMonitor_g==1) pFkb->wLiRand=0;
else if(boMonitor_g==2) pFkb->wLiRand=1;

/* while(wEnd < 1) */
  {
  SREGISTER i, j, k;				      /* Zählervariable       */
  SWORD wSpalte=0, wZ=0;
  BOOL boRumpf=NEIN;

  for(i=0; apPkb[i] &&
    apPkb[i]->wBereich < wBereich; i++) 	     /* Aufsatz für Kopf-,   */
    ;

  if(boHide_m==EIN) wZ++;
  while(pFkb->wAktZeile+wZ+
    pFkb->wNextFootHeight <= pFkb->wFormularHoehe &&
    wBereich == pFkb->wNextFoot)
    {
    /*printf("\npFkb->wNextFootHeight(%d<%d)",pFkb->wAktZeile+wAktZeile_m+
    pFkb->wNextFootHeight, pFkb->wFormularHoehe);*/

    /*if (isatty((SWORD)pfDevice))		      /* Ausgabe == Drucker   */
    /*	 fPutC('\x0D', pfDevice);*/

    boHeadFoot_m=(wBereich==pFkb->wNextHeader||
      wBereich==pFkb->wNextFoot);

    if(boHide_m==AUS || boHeadFoot_m)
      {fPutC('\x0A', pfDevice);
      pFkb->wAktZeile++; T_put("%2d.»");}
    else
      {cat(strHide_m, '\x0A');
      wAktZeile_m++; wZ++; T_cat("°°.>");}

    wSpalte=0;
    wRand_m=0;

    }


  for(; apPkb[i]&&apPkb[i]->wBereich==wBereich;i++)					 /* Kopf-, Rumpf- oder	 */
    {						     /* Fußbereich suchen u. */
    if(pFkb->wAktZeile == 32000 &&		     /* drucken bis das Be-  */
      pFkb->wNextHeader )			     /* reichsende erreicht  */
      {Fo_Set_Page(apstrRecord, pfDevice,
	pFkb, apPkb, pF, wL);				   /* Kopfzeile drucken */
      wSpalte=0; wRand_m=0;}

    if(pFkb->wAktZeile+pFkb->wNextFootHeight+
      apPkb[i]->wDruckLF > pFkb->wFormularHoehe &&
      wBereich != pFkb->wNextFoot && pFkb->wNextFoot)
      {
      Fo_Print(apstrRecord, pfDevice,		       /* Fußzeile drucken   */
	pFkb->wNextFoot, pFkb, apPkb, pF, wL);
      Fo_Set_Page(apstrRecord, pfDevice,
	pFkb, apPkb, pF, wL);			     /* Kopfzeile drucken  */
      }

    /*if(boHide_m==AUS || boHeadFoot_m)
      if(boNoLineFeed_m==YES)
	if(apPkb[i]->wDruckLF>0)
	   {apPkb[i]->wDruckLF-=1;
	   boNoLineFeed_m=NO;}*/

    for(j=0; j<apPkb[i]->wDruckLF; j++)
      {
      /*if(isatty((SWORD)pfDevice))		     /* Ausgabe == Drucker   */
      /*    fPutC('\x0D', pfDevice); */
      boHeadFoot_m=(wBereich==pFkb->wNextHeader||
	wBereich==pFkb->wNextFoot);

      if(boNoLineFeed_m==YES)
	boNoLineFeed_m=NO;
      else if(boHide_m==AUS || boHeadFoot_m)
	{fPutC('\x0A', pfDevice);
	pFkb->wAktZeile++; T_put("%2d:»");}
      else
	{cat(strHide_m, '\x0A');
	wAktZeile_m++; T_cat("¨¨:>");}

      if(pFkb->wAktZeile >= pFkb->wFormularHoehe)
	pFkb->wAktZeile=0;

      /* fprintf(pfDevice, "»%d/%d«", pFkb->wAktZeile,
	pFkb->wFormularHoehe); */

      wSpalte=0;
      wRand_m=0;
      }

    if(!wRand_m)
      {
      wRand_m=1;
      boHeadFoot_m=(wBereich==pFkb->wNextHeader||
	wBereich==pFkb->wNextFoot);
      Fo_Print_SteuerKz(GLpt("SCHR_NORM"), pfDevice);

      for(k=0; k<pFkb->wLiRand; k++)                 /* Bei Zeilenbeginn     */
	if(boHide_m==AUS || boHeadFoot_m)
	  {fPutC(' ', pfDevice);}		       /* linken Rand drucken  */
	else
	  cat(strHide_m, ' ');

      switch(pFkb->wStandardFont)
        {
	case 1: Fo_Print_SteuerKz(GLpt("SCHR_NORM"), pfDevice); break;
	case 2: Fo_Print_SteuerKz(GLpt("SCHR_SCHÖ"), pfDevice); break;
	case 3: Fo_Print_SteuerKz(GLpt("SCHR_SCHM"), pfDevice); break;
	case 4: Fo_Print_SteuerKz(GLpt("SCHR_FREI"), pfDevice); break;
        }
      }

    if(apPkb[i]->bDruckTyp != 20)                    /* Mit Blanks füllen    */
      for( ; wSpalte<apPkb[i]->wDruckS; wSpalte++)   /* wenn nicht ¸ Dateityp*/
	if(boHide_m==AUS || wBereich==pFkb->wNextHeader
	  || wBereich==pFkb->wNextFoot)
	  {fPutC(' ', pfDevice);}
	else
	  cat(strHide_m, ' ');

    Print_Text(apstrRecord, pFkb, apPkb[i], apPkb,
      &wSpalte, pfDevice, wBereich);

    boRumpf=JA;
    } /* end apPkb[i]->wBereich && apPkb[i] */

  /*if(wBereich==pFkb->wNextFoot)*/
  /*  { */
  /*  if(isatty((SWORD)pfDevice))		    /* Ausgabe == Drucker   */
  /*    fPutC('\x0D', pfDevice); */
  /*  fPutC('\x0A', pfDevice);   */
  /*  } */

  if(pFkb->cLFnZeilen && boRumpf)
    if((SWORD)pFkb->cLFnZeilen < ++wZeilenVorLF_m)
      {fPutC('\x0A', pfDevice);
      pFkb->wAktZeile++; wZeilenVorLF_m=1;}

  wEnd++;
  } /* while(wEnd < 1) == Ende der Kopien */

fflush(pfDevice);
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Data()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║  Rückgabewert:                                                          ║
  ║   SWORD wRecord	    Dateinummer in Applikation			     ║
  ║      wOffset           DatenOffset für Applikation                      ║
  ║      wMaxL             Max. Länge des Teilstrings in Datei              ║
  ║                                                                         ║
  ║   BYTE bTyp                  Typ laut Data-Dictionary                   ║
  ║      bVKS                    Anzahl der Vorkommastellen                 ║
  ║	 bNKS			 Anzahl der Nachkommastellen		    ║
  ║                                                                         ║
  ║   PSSTR pstrDatenfeld	  Event. Name (bei Choice/Select)	     ║
  ║                                                                         ║
  ║   Benutzte globale                                                      ║
  ║   Variablen (R/W):                                                      ║
  ║                                                                         ║
  ║ Diese Daten werden aus der Formularbibliothek für die Variable geholt   ║
  ║ und stehen in der Form:                                                 ║
  ║	       »wRecord.wOffset.wMaxL.bTyp.bVKS.bNKS[.pstrDatenfeld]«	    ║
  ║ zur Verfügung.							    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetData (PSSTR pstrText, PSWORD pwRecord, PSWORD pwOffset, PSWORD pwMaxL,
	      PSBYTE pbTyp,  PSBYTE pbVKS, PSBYTE pbNKS, PSSTR pstrDatenfeld)
{
SREGISTER i;
PSSTR  pstrTemp, apstrEintrag[10];		       /* Beginn Record-Info  */

apstrEintrag[0]=NULL;
Ut_Calloc(apstrEintrag[0], strlen(pstrText)+10, CHAR);
strcpy(apstrEintrag[0], pstrText);

i=0;
pstrTemp=apstrEintrag[0];			      /* Adresse für Rec-Info*/
while(*pstrTemp && i<6) 			      /* Bleib bis Stringende*/
  {
  if(*pstrTemp=='.')				      /* Wenn Trennpunkt das */
    {						      /* Ende von Record-    */
    *pstrTemp = '\0';				      /* info kennzeichnen.  */
    apstrEintrag[++i] = pstrTemp+1;		      /* Denn Beg. von Field */
    }						      /* merken, nächst. Zei-*/
  else if(*pstrTemp==' ')			      /* chen. Bei Leerzeich.*/
    *pstrTemp = '\0';				      /* das Ende vom Feld-  */
						      /* namen kennzeichen.  */
  pstrTemp++;					      /* Auf nächsten Buch-  */
  }						      /* staben zeigen.      */

apstrEintrag[6]=pstrTemp;

*pwRecord = atoi(apstrEintrag[0]);
*pwOffset = atoi(apstrEintrag[1]);
*pwMaxL   = atoi(apstrEintrag[2]);
*pbTyp = (SBYTE) atoi(apstrEintrag[3]);
*pbVKS = (SBYTE) atoi(apstrEintrag[4]);
*pbNKS = (SBYTE) atoi(apstrEintrag[5]);

strcpy(pstrDatenfeld, apstrEintrag[6]);

if (*pbTyp == 0)
  *pbNKS = 0;

Ut_Free(apstrEintrag[0]);
return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_Open ()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Öffnen der Formular-Bibliotheks-Datei.                                  ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_Open (PSSTR pstrApplikation, SWORD wFormular, PFKB pFkb, PPPKB ppPkb)
{
SREGISTER i;
SWORD wBufferLength, wStatus;
CHAR strFormularDatei[TB_MAX], strError[TB_MAX], strFileName[TB_MAX];
CHAR strTemp[TB_MAX], strFileBlock[128], strFormular[20];
D_DATE *pD=NULL;

pD=D_get_date(pD);
sprintf(strFormularDatei, "%sD%s\\%#04d\\%sFORM.%s", strDat_g, strMan_g,
       pD->wJahr, pstrApplikation, strExt_g, _N);
Ut_Free(pD);

sprintf(strFormular, "%#04d", wFormular);

strcpy(strTemp, "(c)Mayer");
wBufferLength=strlen(strTemp)+1;

strcpy(strFileName, strFormularDatei);
for(i=0,wStatus=12; (wStatus==11 || wStatus==12) &&
  i<3 && strcmp(strDat_g, strFileName); i++)
  {
  SWORD wHandle=0;
  PSSTR pstr, pstrName=strrchr(strFileName, '\\');

  wStatus=BTRV(B_OPEN, strFileBlock, strTemp,	     /* Datei öffnen	     */
    &wBufferLength, strFileName, O_NORMAL);	     /* 		     */

  sprintf(strError, "B_OPEN(FORM), - Datei: %s",
    strFileName);

  if(wStatus==11 || wStatus==12)
    {wHandle=12; *pstrName='\0';
    pstr=strrchr(strFileName, '\\'); *pstrName='\\';
    if(pstr) memmove(pstr, pstrName, strlen(pstrName)+1);}

  Dl_ErrorHandler (wStatus, strError,
    __FILE__, __LINE__, wHandle);
  }

if(wStatus!=11 && wStatus!=12)
  strcpy(strFormularDatei, strFileName);

wBufferLength = MAX_RECORD_SIZE;
wStatus=BTRV(B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                 &wBufferLength, strFormular, 0);

sprintf(strError, "B_GET_EQ(FORM), - Datei: %d _ Schlüssel %s, Index: %d", 0, strFormular, 0);
Dl_ErrorHandler(wStatus, strError, __FILE__, __LINE__, 0);


wBufferLength = 0;
wStatus=BTRV(B_CLOSE, strFileBlock, strTemp, &wBufferLength, strFormularDatei, O_NORMAL);  /* -(i03)- */
   sprintf(strError, "B_Close, - Datei: %s", strFormularDatei);
   Dl_ErrorHandler (wStatus, strError, __FILE__, __LINE__, 0);

Read_Print_Info(pstrDataBuffer_g, pFkb, ppPkb);
/* ppPkb[0]->wDruckLF-=1; */

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Print_Info()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Kopiern des Recordinhalts in die PrintInfo-Stuktur und FormularInfo-    ║
  ║ struktur.                                                               ║
  ║                                                                         ║
  ║ pFkb = Pointer auf Formular-Kontroll-Block.                             ║
  ║ pPkb = Pointer auf Print-Kontroll-Block                                 ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_Print_Info(PSSTR pstrBuffer, PFKB pFkb, PPPKB ppPkb)
{
SREGISTER i;
PPKB pPkb;
FELDINFO *ptempinfo;
SWORD wTmp;

i=0;

pstrBuffer += 17;                            /* Auf Kopfinformation setzen */
pstrBuffer += strlen(pstrBuffer) + 1;
sscanf(pstrBuffer, "%d", &pFkb->wFormular);	 pstrBuffer+=5;
sscanf(pstrBuffer, "%d", &pFkb->wLiRand);	 pstrBuffer+=3;
sscanf(pstrBuffer, "%d", &pFkb->wFormularHoehe); pstrBuffer+=3;
sscanf(pstrBuffer, "%d", &pFkb->wObRand);	 pstrBuffer+=3;
sscanf(pstrBuffer, "%d", &pFkb->wUnRand);	 pstrBuffer+=3;
sscanf(pstrBuffer, "%d", &wTmp);		 pstrBuffer+=3;
pFkb->cLFnZeilen=(SBYTE)wTmp;
sscanf(pstrBuffer, "%d", &pFkb->wNextFootHeight);
pFkb->wNextFootHeight++;

pstrBuffer+=strlen(pstrBuffer) + 1;             /* auf Vairablen Teil setzen */

while(*pstrBuffer)				    /* Bis NULL-P. kopieren  */
  {
  Ut_Calloc(*ppPkb, 1, PKB);
  pPkb=*ppPkb;

  pPkb->wBereich  = atoi (pstrBuffer);
  pPkb->bDruckTyp = (SBYTE) atoi (pstrBuffer+=4);
  pPkb->wDruckLF  = atoi (pstrBuffer+=3);
  pPkb->wDruckS   = atoi (pstrBuffer+=3);
  pPkb->wDruckB   = atoi (pstrBuffer+=4);

  strncpy(pPkb->acDruckCode, pstrBuffer+=4, 3);
  pPkb->bKomma = (SBYTE) atoi(pstrBuffer+=8);

  Ut_Calloc(pPkb->acText, strlen(pstrBuffer+=5)+1, CHAR);
  strcpy(pPkb->acText, pstrBuffer);
  pstrBuffer+=strlen(pstrBuffer)+1;

  Ut_Calloc(pPkb->pFeld, 1, FELDINFO);
  ptempinfo=pPkb->pFeld;

  while (*pstrBuffer)
    {
    SREGISTER k;
    PSSTR pstrTmp=pstrBuffer;
    pstrBuffer=strchr(pstrBuffer, '\0')+1;

    sscanf(pstrTmp, "%3d", &ptempinfo->bArt);

    pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &ptempinfo->bLaenge);
    pstrTmp+= 4;

    for(k=0; k < 20; k++)
      {
      if(*pstrTmp != '■')
	{
	sscanf (pstrTmp, "%3d", &ptempinfo->abFormat[k]);
	pstrTmp += 4;
	}
      else
	ptempinfo->abFormat[k] = 0;
      }

    if(*pstrBuffer)
      {Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
      ptempinfo = ptempinfo->pNext;}
    }

  pstrBuffer++;
  ppPkb++;
  }

*ppPkb=NULL;
return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Text()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ pFkb = Pointer auf Formular-Kontroll-Block.                             ║
  ║ pPkb = Pointer auf Print-Kontroll-Block                                 ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Print_Text(PSSTR apstrRecord[], PFKB pFkb, PPKB pPkb, PPKB apPkb[],
		PSWORD pwSpalte, FILE *pfDevice, SWORD wBereich)
{
CHAR strText[500];				  /* reichsende verweilen */
TEXTBOX TB;
PTEXTBOX pTB=&TB;

iSteuerKz(pFkb, pPkb, pfDevice, wBereich);

switch(pPkb->bDruckTyp)
  {
  CHAR strDatenfeld[20];
  SWORD wRec, wOff, wMaxL;
  SBYTE bTyp, bVKS, bNKS;

  case 1:                                            /* Hintergrund-Text     */
      i_ChangexFF(pPkb->acText, apstrRecord, pFkb, apPkb, pfDevice);
      if(boHide_m==AUS || wBereich==pFkb->wNextHeader
	|| wBereich==pFkb->wNextFoot)
	{fPutS(pPkb->acText, pfDevice);/*T_put("%2d~");*/}
      else
	{strcat(strHide_m, pPkb->acText);/*T_cat("  ^");*/}

      *pwSpalte+=strlen(pPkb->acText);
      break;

   case 2:                                           /* Offset-Informationen */
   case 20:
      SetData(pPkb->acText, &wRec, &wOff,
	   &wMaxL, &bTyp, &bVKS, &bNKS, strDatenfeld);

      /* if(*strDatenfeld=='\0') */
	{pTB->wRecord=wRec;	     pTB->wOffset	 =wOff;
	pTB->wMaxL   =wMaxL;	     pTB->bTyp		 =bTyp;
	pTB->bVKS    =bVKS;	     pTB->bNKS		 =bNKS;
	pTB->bKomma =pPkb->bKomma; pTB->wFensterB      =pPkb->wDruckB;
	pTB->pFeld   =pPkb->pFeld;   pTB->acText[0]	 ='\0';
	pTB->pChoice =NULL;	     pTB->pstrChoiceTitel=NULL;
	pTB->pSelect =NULL;	     pTB->strDatenfeld[0]='\0';}
      /* else
	{PTEXTBOX pT=TBpt(strDatenfeld, 99, 99);
	Dl_KopierBox(pTB, pT);} */

      /* if(*strDatenfeld!='\0')
	{boTestModus_g=JA;
	Wi_TestPrintf(pWkbInfo_g, "\nX:%s=%s/%s.", pTB->strDatenfeld,
	  pTB->pstrChoiceTitel, pTB->acText);
	boTestModus_g=NEIN;} */

      M_RecordToBox(pTB, apstrRecord);
      /*if(bTyp==0 && !pTB->pChoice && !pTB->pSelect) pTB->acText[wMaxL]='\0';*/
      strcpy(strText, pTB->acText);

      if(lPrintModus_g&1)			    /* Probedruck	    */
	{memset(strText, '\0', 500);
	memset(strText, '.', 499);}
      else if(lPrintModus_g&2)			    /* Probedruck Etiketten */
	strcpy(strText, str("Zeile: »%d«",pFkb->wAktZeile));

      if(bTyp>=1 && bTyp<=5)
	{if(boHide_m==EIN && atoi(strText) )
	  boInhalt_m=YES;}
      else if(boHide_m==EIN && strlen(strText))
	boInhalt_m=YES;

      if(boInhalt_m==YES && strlen(strText)>(SIZE_T)pPkb->wDruckB)
	{
	PSSTR pstr;
	while(strlen(strText)>(SIZE_T)pPkb->wDruckB)
	  {
	  CHAR c=strText[pPkb->wDruckB-1];
		  strText[pPkb->wDruckB-1]='\0';
	  pstr=strrchr(strText, ',');
	  if(!pstr) pstr=strrchr(strText, ' ');
		 strText[pPkb->wDruckB-1]=c;
	  if(pstr)
	    {if(*pstr==' ') *pstr='\0';
	    else *(++pstr)='\0';

	    strcat(strHide_m, strText);
	    cat(strHide_m, '\x0A');		     /* LF		     */
	    wAktZeile_m++; T_cat("■■->");
	    *pwSpalte=0;

	    while(*pwSpalte < pPkb->wDruckS+pFkb->wLiRand)  /*		       */
	      {/*printf("\nSpalte(%d)<DruckS(%d)+Rand(%d)[%d]",
	      *pwSpalte, pPkb->wDruckS, pFkb->wLiRand,
	      pPkb->wDruckS+pFkb->wLiRand);*/
	      cat(strHide_m, ' '); (*pwSpalte)++;}     /* linker Rand	       */

	    strcpy(strText, ++pstr);}
	  else
	    strText[pPkb->wDruckB]='\0';	     /* rechts begrenzen     */

	  } /* end while */
	} /* end Hide-Feld == Select */

      if(pPkb->bDruckTyp != 20) 		     /* Ausser bei ¸ Typ     */
	strText[pPkb->wDruckB]='\0';		     /* rechts begrenzen     */

      i_ChangexFF(strText, apstrRecord, pFkb, apPkb, pfDevice);
      if(boHide_m==AUS || wBereich==pFkb->wNextHeader
	|| wBereich==pFkb->wNextFoot)
	{fPutS(strText, pfDevice);}
      else
	{strcat(strHide_m, strText);}

      *pwSpalte+=strlen(strText);
      break;

  } /* end switch(pPkb->bDruckTyp) */


if(!strcmp(pPkb->acDruckCode,"HA ") ||
  !strcmp(pPkb->acDruckCode,"HL ") ||
  !strcmp(pPkb->acDruckCode,"HD "))
  {
  boHide_m=AUS;
  if(boInhalt_m==NO &&
    (!strcmp(pPkb->acDruckCode,"HA ") ||
    !strcmp(pPkb->acDruckCode,"HD ")) )
    boNoLineFeed_m=YES;
  else if(boInhalt_m)
    {
    if(pFkb->wAktZeile+wAktZeile_m+pFkb->wNextFootHeight >=
      pFkb->wFormularHoehe && pFkb->wNextFoot)
      {/* printf("\x07"); /* ~ */
      boNoLineFeed_m=YES;
      Fo_Print(apstrRecord, pfDevice,		       /* Fußzeile drucken   */
	pFkb->wNextFoot, pFkb, apPkb, _F, _L);
      Fo_Set_Page(apstrRecord, pfDevice,
	pFkb, apPkb, _F, _L);}			       /* Kopfzeile drucken  */

    fPutS(strHide_m, pfDevice);
    pFkb->wAktZeile+=wAktZeile_m;
    }

  boHide_m=AUS;
  if(!strcmp(pPkb->acDruckCode,"HD "))
    boHide_m=EIN;

  *strHide_m='\0';
  wAktZeile_m=0;
  boInhalt_m=NO;
  }

return(OK);
} /* end Print_Text */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iSteuerKz() 							    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iSteuerKz(PFKB pFkb, PPKB pPkb, FILE *pfDevice, SWORD wBereich)
{

if(pFkb->wVonSeite<=pFkb->wAktSeite && pFkb->wBisSeite>=pFkb->wAktSeite)
  {
  CHAR strSteuer[TB_MAX];
  PSSTR pstr=pPkb->acDruckCode;
  *strSteuer='\0';

       IF(pstr,"NO ") {strcpy(strSteuer,"SCHR_NORM"); pFkb->wStandardFont=1;}
  else IF(pstr,"SM ") {strcpy(strSteuer,"SCHR_SCHM"); pFkb->wStandardFont=3;}
  else IF(pstr,"LQ ") {strcpy(strSteuer,"SCHR_SCHÖ"); pFkb->wStandardFont=2;}
  else IF(pstr,"OP ") {strcpy(strSteuer,"SCHR_FREI"); pFkb->wStandardFont=4;}
  else IF(pstr,"HE ") boHide_m=EIN;
  else IF(pstr,"UE ") strcpy(strSteuer, "UNTER_EIN");
  else IF(pstr,"UA ") strcpy(strSteuer, "UNTER_AUS");
  else IF(pstr,"FE ") strcpy(strSteuer, "FETT_EIN");
  else IF(pstr,"FA ") strcpy(strSteuer, "FETT_AUS");
  else IF(pstr,"BE ") strcpy(strSteuer, "BREIT_EIN");
  else IF(pstr,"BA ") strcpy(strSteuer, "BREIT_AUS");
  else IF(pstr,"KE ") strcpy(strSteuer, "KURSIV_EIN");
  else IF(pstr,"KA ") strcpy(strSteuer, "KURSIV_AUS");
  else IF(pstr,"1E ") strcpy(strSteuer, "FREI_1_EIN");
  else IF(pstr,"1A ") strcpy(strSteuer, "FREI_1_AUS");
  else IF(pstr,"2E ") strcpy(strSteuer, "FREI_2_EIN");
  else IF(pstr,"2A ") strcpy(strSteuer, "FREI_2_AUS");
  else IF(pstr,"3E ") strcpy(strSteuer, "FREI_3_EIN");
  else IF(pstr,"3A ") strcpy(strSteuer, "FREI_3_AUS");
  else IF(pstr,"4E ") strcpy(strSteuer, "FREI_4_EIN");
  else IF(pstr,"4A ") strcpy(strSteuer, "FREI_4_AUS");

  boHeadFoot_m=(wBereich==pFkb->wNextHeader ||
    wBereich==pFkb->wNextFoot);

  if(*strSteuer)
    Fo_Print_SteuerKz(GLpt(strSteuer), pfDevice);
  }

return;
} /* end iSteuerKz() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ChangexFF ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ChangexFF(PSSTR pstr, PSSTR apstrRecord[], PFKB pFkb,
		PPKB apPkb[], FILE *pfDevice)
{
SWORD wHead=pFkb->wNextHeader, wFoot=pFkb->wNextFoot;

apstrRecord[0]=apstrRecord[0];
pFkb=pFkb; apPkb[0]=apPkb[0];
pfDevice=pfDevice;

for(; *pstr; pstr++)
  {if(*pstr == '\xff') *pstr = ' ';
   if(*pstr=='') {*pstr=' '; boFF_g=JA;}
  }

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_InitSteuerZ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Druckersteuerzeichen von \xxx in ASCII-Code wandeln.		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_InitSteuerZ()
{

Fo_WandleSteuerZ("INIT_ALLE");
Fo_WandleSteuerZ("DEINIT_ALL");
Fo_WandleSteuerZ("INIT_ERSTE");
Fo_WandleSteuerZ("DEINIT_ERS");

Fo_WandleSteuerZ("SCHR_NORM");
Fo_WandleSteuerZ("SCHR_SCHM");
Fo_WandleSteuerZ("SCHR_SCHÖ");
Fo_WandleSteuerZ("SCHR_FREI");
Fo_WandleSteuerZ("UNTER_EIN");
Fo_WandleSteuerZ("UNTER_AUS");
Fo_WandleSteuerZ("FETT_EIN");
Fo_WandleSteuerZ("FETT_AUS");
Fo_WandleSteuerZ("BREIT_EIN");
Fo_WandleSteuerZ("BREIT_AUS");
Fo_WandleSteuerZ("KURSIV_EIN");
Fo_WandleSteuerZ("KURSIV_AUS");
Fo_WandleSteuerZ("FREI_1_EIN");
Fo_WandleSteuerZ("FREI_1_AUS");
Fo_WandleSteuerZ("FREI_2_EIN");
Fo_WandleSteuerZ("FREI_2_AUS");
Fo_WandleSteuerZ("FREI_3_EIN");
Fo_WandleSteuerZ("FREI_3_AUS");
Fo_WandleSteuerZ("FREI_4_EIN");
Fo_WandleSteuerZ("FREI_4_AUS");

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_iWandleSteuerZ() 						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_iWandleSteuerZ(PSSTR pstrAnf)
{
PSSTR pstr, pstr1, pstr2;
CHAR strText[TB_MAX], cChar, cChar1;

pstr=pstr1=pstrAnf;
*strText='\0';

for(pstr=strchr(pstr, '\\'); pstr; pstr=strchr(pstr, '\\'))
  if(strlen(pstr)>=4 && isdigit(*(pstr+1)) &&
    isdigit(*(pstr+2)) && isdigit(*(pstr+3)))
    {
    *pstr='\0';
    strcat(strText, pstr1);
    pstr++;
    pstr1=pstr+3;
    cChar=*pstr1;
    *pstr1='\0';
    cChar1=(CHAR) atoi(pstr);
    pstr+=3;
    *pstr1=cChar;
    pstr2=&strText[strlen(strText)];
    *pstr2=cChar1;
    *(pstr2+1)='\0';
    }
  else
    pstr++;

strcat(strText, pstr1);
strcpy(pstrAnf, strText);

return(OK);
}  /* Ende Fo_iWandleSteuerZ () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_Print_SteuerKz ()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_Print_SteuerKz(PSSTR pstrTyp, FILE *pfDevice)
{
PSSTR pstr;

while(pstrTyp && *pstrTyp)
  {
  pstr=strstr(pstrTyp, "(null)");
  if(pstr) *pstr='\0';

  if(boHide_m==AUS || boHeadFoot_m)
    {fputs(pstrTyp, pfDevice);
    if(pstr) fputc('\x00', pfDevice);}
  else
    {strcat(strHide_m, pstrTyp);
    strcat(strHide_m, "(null)");}	 /* Erst beim Druck rausfiltern  */

  if(pstr) *pstr='(';
  pstrTyp=(pstr) ? pstr+6 : NULL;
  }

return(OK);
} /* end Fo_Print_SteuerKz() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_Set_Page()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Seite setzten, nächste Kopfzeile drucken.				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_Set_Page(PSSTR apstrRecord[], FILE *pfDevice, PFKB pFkb, PPKB apPkb[],
		 PSSTR pF, SWORD wL)
{

boRekursiv_g=YES;
wZeilenVorLF_m=pFkb->wAktZeile=0;

pFkb->wAktSeite++;
memcpy(GLpt("_SEITE"), &(pFkb->wAktSeite), 2);

Fo_Print_SteuerKz(GLpt("INIT_ALLE"), pfDevice);
Fo_Print(apstrRecord, pfDevice,
   pFkb->wNextHeader, pFkb, apPkb, pF, wL);

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_WahlSelect()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ pstrOut auf 150 Zeichen allocieren!!!				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_WahlSelect(PSSTR pstrSatz, PTEXTBOX pTW,
		   PSSTR apstrRecord[], SWORD wLine, PSSTR pstrFile)
{
CHOICES *pHelp;
SLONG lHelp, lWahl;
PSSTR apstrWahl[25];
SWORD wSelect, i;
PSSTR pstrWahl=&apstrRecord[pTW->wRecord][pTW->wOffset];

memcpy(&lHelp, pstrWahl, 4); lWahl=lHelp;
for(i=0,pHelp=pTW->pSelect; pHelp; pHelp=pHelp->pNext)
  if( (lHelp & atol(pHelp->pstrFormat+4)) )
    apstrWahl[i++]=pHelp->pstrWert+4;
apstrWahl[i]=NULL;

if(pTW->pChoice)
  Dl_Abbruch("Choice Feld", wLine, pstrFile, pTW->strDatenfeld);
else if(!pTW->pSelect)
  Dl_Abbruch("Kein Select Feld", wLine, pstrFile, pTW->strDatenfeld);

memcpy(&lHelp, pstrSatz, 4);
for(wSelect=i=0, pHelp=pTW->pSelect; pHelp; pHelp=pHelp->pNext, i++)
  {
  if( (lHelp & atol(pHelp->pstrFormat+4)) )
    {
    SREGISTER j;

    if(lWahl)
      for(j=0; !wSelect && apstrWahl[j]; j++)
	{if(!strcmp(pHelp->pstrWert+4, apstrWahl[j]))
          wSelect=JA;

	Wi_TestPrintf(pWkbInfo_g, "\nHlp(%s), Wahl(%s).",
	    pHelp->pstrWert+4, apstrWahl[j]);}
    }
  }

if(lWahl==0L) wSelect=JA;
return(wSelect);
} /* end Fo_WahlSelect() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_WahlChoice()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_WahlChoice(PSSTR pstrSatz, PSSTR pstrWahl, PTEXTBOX pTW,
		   SWORD wLine, PSSTR pstrFile)
{
CHOICES *pHelp;
PSSTR apstrFormat[25], apstrWert[25];
SWORD wSelect, i;

if(pTW->pSelect)
  Dl_Abbruch("Select Feld", wLine, pstrFile, pTW->strDatenfeld);
else if(!pTW->pChoice)
  Dl_Abbruch("Kein Choice Feld", wLine, pstrFile, pTW->strDatenfeld);

for(i=0,pHelp=pTW->pChoice; pHelp; i++, pHelp=pHelp->pNext)
   {apstrFormat[i]=pHelp->pstrFormat;
   apstrWert[i]=pHelp->pstrWert;}
apstrFormat[i]=NULL;

if(*pstrWahl=='\0')
  wSelect=JA;
else if(!strcmp(pstrSatz, pstrWahl))
  wSelect=JA;
else wSelect=NEIN;

return(wSelect);
} /* end Fo_WahlChoice() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_WahlStr()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Fo_WahlStr(PSWORD pwRetCode, PSSTR pstrSatz, PSSTR pstrWahl, ...)
{
SWORD wWahl=NEIN;
PPSTR ppstrWahl=&pstrWahl;
PPSTR ppstr=ppstrWahl;

if(*pwRetCode==0) return;

for(ppstr=ppstrWahl; *ppstr; *ppstr++)
  {if(strlen(*ppstr)) wWahl=YES;
  Wi_TestPrintf(pWkbInfo_g, "\nFo_WahlStr(%d/%s/%s).",
    wWahl, pstrSatz, *ppstr);}

if(wWahl==NEIN) return;

*pwRetCode=NEIN;
for(ppstr=ppstrWahl; *ppstr; *ppstr++)
  if(!strcmp(pstrSatz, *ppstr))
    {*pwRetCode=JA;
    Wi_TestPrintf(pWkbInfo_g, "\nFo_WahlStr(%d/%s/%s).",
      *pwRetCode, pstrSatz, *ppstr);}

/* if(pTW->pSelect)
     Dl_Abbruch("Select Feld", wLine, pstrFile, pTW->strDatenfeld); */

return;
} /* end Fo_WahlStr() */


/*
memcpy(pT->acBuffer, pstrSatz, 4);
memcpy(&lHelp, pT->acBuffer, 4);
for(wSelect=i=0, pHelp=pT->pSelect; pHelp; pHelp=pHelp->pNext, i++)
  {
  if( (lHelp & atol(pHelp->pstrFormat)) )
    {
    SREGISTER j;
    *(pT->acBuffer+i/8) |= 1 << i%8;

    swLength = strlen(pHelp->pstrWert) + strlen (acText);
    if (acText[0] != '\0' && acText[0] != ' ' && swLength < 300)
      strcat (acText, ", ");
    if (swLength < 300)
      strcat (acText, pHelp->pstrWert);
    else
      {
       swLength = 300 - strlen(acText);
       if (swLength > 0)
	 strncat (acText, pHelp->pstrWert, swLength);
      }

    if(*pstrWahl)
      for(j=0; !wSelect && apstrWahl[j]; j++)
	if(!strcmp(pHelp->pstrWert, apstrWahl[j]))
          wSelect=JA;

        Wi_TestPrintf(pWkbInfo_g, "\nHlp(%s), Wahl(%s).",
	    pHelp->pstrWert, apstrWahl[j]);
    }
  }

*/
