// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Dateiname:        MS_BASIS.C                     Datum: 18.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Diese Datei enthlt die Grundfunktionen zur Untersttzung der         บ
  บ  Microsoft(TM)-Maus.                                                   บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <dos.h>

#ifndef EUR_MSM_HEADER
#include <eur_msm.h>
#endif

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define MAUS_INT             0x33                  /* Interrupt-Nummer      */
#define MSM_RESET_STATUS     0x00                  /* verwendete Funktionen */
#define MSM_SHOW_CURSOR      0x01                  /* des Maus-INT          */
#define MSM_HIDE_CURSOR      0x02
#define MSM_GET_B_STATUS_POS 0x03
#define MSM_SET_POSITION     0x04
#define MSM_GET_B_PRESS      0x05
#define MSM_GET_B_RELEASE    0x06
#define MSM_SET_MINMAX_HOR   0x07
#define MSM_SET_MINMAX_VER   0x08
#define MSM_SET_TXT_CURSOR   0x0A
#define MSM_GET_MOTION       0x0B
#define MSM_SET_RATIO        0x0F
#define MSM_SET_THRESHOLD    0x13
#define MSM_STORAGE_REQU     0x15
#define MSM_SAVE_DRIVER      0x16
#define MSM_RESTORE_DRIVER   0x17
#define MSM_SET_CRT_PAGE     0x1D
#define MSM_GET_CRT_PAGE     0x1E

#define MSM_SW_CURSOR           0
#define MSM_HW_CURSOR           1


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	BOOL  boMausInitialisiert_m = 0;	     /* Flags - initialisiert*/
STATIC	BOOL  boMsmTreiberda_m	    = 0;	     /*       - Treiber da   */
STATIC	BOOL  boTextMode_m	    = MSM_TEXT_MODUS;/*       - Koord. Rckg.*/
STATIC	SWORD wKopieCursorFlag_m   = 0; 	    /* Kopie des Cursorflag */
MGLOBAL FPBYTE fpbBuffer_m          = 0L;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        FUNKTIONS-DEFINITIONEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_Init                        Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion berprft, ob im System die Maus-    บ
  บ                    Hardware und Software vorhanden ist.                บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   MSM_NO_DRIVER wenn Maus Hard- und Software   บ
  บ                               nicht installiert                        บ
  บ                           >0  Anzahl der Mausbuttons                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            บ
  บ		       wKopieCursorFlag_m (W), wMausda_g (W)		  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_Init(VOID)
{
    IMPORT SWORD wMausda_g;

    union REGS mausregs;

    if (boMausInitialisiert_m != 1)
    {
        boMausInitialisiert_m = 1;                  /* Flag setzen,Funktion */
                                                    /*     wurde aufgerufen */
        mausregs.x.ax = MSM_RESET_STATUS;           /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */

        if (mausregs.x.ax == 0)                     /* wenn AX == Null, ist */
        {
            wMausda_g = NEIN;                       /* globale Var. setzen  */
            return (MSM_NO_DRIVER);                 /* kein Maustreiber da  */
        }
        else
        {
            boMsmTreiberda_m =  1;                  /* Toolbox-Flag setzen  */
	    wKopieCursorFlag_m = -1;		   /* Treiber-Cursor-Flag  */
                                                    /*   mithalten          */
            wMausda_g = JA;                         /* globale Var. setzen  */
            return (mausregs.x.bx);                 /* Anzahl Maus-Buttons  */
	}

    }

return(0); //
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_CursorOn                    Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion inkrementiert das interne Mauscur-   บ
  บ                    sorflag und zeigt den Cursor an, wenn das Flag      บ
  บ                    den Wert 0 hat.                                     บ
  บ                    Nach dem Aufruf von Ms_Init() hat das Flag          บ
  บ                    den Wert -1.                                        บ
  บ                                                                        บ
  บ                    Diese Funktion verwendet eine Kopie dieses Flags    บ
  บ                    damit der Cursor nur eingeschaltet werden kann,     บ
  บ                    wenn er nicht sichtbar ist.                         บ
  บ                                                                        บ
  บ  Rckgabewert:     Status des Mauszeigers beim Funktionsaufruf         บ
  บ                                                                        บ
  บ		       SWORD	  - MSM_WAR_AN	  Cursor war an 	    บ
  บ                              - MSM_WAR_AUS   Cursor war aus            บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            บ
  บ		       wKopieCursorFlag_m (R/W) 			  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_CursorOn (VOID)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m                            /* wenn Maus im System  */
	&& (wKopieCursorFlag_m==-1))		   /*	   und Cursor aus  */
    {
	wKopieCursorFlag_m = 0;
        mausregs.x.ax = MSM_SHOW_CURSOR;            /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
        return(MSM_WAR_AUS);
    }
    return (MSM_WAR_AN);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_CursorOff                   Datum: 16.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion dekrementiert das interne Mauscur-   บ
  บ                    sorflag und entfernt den Cursor vom Bildschirm.     บ
  บ                    Die Bewegungen der Maus werden weiter registriert.  บ
  บ                                                                        บ
  บ                    Diese Funktion verwendet eine Kopie dieses Flags    บ
  บ                    damit der Cursor nur ausgeschaltet werden kann,     บ
  บ                    wenn er sichtbar ist.                               บ
  บ                                                                        บ
  บ  Rckgabewert:     Status des Mauszeigers beim Funktionsaufruf         บ
  บ                                                                        บ
  บ		       SWORD	  - MSM_WAR_AN	  Cursor war an 	    บ
  บ                              - MSM_WAR_AUS   Cursor war aus            บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ		       wKopieCursorFlag_m (R/W) 			  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_CursorOff (VOID)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m                            /* wenn Maus im System  */
	&& (wKopieCursorFlag_m==0))		   /*	    und Cursor an  */
    {
	wKopieCursorFlag_m = -1;
        mausregs.x.ax = MSM_HIDE_CURSOR;            /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
        return(MSM_WAR_AN);
    }
    return(MSM_WAR_AUS);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_GetPosition                 Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSWORD  pwButton     erhlt - Buttonstatus	    บ
  บ		       PSWORD  pwHor		   - horiz. Position	    บ
  บ		       PSWORD  pwVer		   - vert. Position	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ermittelt die aktuelle Position des  บ
  บ                    Mauscursors und den Status der Mausbuttons.         บ
  บ                                                                        บ
  บ                    Das Format der Rckgabewerte von "ver" und "hor"    บ
  บ                    kann ber die Funktion "Ms_SetModus" gesteuert      บ
  บ                    werden. Defaultmแig werden die Grafikkoordinaten   บ
  บ                    in Textkoordinaten umgerechnet.                     บ
  บ                                                                        บ
  บ  Rckgabewert:     ermittelte Werte werden ber bergebene Zeiger      บ
  บ                    zurckgegeben.                                      บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                    boTextMode_m(R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_GetPosition(PSWORD pwButton, PSWORD pwHor, PSWORD pwVer)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_B_STATUS_POS;       /* Funktionsnummer      */
        mausregs.x.bx = 0;                          /* BX-Register l”schen  */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */

        *pwButton = mausregs.x.bx;                  /* ermittelte Werte in  */
        if (boTextMode_m)                           /* bergebene Variablen */
        {                                           /* laden                */
            *pwHor   = mausregs.x.cx >>3;           /* im Textmodus Koordi- */
            *pwVer   = mausregs.x.dx >>3;           /* naten umrechnen      */
        }
        else
        {
            *pwHor   = mausregs.x.cx;
            *pwVer   = mausregs.x.dx;
        }
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetPosition                 Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wHor	 horizontale Position des Cursors   บ
  บ		       SWORD  wVer	 vertikale Position des Cursors     บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion positioniert den Maus-Cursor an      บ
  บ                    der durch wHor/wVer bergebenen Position.           บ
  บ                                                                        บ
  บ                    Defaultmแig werden die bergebenen Koordinaten     บ
  บ                    als Textkoordinaten interpretiert und vor dem       บ
  บ                    Aufruf der Interrupt-Routine in die internen        บ
  บ                    Grafik-Koordinaten des Maustreibers umgewandelt.    บ
  บ                                                                        บ
  บ                    Diese Umwandlung kann ber die Funktion             บ
  บ                    "Ms_SetModus" gesteuert werden.                     บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                    boTextMode_m(R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetPosition(SWORD wHor, SWORD wVer)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        if (boTextMode_m)                           /* evtl. Umwandlung der */
        {                                           /* Koordinaten          */
            mausregs.x.cx = wHor <<3 ;              /* Hor. Koord. nach CX  */
            mausregs.x.dx = wVer <<3 ;              /* Ver. Koord. nach DX  */
        }
        else
        {
            mausregs.x.cx = wHor;                   /* Hor. Koord. nach CX  */
            mausregs.x.dx = wVer;                   /* Ver. Koord. nach DX  */
        }

        mausregs.x.ax = MSM_SET_POSITION;           /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_ButtonPress                 Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	  wButton	 MSM_B_LINKS, MSM_B_RECHTS  บ
  บ		       PSWORD	  pwStatus	 derzeitiger Buttonstatus   บ
  บ		       PSWORD	  pwKlicks	 Anzahl der Klicks	    บ
  บ		       PSWORD	  pwHor 	 horizontale Position	    บ
  บ		       PSWORD	  pwVer 	 vertikale Position	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liefert als Ergebnis Informationen   บ
  บ                    ber die Position des Mauszeigers beim letzten      บ
  บ                    Klicken, die Anzahl der Klicks und den              บ
  บ                    aktuellen Status der Maustasten.                    บ
  บ                                                                        บ
  บ                    Die Position wird defaultmแig in Textkoordinaten   บ
  บ                    bergeben. Wird die bergabe in Grafikkoordinaten   บ
  บ                    gewnscht, kann dies ber die Funktion              บ
  บ                    "Ms_SetModus()" gesteuert werden.                   บ
  บ                                                                        บ
  บ  Rckgabewert:     ermittelte Werte werden ber bergebene Zeiger      บ
  บ                    zurckgegeben.                                      บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            บ
  บ                    boTextMode_m(R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_ButtonPress(SWORD  wButton,
		    PSWORD pwStatus, PSWORD pwKlicks,
		    PSWORD pwHor,    PSWORD pwVer)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_B_PRESS;            /* Funktionsnummer      */
        mausregs.x.bx = wButton;                    /* Taste nach BX        */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */

        *pwStatus     = mausregs.x.ax;              /* ermittelte Daten in  */
        *pwKlicks     = mausregs.x.bx;              /* bergebene Variablen */
        if (boTextMode_m)                           /* laden                */
        {
            *pwHor    = mausregs.x.cx >>3;          /* Umwandlung der Koord.*/
            *pwVer    = mausregs.x.dx >>3;          /*    im Textmodus      */
        }
        else
        {
            *pwHor    = mausregs.x.cx;
            *pwVer    = mausregs.x.dx;
        }
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_ButtonRelease               Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	  wButton	MSM_B_LINKS, MSM_B_RECHTS   บ
  บ		       PSWORD	  pwStatus	derzeitiger Buttonstatus    บ
  บ		       PSWORD	  pwLos 	Anzahl der Freigaben	    บ
  บ		       PSWORD	  pwHor 	horizontale Position	    บ
  บ		       PSWORD	  pwVer 	vertikale Position	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liefert als Ergebnis Informationen   บ
  บ                    ber Position des Mauszeigers beim letzten          บ
  บ                    Loslassen, wie oft die Taste losgelassen wurde und  บ
  บ                    den aktuellen Status der Maustasten.                บ
  บ                                                                        บ
  บ                    Die Position wird defaultmแig in Textkoordinaten   บ
  บ                    bergeben. Wird die bergabe in Grafikkoordinaten   บ
  บ                    gewnscht, kann dies mit der Funktion               บ
  บ                    "Ms_SetModus()" gesteuert werden.                   บ
  บ                                                                        บ
  บ  Rckgabewert:     ermittelte Werte werden ber bergebene Zeiger      บ
  บ                    zurckgegeben (siehe Paramater).                    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                    boTextMode_m  (R)                                   บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_ButtonRelease(SWORD  wButton,
		      PSWORD pwStatus, PSWORD pwLos,
		      PSWORD pwHor,    PSWORD pwVer)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_B_RELEASE;          /* Funktionsnummer      */
        mausregs.x.bx = wButton;                    /* Taste nach BX        */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */

        *pwStatus     = mausregs.x.ax;              /* ermittelte Werte in  */
        *pwLos        = mausregs.x.bx;              /* bergebene Variablen */
        if (boTextMode_m)                           /* laden                */
        {
            *pwHor      = mausregs.x.cx>>3;         /* im Textmodus Koord.  */
            *pwVer      = mausregs.x.dx>>3;         /* umrechnen            */
        }
        else
        {
            *pwHor      = mausregs.x.cx;
            *pwVer      = mausregs.x.dx;
        }
    }
    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_HorMinMax                   Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wMin   minimale Cursorkoordinaten	    บ
  บ		       SWORD   wMax   maximale Cursorkoordinaten	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt die minimale und maximale       บ
  บ                    Koordinate in der Horizontalen fest, innerhalb      บ
  บ                    derer sich der Mauscursor bewegen lแt.             บ
  บ                                                                        บ
  บ                    Defaultmแig werden die bergebenen Koordinaten     บ
  บ                    als Textkoordinaten interpretiert und vor dem       บ
  บ                    Aufruf der Interrupt-Routine in die internen        บ
  บ                    Grafik-Koordinaten des Maustreibers umgewandelt.    บ
  บ                                                                        บ
  บ                    Diese Umwandlung kann ber die Funktion             บ
  บ                    "Ms_SetModus" gesteuert werden.                     บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                    boTextMode_m(R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_HorMinMax(SWORD wMin, SWORD wMax)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        if (boTextMode_m)                           /* im Textmodus werden  */
        {                                           /* die Koordinaten beim */
            mausregs.x.cx = wMin <<3;               /* Laden in die Regs    */
            mausregs.x.dx = wMax <<3;               /* umgewandelt          */
        }
        else
        {
            mausregs.x.cx = wMin;                   /* min. Pos. nach CX    */
            mausregs.x.dx = wMax;                   /* max. Pos. nach DX    */
        }
        mausregs.x.ax = MSM_SET_MINMAX_HOR;         /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_VerMinMax                   Datum: 14.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wMin   minimale Cursorkoordinaten	    บ
  บ		       SWORD   wMax   maximale Cursorkoordinaten	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt die minimale und maximale       บ
  บ                    Koordinate in der Vertikalen fest, innerhalb derer  บ
  บ                    der Cursor bewegt werden kann.                      บ
  บ                                                                        บ
  บ                    Defaultmแig werden die bergebenen Koordinaten     บ
  บ                    als Textkoordinaten interpretiert und vor dem       บ
  บ                    Aufruf der Interrupt-Routine in die internen        บ
  บ                    Grafik-Koordinaten des Maustreibers umgewandelt.    บ
  บ                                                                        บ
  บ                    Diese Umwandlung kann ber die Funktion             บ
  บ                    "Ms_SetModus()" gesteuert werden.                   บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_VerMinMax(SWORD wMin, SWORD wMax)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        if (boTextMode_m)                           /* im Textmodus werden  */
        {                                           /* die Koordinaten beim */
            mausregs.x.cx = wMin <<3;               /* Laden in die Regs    */
            mausregs.x.dx = wMax <<3;               /* umgewandelt          */
        }
        else
        {
            mausregs.x.cx = wMin;                   /* min. Pos. nach CX    */
            mausregs.x.dx = wMax;                   /* max. Pos. nach DX    */
        }

        mausregs.x.ax = MSM_SET_MINMAX_VER;         /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetSoftCursor                 Datum: 17.08.88    บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wScreenmask    Screenmask des SW-Cursors     บ
  บ		       SWORD   wCursormask    Cursormask des SW-Cursors     บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt Screen- und Cursormask des      บ
  บ                    Software-Cursors fest.                              บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetSoftCursor(SWORD wScreenmask,
		      SWORD wCursormask)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_TXT_CURSOR;         /* Funktionsnummer      */
        mausregs.x.bx = MSM_SW_CURSOR;              /* SW-Curs. selektieren */
        mausregs.x.cx = wScreenmask;                /* Parameter laden      */
        mausregs.x.dx = wCursormask;
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetHardCursor               Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wStartscan   Startscanzeile des HW-Cursors   บ
  บ		       SWORD   wStopscan    Stopscanzeile des HW-Cursors    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt die Gr”แe des Hardware-Text-    บ
  บ                    Cursors fest. Die Parameter fr die Angabe der      บ
  บ                    Zeilen sind von der verwendeten Grafikkarte         บ
  บ                    abhngig.                                           บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetHardCursor(SWORD wStartscan,
		      SWORD wStopscan)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_TXT_CURSOR;         /* Funktionsnummer      */
        mausregs.x.bx = MSM_HW_CURSOR;              /* HW-Curs. selektieren */
        mausregs.x.cx = wStartscan;                 /* Parameter laden      */
        mausregs.x.dx = wStopscan;
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_GetMickeys                  Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSWORD	pwHorMickeys				    บ
  บ		       PSWORD	pwVerMickeys				    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ermittelt die letzten Bewegungen     บ
  บ                    der Maus in Mickeys seit dem letzten Aufruf der     บ
  บ                    Funktion. Die zurckgegebenen Werte mssen          บ
  บ                    folgendermaแen interpretiert werden:                บ
  บ                    pwHorMickeys  negativ  Bewegung nach links          บ
  บ                                  positiv  Bewegung nach rechts         บ
  บ                    pwVerMickeys  negativ  Bewegung nach oben           บ
  บ                                  positiv  Bewegung nach unten          บ
  บ                                                                        บ
  บ                    1 Mickey entspricht 1/200 inch                      บ
  บ                                                                        บ
  บ  Rckgabewert:     ermittelte Werte werden ber bergebene Zeiger      บ
  บ                    zurckgegeben.                                      บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_GetMickeys(PSWORD pwHorMickeys, PSWORD pwVerMickeys)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_MOTION;             /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
        *pwHorMickeys = mausregs.x.cx;              /* ermittelte Werte     */
        *pwVerMickeys = mausregs.x.dx;              /* laden                */
    }
    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetRatio                    Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wHorRatio				    บ
  บ		       SWORD   wVerRatio				    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt das Verhltnis von Mickeys      บ
  บ                    zu Pixeln fr die horizontale und vertikale Bewe-   บ
  บ                    gung der Maus fest.                                 บ
  บ                    Mit diesem Wert wird die Anzahl von Mickeys pro     บ
  บ                    Pixel festgelegt. Die bergabewerte mssen im       บ
  บ                    Bereich zwischen 1..32767 liegen.                   บ
  บ                                                                        บ
  บ                    Der Default-Wert nach Aufruf von Ms_Init() betrgt  บ
  บ                        - horizontal:  8 Mickeys pro 8 Pixel            บ
  บ                        - vertikal:   16 Mickeys pro 8 Pixel            บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetRatio(SWORD wHorRatio, SWORD wVerRatio)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_RATIO;              /* Funktionsnummer      */
        mausregs.x.cx = wHorRatio;                  /* Parameter laden      */
        mausregs.x.dx = wVerRatio;
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetThreshold                Datum: 14.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wSpeed					    บ
  บ                                                                        บ
  บ  Beschreibung:     Mit dieser Funktion wird die Bewegungsgeschwindig-  บ
  บ                    keit der Maus festgelegt, ab der die Bewegung des   บ
  บ                    Maus-Cursors auf dem Bildschirm verdoppelt wird.    บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetThreshold(SWORD wSpeed)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_THRESHOLD;          /* Funktionsnummer      */
        mausregs.x.dx = wSpeed;                     /* neue Geschwindigkeit */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}









/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_TreiberSichern              Datum: 14.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion sichert den Status des Maustreibers  บ
  บ                    in einem modulglobalen Puffer.                      บ
  บ                    Sie k”nnen so ein Programm unterbrechen, das die    บ
  บ                    Maus benutzt und ein anderes Programm starten, das  บ
  บ                    ebenfalls die Maus verwendet.                       บ
  บ                                                                        บ
  บ                    s. a. Ms_TreiberRestore()                           บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   OK     wenn Treiber gesichert wurde	    บ
  บ                           ERROR  wenn kein Maustreiber installiert     บ
  บ                                  oder kein Speicher fr den Maus-      บ
  บ                                  treiber allokiert werden konnte       บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                    fpbBuffer_m(R/W)                                    บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_TreiberSichern(VOID)
{
    SWORD   wGroesse;
    SWORD   wReturn;

    union  REGS mausregs;
    struct SREGS sregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_STORAGE_REQU;           /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
        wGroesse = mausregs.x.bx;                   /* Gr”sse merken        */
	if ((fpbBuffer_m=(FPBYTE)Ut_Fmalloc(wGroesse*sizeof(SBYTE))) == 0L)
            wReturn = ERROR;
        mausregs.x.ax = MSM_SAVE_DRIVER;            /* Funktionsnummer      */
        mausregs.x.dx = FP_OFF(fpbBuffer_m);        /* Adresse des Puffers  */
        sregs.es      = FP_SEG(fpbBuffer_m);
        int86x(MAUS_INT, &mausregs, &mausregs, &sregs);
        wReturn = OK;
    }
    else
        wReturn = ERROR;
    return (wReturn);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_TreiberRestore              Datum: 14.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ldt den mit Ms_TreiberSichern()     บ
  บ                    gesicherten Status des Maustreibers.                บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   OK     wenn alter Maustreiberstatus geladen  บ
  บ                                  wurde                                 บ
  บ                           ERROR  wenn ein Null-Zeiger auf den Speicher บ
  บ                                  des alten Maustreibers zeigt          บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_TreiberRestore(VOID)
{
    SWORD   wReturn=0;
    union  REGS mausregs;
    struct SREGS sregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        if (fpbBuffer_m == 0L)
            wReturn = ERROR;
        else
        {
            mausregs.x.ax = MSM_RESTORE_DRIVER;     /* Funktionsnummer      */
            sregs.es      =  FP_SEG (fpbBuffer_m);  /* Adresse des Puffers  */
            mausregs.x.dx =  FP_OFF (fpbBuffer_m);  /* Interrupt ausl”sen   */
            int86x(MAUS_INT, &mausregs, &mausregs, &sregs);
            Ut_Ffree(fpbBuffer_m);
            fpbBuffer_m = 0L;
            wReturn = OK;
        }
    }
    return (wReturn);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetSeite                    Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   seite					    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt die Anzeigeseite fest, auf      บ
  บ                    der der Cursor angezeigt wird.                      บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ms_SetSeite(SWORD wSeite)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_CRT_PAGE;           /* Funktionsnummer      */
        mausregs.x.bx = wSeite;                     /* gewnschte Seite     */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
    }
    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_GetSeite                    Datum: 18.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ermittelt die Anzeigeseite, auf      บ
  บ                    der der Mauscursor angezeigt wird.                  บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD  == MSM_NO_DRIVER	kein Maustreiber vorhanden  บ
  บ                          => 0            Anzeigeseite                  บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_GetSeite(VOID)
{
    SWORD	 wSeite;
    union REGS  mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_CRT_PAGE;           /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt ausl”sen   */
        wSeite = mausregs.x.bx;                     /* ermittelte Seite     */
    }
    else                                            /* sonst 255 zurck     */
        wSeite = MSM_NO_DRIVER;

    return (wSeite);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ms_SetModus                    Datum: 17.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wModus	     MSM_GRAF_MODUS		    บ
  บ                                         MSM_TEXT_MODUS                 บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt fest, ob die Rckgabewerte      บ
  บ                    der Funktionen, die sich auf Bildschirmpositionen   บ
  บ                    beziehen, im Grafik- oder Textmodus zurckgegeben   บ
  บ                    werden.                                             บ
  บ                    Der Maustreiber arbeitet nur in Grafikkoordina-     บ
  บ                    ten (0..639, 0..239). Wenn mit dieser Funktion      บ
  บ                    der Textmodus selektiert wird, werden die           บ
  บ                    Rckgabewerte in Textkoordinaten (0..79, 0..24)     บ
  บ                    umgerechnet.                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD	der aktuelle Modus			    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boTextMode_m(R/W)                                   บ
  บ                    boMausInitialisiert_m, boMsmTreiberda_m (R)         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ms_SetModus(SWORD wModus)
{
    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
        switch (wModus)                             /* Modus auswerten      */
        {
            case MSM_TEXT_MODUS:
                 if (!boTextMode_m)
                    boTextMode_m=MSM_TEXT_MODUS;
                 break;

            case MSM_GRAF_MODUS:
                 if (boTextMode_m)
                     boTextMode_m=MSM_GRAF_MODUS;
                 break;
        }
    return(boTextMode_m);
}
