// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Wi_RS232Init(), Wi_Wahl(PSTR)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_RS232Init(), Wi_Wahl()						       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_RS232Init();
SWORD Wi_Wahl(pstrTelNummer);

Parameter:
────────────────────────────────────────────────────────────────────────────────

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion schreibt ein Zeichen an die aktuelle Cursorposition. Das zugehörige

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wMausda_g (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Siehe Beispiel zu »Wi_Cls()«

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <string.h>
#include <dos.h>
#include <eur_tool.h>

STATIC	SWORD  i_ZeilEnd(VOID);
STATIC	SWORD  i_PufIni(VOID);
STATIC	SWORD  i_KbStat(VOID);
STATIC	SWORD i_ReaByt(VOID);
STATIC	SWORD i_SenByt(SWORD);
STATIC	SWORD  i_TTY_Flush(VOID);
STATIC	SWORD  i_Build_TelNr (PSSTR);
STATIC	SWORD  i_TelefonBetrieb(VOID);
STATIC	SWORD  i_Hilfs_Text(VOID);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  G l o b a l e   V a r i a b l e n                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];
IMPORT SWORD	 wCSInd_g;

extern int ICLEAR(VOID);
extern int ISET(VOID);
extern int IVECTOR(VOID);
extern int IRESTORE(VOID);
extern int SRPREP(PPSTR, CHAR[], CHAR[]);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define CR       13
#define LF       10
#define ESC      27
#define XON      17
#define XOFF     19

#define L_BUF    2000              /* Laenge f. strRecBuf in SRPREP   */

STATIC	CHAR cEZei_m;
STATIC	CHAR cZEnd_m=3;
STATIC	CHAR cXofL_m;
STATIC	PSSTR pstrWtAdr_m, pstrRdAdr_m, pstrEom_m, pstrStaBufs_m;
STATIC	SWORD wEsc_m, wMsb_m;
STATIC	BOOL boDownL_m, boEcho_m;
STATIC	SWORD wZe_m;

//IMPORT CHAR	    strModemTyp_g[4];
  IMPORT CHAR	    strMoDevice_g[2];	   // Wählautomat
  IMPORT CHAR	    strPulsWahl_g[21];
  IMPORT CHAR	    strAmtsLeit_g[21];
  IMPORT CHAR	    strWahlEnd_g[21];
  IMPORT CHAR	    strTelBetr_g[21];
  IMPORT CHAR	    strVorwahl_g[21];


IMPORT	ERRINFO errInfo_g;

STATIC	PSSTR pstrVorwahl_m;
STATIC	PSSTR apstrNr_m[8];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ PC als Telefon-Wählautomat.                                             ║
  ║ Die empfangenen Daten werden im Speicher abgelegt.                      ║
  ║ Da im Polling gearbeitet wird: nicht über 300 Baud verwenden!           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_Wahl(PSSTR pstrTel)
{
PSSTR pstrTelNummer;
CHAR strTelNr[TB_MAX], cTmp;
SWORD wOff=0;

i_TTY_Flush();

pstrTelNummer=strdup(pstrTel);
i_Build_TelNr (pstrTelNummer);

cXofL_m = XON;                                       /*                      */
boDownL_m = wEsc_m = NEIN;                           /*                      */
i_PufIni();					     /* 		     */

for(;;)
  {
  wZe_m = i_ReaByt();				     /* Schnittstelle prüfen */

  if(wZe_m != EOF)
    {
    wZe_m &= 0x7f;                                   /*                      */
    if((wZe_m == XON) || (wZe_m == XOFF))            /*                      */
      cXofL_m = (CHAR)wZe_m;                         /*                      */
    else                                             /*                      */
      {                                              /*                      */
      if(wZe_m != CR)                                /*                      */
        Wi_Printf("%c", wZe_m);                      /*                      */

      if(pstrWtAdr_m > pstrEom_m)
	i_PufIni();				     /* Empfangenes Zeichen  */
                                                     /* im Speicher ablegen  */
      *pstrWtAdr_m++ = (CHAR)wZe_m;                  /*                      */
      }

      continue;

    }  /* if wZe_m != EOF */

  if(boDownL_m)                                      /* Ist File - Download  */
    {                                                /* aktiv ?              */
    cTmp=strTelNr[wOff++];
      if(cTmp == '\0')
        {
	i_SenByt(cEZei_m ^ wMsb_m);
        boDownL_m = NEIN;
        }
      else
        {
        wZe_m = cTmp;
        if (wZe_m == CR) continue;
	if (wZe_m == LF) i_ZeilEnd();
	else i_SenByt(wZe_m ^ wMsb_m);

        }
      continue;
    }  /* if boDownL_m */
                                                     /*                      */
    wZe_m = i_KbStat(); 			     /* Liegt Zeichen von    */
    if(wZe_m == T_F8)				     /* Tastatur an ?	     */
      i_TelefonBetrieb();			     /* F8= Modem aus	     */
    else if(wZe_m == T_F1)			     /* F1= Hilfe	     */
      i_Hilfs_Text();
    else if(wZe_m != 0) 			     /* Liegt Zeichen von    */
      { 					     /* Tastatur an	     */
      if(boEcho_m)
        {
        if(wZe_m == CR)                              /* Echo wird lokal er-  */
	  Wi_Printf("%c",LF);			     /* zeugt		     */
        else
          Wi_Printf("%c", wZe_m);                    /*                      */
        }

      if(wZe_m == ESC)
        {
        wEsc_m = 1;
        continue;
        }

      if(wEsc_m)
        {                                            /*                      */
        if(wZe_m == '0') break;                      /* ESC 0 = Ende         */
	wEsc_m = 0;				     /* ESC 1..8 = Download  */
	if(wZe_m >= '1' && wZe_m <= '8')	     /* ESC 9 = ruecksetzen  */
	  {
	  PSSTR pstrTmp;			      /* Pufferpointer	      */
	  SWORD wNr=wZe_m-49;

	  strcpy(strTelNr, strPulsWahl_g);
	  strcat(strTelNr, strAmtsLeit_g);

	  pstrTmp=strchr(apstrNr_m[wNr], '|');
	  if(pstrTmp)
	    strcat(strTelNr, ++pstrTmp);
	  else
	    {
	    strcat(strTelNr, pstrVorwahl_m);
	    strcat(strTelNr, apstrNr_m[wNr]);
	    }

	  if(*apstrNr_m[wNr] != '\0')
	    strcat(strTelNr, strWahlEnd_g);

          wOff=0;
          boDownL_m = JA;
          continue;
          }

	if(wZe_m == '9')
          {
	  i_PufIni();
          continue;
          }

	i_SenByt(ESC ^ wMsb_m); 		     /* war nicht ESC 0 ..   */
	}   /* if wEsc_m */			     /* ... ESC 9	     */

      if(cXofL_m != XON)
        {
        Wi_Putch(7);
        continue;
        }
                                                     /* Zeichen senden       */
      i_SenByt(wZe_m ^ wMsb_m); 		     /* wird als Echo angeze.*/
      }  /* if wZe_m != 0 */

  }   /* while (1) */

return(OK);
}   /* end Wahl */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Modem aus / Telefonbetrieb						    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_TelefonBetrieb()
{
CHAR strTemp[30];
SREGISTER i;

stradd(strTemp, strTelBetr_g, "\x0D", _N);

for(i=0; strTemp[i]; i++)
  {
  SREGISTER n;
  i_SenByt(strTemp[i] ^ wMsb_m);		 /* wird als Echo angeze.*/
  for (n=0; n < 30000; n++);			 /* Zeitverzögerung	 */
  wZe_m = i_ReaByt();				 /* Schnittstelle prüfen */
  Wi_Printf("%c", wZe_m);			 /*			 */
  }

return(OK);
}   /* end i_TelefonBetrieb */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Zeilen-Endbehandlung                                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ZeilEnd()
{
SWORD n;

if(cZEnd_m == '1') i_SenByt(CR ^ wMsb_m);
if(cZEnd_m == '2') i_SenByt(LF ^ wMsb_m);
if(cZEnd_m == '3')
  {
  i_SenByt(CR ^ wMsb_m);
  i_SenByt(LF ^ wMsb_m);
  }

for (n=0; n < 3000; n++);                            /* Zeitverzögerung      */

return(OK);
}   /* end i_ZeilEnd */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Puffer initialisieren                                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_PufIni()
{
pstrWtAdr_m = pstrStaBufs_m;
pstrRdAdr_m = pstrStaBufs_m;

return (OK);
}   /* end i_PufIni */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Tastaturstatus prüfen und ggf. Zeichen liefern                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_KbStat()
{
EVENT Event;
PEVENT pEvent;
pEvent=&Event;

Ut_Event(pEvent);

if(pEvent->wArt == EVENT_TASTE)
  return (pEvent->wTaste);
else
  return (0);
}   /* end KbStat */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ein Zeichen seriell empfangen. Liefert -1 falls kein Zeichen da.        ║
  ║ Sonst Rückgabe des empfangenen Zeichens.                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ReaByt()
{
if((inp(0x3fd) & 0x01) == 0)                         /* Funktion get input   */
  return(-1);                                        /* status               */

return(inp(0x3f8));                                  /* get character */
}   /* end i_ReaByt */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ein Zeichen seriell senden.                                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_SenByt(SWORD wByt)
{
while (! (inp(0x3fd) & 0x20));                       /* warten bis transmit- */
                                                     /* ter Holding-Reg leer */
outp(0x3f8, wByt);                                   /* jetzt Zeichen ausgeb.*/

return(0);
}   /* end i_SenByt */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Serielle Leitung von Schmutzzeichen räumen.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_TTY_Flush()
{
for(;;)
  if(i_ReaByt() == EOF)
    break;

return (OK);
}   /* end i_TTY_Flush */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ RS-232 Schnittstelle initialisieren:                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_RS232Init(PPWKB ppWkbTel)
{
/* PSSTR pstrWadr;
   CHAR strRecBuf[L_BUF]; */
int  w8259;
SWORD wSpalte=3,
     wZeile=6,
     wBreite=70,
     wHoehe=17;
SWORD wTitelAttr, wRahmenAttr, wInnenAttr;

outp(0x3fc,0);                                       /* Reset fuer UART 8250 */
inp(0x3fd);
inp(0x3f8);
inp(0x3fe);

/*pstrWadr=strRecBuf;				     /* 		     */
/*SRPREP(&pstrWadr, strRecBuf,			     /* Interrupt Service    */
/*	 &strRecBuf[sizeof(strRecBuf)-1]);	     /* Routine vorbereiten  */

/*IVECTOR();					     /* Interrupt Service    */
						     /* Routine vorbereiten. */
w8259=inp(0x21);				     /* Enable-Bit am 8259   */
outp(0x21, w8259 & 0xef);			     /* setzen. 	     */

outp(0x3f9, 0x01);				     /* Receive Interrupt    */
						     /* am 8250 erlauben.    */
outp(0x3f9, 0x0b);				     /* DTR, RTS und OUT2    */
						     /* setzen (=Hardware    */
						     /* Int. enable)	     */
/*ISET();*/

/*ICLEAR();					     /* Ende der Kommunik.   */
outp(0x21,w8259);                                    /* Seriellen Interrupt  */
                                                     /* am 8259 verbieten.   */
outp(0x3f9,0);                                       /* Interrupts am 8250   */
                                                     /* verbieten.           */
outp(0x3fc,0x03);                                    /* Leitung out2 auf 0   */
                                                     /* setzen = Interrupt   */
                                                     /* enable abklemmen.    */
/*IRESTORE();					     /* Original Interrupt-  */
                                                     /* vektor wieder einst. */
/*ISET();*/

pstrStaBufs_m=(PSSTR)malloc(0x7fff);		      // Anfangsadresse der
if(pstrStaBufs_m == NULL)                            /* freien Speichers     */
  {                                                  /*                      */
  printf("\n\n--- Nicht genug Speicher ---");        /*                      */
  E(); exit(1); 				     /* 		     */
  }                                                  /*                      */

pstrEom_m = pstrStaBufs_m + 0x7fff - 1;              /* Speicherende fest-   */
                                                     /* legen                */
i_PufIni();					     /* 		     */
boEcho_m=NEIN;

if(( *ppWkbTel = Wi_Einrichten(wSpalte, wZeile, wBreite, wHoehe)) == NULL)
  {
  printf("\n\nFenster für Tel. - Wählautomat konte nicht eingerichtet werden.");
  E(); exit(1);
  }

Wi_SchattenEin(*ppWkbTel);

/* wTitelAttr = aCS_g[wCSInd_g].wCs_mz_c;
   wRahmenAttr = aCS_g[wCSInd_g].wCs_mz_c;
   wInnenAttr = aCS_g[wCSInd_g].wCs_ca;  */

wTitelAttr  = VH(HELLWEISS, KOBALT);
wRahmenAttr = VH(HELLWEISS, KOBALT);
wInnenAttr  = VH(HELLWEISS, KOBALT);

Wi_SetAttri(*ppWkbTel, wTitelAttr, wRahmenAttr, wInnenAttr);
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Build_TelNr ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Telefonnummer für Wählautomatik bilden.				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Build_TelNr (PSSTR pstrTelNr)
{
SREGISTER i;
PSSTR pstrTmp=strchr(pstrTelNr, '/');
SWORD wEnd=NEIN;

if(pstrTmp)
  {
  *pstrTmp='\0';
  pstrVorwahl_m=pstrTelNr;
  pstrTelNr=++pstrTmp;
  }

if( boStrCmp(pstrVorwahl_m, strVorwahl_g) )
   *pstrVorwahl_m='\0';

for(i=0; i<8; i++)
  {
  apstrNr_m[i]=pstrTelNr;

  pstrTmp=strchr(pstrTelNr, ',');

  if(pstrTmp)
    {
    *pstrTmp='\0';
    pstrTmp++;
    pstrTelNr=pstrTmp;
    }
  else
    pstrTelNr=strchr(pstrTelNr, '\0');

  }

i_Hilfs_Text();
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Hilfs_Text()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ [F1]-Taste und nach Aufruf Hilfstext ausgeben.			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Hilfs_Text ()
{
SREGISTER i;
SWORD wEnd=NEIN;

Wi_Printf("\n┌── PC als Wählautomat ──┬─────(Vw:%s)───── [F8]= Modem aus", pstrVorwahl_m);
Wi_Printf("\n└── [ESC][0]= Ende ──────┘     » Wählen mit [ESC][Nr] «\n");
Wi_Printf("\n Nr.: 1= %s", apstrNr_m[0]);
if(!*apstrNr_m[0])
   Wi_Printf("frei.");

for(i=1; i<4; i++)
  if(*apstrNr_m[i])
    Wi_Printf(", %d=%s", i+1, apstrNr_m[i]);

if(*apstrNr_m[4])
  Wi_Printf("\n Nr.: 5=%s", apstrNr_m[4]);

for(i=5; i<8; i++)
  if(*apstrNr_m[i])
    Wi_Printf(", %d=%s", i+1, apstrNr_m[i]);

for(i=0; i<8 && !wEnd; i++)
  if(*apstrNr_m[0] && !*apstrNr_m[i])
    {
    Wi_Printf(", %d= frei.", i+1);
    wEnd=JA;
    }

Wi_Printf("\n\n");
return(OK);
}
