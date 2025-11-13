// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        MS_BASIS.C                     Datum: 18.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Diese Datei enthält die Grundfunktionen zur Unterstützung der         ║
  ║  Microsoft(TM)-Maus.                                                   ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <dos.h>

#ifndef EUR_MSM_HEADER
#include <eur_msm.h>
#endif

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	BOOL  boMausInitialisiert_m = 0;	     /* Flags - initialisiert*/
STATIC	BOOL  boMsmTreiberda_m	    = 0;	     /*       - Treiber da   */
STATIC	BOOL  boTextMode_m	    = MSM_TEXT_MODUS;/*       - Koord. Rückg.*/
STATIC	SWORD wKopieCursorFlag_m   = 0; 	    /* Kopie des Cursorflag */
MGLOBAL FPBYTE fpbBuffer_m          = 0L;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        FUNKTIONS-DEFINITIONEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_Init                        Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion überprüft, ob im System die Maus-    ║
  ║                    Hardware und Software vorhanden ist.                ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   MSM_NO_DRIVER wenn Maus Hard- und Software   ║
  ║                               nicht installiert                        ║
  ║                           >0  Anzahl der Mausbuttons                   ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            ║
  ║		       wKopieCursorFlag_m (W), wMausda_g (W)		  ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */

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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_CursorOn                    Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion inkrementiert das interne Mauscur-   ║
  ║                    sorflag und zeigt den Cursor an, wenn das Flag      ║
  ║                    den Wert 0 hat.                                     ║
  ║                    Nach dem Aufruf von Ms_Init() hat das Flag          ║
  ║                    den Wert -1.                                        ║
  ║                                                                        ║
  ║                    Diese Funktion verwendet eine Kopie dieses Flags    ║
  ║                    damit der Cursor nur eingeschaltet werden kann,     ║
  ║                    wenn er nicht sichtbar ist.                         ║
  ║                                                                        ║
  ║  Rückgabewert:     Status des Mauszeigers beim Funktionsaufruf         ║
  ║                                                                        ║
  ║		       SWORD	  - MSM_WAR_AN	  Cursor war an 	    ║
  ║                              - MSM_WAR_AUS   Cursor war aus            ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            ║
  ║		       wKopieCursorFlag_m (R/W) 			  ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
        return(MSM_WAR_AUS);
    }
    return (MSM_WAR_AN);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_CursorOff                   Datum: 16.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion dekrementiert das interne Mauscur-   ║
  ║                    sorflag und entfernt den Cursor vom Bildschirm.     ║
  ║                    Die Bewegungen der Maus werden weiter registriert.  ║
  ║                                                                        ║
  ║                    Diese Funktion verwendet eine Kopie dieses Flags    ║
  ║                    damit der Cursor nur ausgeschaltet werden kann,     ║
  ║                    wenn er sichtbar ist.                               ║
  ║                                                                        ║
  ║  Rückgabewert:     Status des Mauszeigers beim Funktionsaufruf         ║
  ║                                                                        ║
  ║		       SWORD	  - MSM_WAR_AN	  Cursor war an 	    ║
  ║                              - MSM_WAR_AUS   Cursor war aus            ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║		       wKopieCursorFlag_m (R/W) 			  ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
        return(MSM_WAR_AN);
    }
    return(MSM_WAR_AUS);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_GetPosition                 Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSWORD  pwButton     erhält - Buttonstatus	    ║
  ║		       PSWORD  pwHor		   - horiz. Position	    ║
  ║		       PSWORD  pwVer		   - vert. Position	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ermittelt die aktuelle Position des  ║
  ║                    Mauscursors und den Status der Mausbuttons.         ║
  ║                                                                        ║
  ║                    Das Format der Rückgabewerte von "ver" und "hor"    ║
  ║                    kann über die Funktion "Ms_SetModus" gesteuert      ║
  ║                    werden. Defaultmäßig werden die Grafikkoordinaten   ║
  ║                    in Textkoordinaten umgerechnet.                     ║
  ║                                                                        ║
  ║  Rückgabewert:     ermittelte Werte werden über übergebene Zeiger      ║
  ║                    zurückgegeben.                                      ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                    boTextMode_m(R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ms_GetPosition(PSWORD pwButton, PSWORD pwHor, PSWORD pwVer)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_B_STATUS_POS;       /* Funktionsnummer      */
        mausregs.x.bx = 0;                          /* BX-Register löschen  */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */

        *pwButton = mausregs.x.bx;                  /* ermittelte Werte in  */
        if (boTextMode_m)                           /* übergebene Variablen */
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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetPosition                 Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wHor	 horizontale Position des Cursors   ║
  ║		       SWORD  wVer	 vertikale Position des Cursors     ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion positioniert den Maus-Cursor an      ║
  ║                    der durch wHor/wVer übergebenen Position.           ║
  ║                                                                        ║
  ║                    Defaultmäßig werden die übergebenen Koordinaten     ║
  ║                    als Textkoordinaten interpretiert und vor dem       ║
  ║                    Aufruf der Interrupt-Routine in die internen        ║
  ║                    Grafik-Koordinaten des Maustreibers umgewandelt.    ║
  ║                                                                        ║
  ║                    Diese Umwandlung kann über die Funktion             ║
  ║                    "Ms_SetModus" gesteuert werden.                     ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                    boTextMode_m(R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_ButtonPress                 Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	  wButton	 MSM_B_LINKS, MSM_B_RECHTS  ║
  ║		       PSWORD	  pwStatus	 derzeitiger Buttonstatus   ║
  ║		       PSWORD	  pwKlicks	 Anzahl der Klicks	    ║
  ║		       PSWORD	  pwHor 	 horizontale Position	    ║
  ║		       PSWORD	  pwVer 	 vertikale Position	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liefert als Ergebnis Informationen   ║
  ║                    über die Position des Mauszeigers beim letzten      ║
  ║                    Klicken, die Anzahl der Klicks und den              ║
  ║                    aktuellen Status der Maustasten.                    ║
  ║                                                                        ║
  ║                    Die Position wird defaultmäßig in Textkoordinaten   ║
  ║                    übergeben. Wird die Übergabe in Grafikkoordinaten   ║
  ║                    gewünscht, kann dies über die Funktion              ║
  ║                    "Ms_SetModus()" gesteuert werden.                   ║
  ║                                                                        ║
  ║  Rückgabewert:     ermittelte Werte werden über übergebene Zeiger      ║
  ║                    zurückgegeben.                                      ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m,            ║
  ║                    boTextMode_m(R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */

        *pwStatus     = mausregs.x.ax;              /* ermittelte Daten in  */
        *pwKlicks     = mausregs.x.bx;              /* übergebene Variablen */
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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_ButtonRelease               Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	  wButton	MSM_B_LINKS, MSM_B_RECHTS   ║
  ║		       PSWORD	  pwStatus	derzeitiger Buttonstatus    ║
  ║		       PSWORD	  pwLos 	Anzahl der Freigaben	    ║
  ║		       PSWORD	  pwHor 	horizontale Position	    ║
  ║		       PSWORD	  pwVer 	vertikale Position	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liefert als Ergebnis Informationen   ║
  ║                    über Position des Mauszeigers beim letzten          ║
  ║                    Loslassen, wie oft die Taste losgelassen wurde und  ║
  ║                    den aktuellen Status der Maustasten.                ║
  ║                                                                        ║
  ║                    Die Position wird defaultmäßig in Textkoordinaten   ║
  ║                    übergeben. Wird die Übergabe in Grafikkoordinaten   ║
  ║                    gewünscht, kann dies mit der Funktion               ║
  ║                    "Ms_SetModus()" gesteuert werden.                   ║
  ║                                                                        ║
  ║  Rückgabewert:     ermittelte Werte werden über übergebene Zeiger      ║
  ║                    zurückgegeben (siehe Paramater).                    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                    boTextMode_m  (R)                                   ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */

        *pwStatus     = mausregs.x.ax;              /* ermittelte Werte in  */
        *pwLos        = mausregs.x.bx;              /* übergebene Variablen */
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




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_HorMinMax                   Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wMin   minimale Cursorkoordinaten	    ║
  ║		       SWORD   wMax   maximale Cursorkoordinaten	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt die minimale und maximale       ║
  ║                    Koordinate in der Horizontalen fest, innerhalb      ║
  ║                    derer sich der Mauscursor bewegen läßt.             ║
  ║                                                                        ║
  ║                    Defaultmäßig werden die übergebenen Koordinaten     ║
  ║                    als Textkoordinaten interpretiert und vor dem       ║
  ║                    Aufruf der Interrupt-Routine in die internen        ║
  ║                    Grafik-Koordinaten des Maustreibers umgewandelt.    ║
  ║                                                                        ║
  ║                    Diese Umwandlung kann über die Funktion             ║
  ║                    "Ms_SetModus" gesteuert werden.                     ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                    boTextMode_m(R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_VerMinMax                   Datum: 14.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wMin   minimale Cursorkoordinaten	    ║
  ║		       SWORD   wMax   maximale Cursorkoordinaten	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt die minimale und maximale       ║
  ║                    Koordinate in der Vertikalen fest, innerhalb derer  ║
  ║                    der Cursor bewegt werden kann.                      ║
  ║                                                                        ║
  ║                    Defaultmäßig werden die übergebenen Koordinaten     ║
  ║                    als Textkoordinaten interpretiert und vor dem       ║
  ║                    Aufruf der Interrupt-Routine in die internen        ║
  ║                    Grafik-Koordinaten des Maustreibers umgewandelt.    ║
  ║                                                                        ║
  ║                    Diese Umwandlung kann über die Funktion             ║
  ║                    "Ms_SetModus()" gesteuert werden.                   ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetSoftCursor                 Datum: 17.08.88    ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wScreenmask    Screenmask des SW-Cursors     ║
  ║		       SWORD   wCursormask    Cursormask des SW-Cursors     ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt Screen- und Cursormask des      ║
  ║                    Software-Cursors fest.                              ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetHardCursor               Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wStartscan   Startscanzeile des HW-Cursors   ║
  ║		       SWORD   wStopscan    Stopscanzeile des HW-Cursors    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt die Größe des Hardware-Text-    ║
  ║                    Cursors fest. Die Parameter für die Angabe der      ║
  ║                    Zeilen sind von der verwendeten Grafikkarte         ║
  ║                    abhängig.                                           ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_GetMickeys                  Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSWORD	pwHorMickeys				    ║
  ║		       PSWORD	pwVerMickeys				    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ermittelt die letzten Bewegungen     ║
  ║                    der Maus in Mickeys seit dem letzten Aufruf der     ║
  ║                    Funktion. Die zurückgegebenen Werte müssen          ║
  ║                    folgendermaßen interpretiert werden:                ║
  ║                    pwHorMickeys  negativ  Bewegung nach links          ║
  ║                                  positiv  Bewegung nach rechts         ║
  ║                    pwVerMickeys  negativ  Bewegung nach oben           ║
  ║                                  positiv  Bewegung nach unten          ║
  ║                                                                        ║
  ║                    1 Mickey entspricht 1/200 inch                      ║
  ║                                                                        ║
  ║  Rückgabewert:     ermittelte Werte werden über übergebene Zeiger      ║
  ║                    zurückgegeben.                                      ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ms_GetMickeys(PSWORD pwHorMickeys, PSWORD pwVerMickeys)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_GET_MOTION;             /* Funktionsnummer      */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
        *pwHorMickeys = mausregs.x.cx;              /* ermittelte Werte     */
        *pwVerMickeys = mausregs.x.dx;              /* laden                */
    }
    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetRatio                    Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wHorRatio				    ║
  ║		       SWORD   wVerRatio				    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt das Verhältnis von Mickeys      ║
  ║                    zu Pixeln für die horizontale und vertikale Bewe-   ║
  ║                    gung der Maus fest.                                 ║
  ║                    Mit diesem Wert wird die Anzahl von Mickeys pro     ║
  ║                    Pixel festgelegt. Die Übergabewerte müssen im       ║
  ║                    Bereich zwischen 1..32767 liegen.                   ║
  ║                                                                        ║
  ║                    Der Default-Wert nach Aufruf von Ms_Init() beträgt  ║
  ║                        - horizontal:  8 Mickeys pro 8 Pixel            ║
  ║                        - vertikal:   16 Mickeys pro 8 Pixel            ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetThreshold                Datum: 14.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wSpeed					    ║
  ║                                                                        ║
  ║  Beschreibung:     Mit dieser Funktion wird die Bewegungsgeschwindig-  ║
  ║                    keit der Maus festgelegt, ab der die Bewegung des   ║
  ║                    Maus-Cursors auf dem Bildschirm verdoppelt wird.    ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}









/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_TreiberSichern              Datum: 14.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion sichert den Status des Maustreibers  ║
  ║                    in einem modulglobalen Puffer.                      ║
  ║                    Sie können so ein Programm unterbrechen, das die    ║
  ║                    Maus benutzt und ein anderes Programm starten, das  ║
  ║                    ebenfalls die Maus verwendet.                       ║
  ║                                                                        ║
  ║                    s. a. Ms_TreiberRestore()                           ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   OK     wenn Treiber gesichert wurde	    ║
  ║                           ERROR  wenn kein Maustreiber installiert     ║
  ║                                  oder kein Speicher für den Maus-      ║
  ║                                  treiber allokiert werden konnte       ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                    fpbBuffer_m(R/W)                                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
        wGroesse = mausregs.x.bx;                   /* Grösse merken        */
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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_TreiberRestore              Datum: 14.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion lädt den mit Ms_TreiberSichern()     ║
  ║                    gesicherten Status des Maustreibers.                ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   OK     wenn alter Maustreiberstatus geladen  ║
  ║                                  wurde                                 ║
  ║                           ERROR  wenn ein Null-Zeiger auf den Speicher ║
  ║                                  des alten Maustreibers zeigt          ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
            mausregs.x.dx =  FP_OFF (fpbBuffer_m);  /* Interrupt auslösen   */
            int86x(MAUS_INT, &mausregs, &mausregs, &sregs);
            Ut_Ffree(fpbBuffer_m);
            fpbBuffer_m = 0L;
            wReturn = OK;
        }
    }
    return (wReturn);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetSeite                    Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   seite					    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt die Anzeigeseite fest, auf      ║
  ║                    der der Cursor angezeigt wird.                      ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ms_SetSeite(SWORD wSeite)
{
    union REGS mausregs;

    if (!boMausInitialisiert_m)                     /* Test ob Maus bereits */
        Ms_Init();                                  /* initialisiert        */

    if (boMsmTreiberda_m)                           /* wenn Maus im System  */
    {
        mausregs.x.ax = MSM_SET_CRT_PAGE;           /* Funktionsnummer      */
        mausregs.x.bx = wSeite;                     /* gewünschte Seite     */
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
    }
    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_GetSeite                    Datum: 18.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ermittelt die Anzeigeseite, auf      ║
  ║                    der der Mauscursor angezeigt wird.                  ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  == MSM_NO_DRIVER	kein Maustreiber vorhanden  ║
  ║                          => 0            Anzeigeseite                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        int86(MAUS_INT, &mausregs, &mausregs);      /* Interrupt auslösen   */
        wSeite = mausregs.x.bx;                     /* ermittelte Seite     */
    }
    else                                            /* sonst 255 zurück     */
        wSeite = MSM_NO_DRIVER;

    return (wSeite);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ms_SetModus                    Datum: 17.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wModus	     MSM_GRAF_MODUS		    ║
  ║                                         MSM_TEXT_MODUS                 ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt fest, ob die Rückgabewerte      ║
  ║                    der Funktionen, die sich auf Bildschirmpositionen   ║
  ║                    beziehen, im Grafik- oder Textmodus zurückgegeben   ║
  ║                    werden.                                             ║
  ║                    Der Maustreiber arbeitet nur in Grafikkoordina-     ║
  ║                    ten (0..639, 0..239). Wenn mit dieser Funktion      ║
  ║                    der Textmodus selektiert wird, werden die           ║
  ║                    Rückgabewerte in Textkoordinaten (0..79, 0..24)     ║
  ║                    umgerechnet.                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD	der aktuelle Modus			    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boTextMode_m(R/W)                                   ║
  ║                    boMausInitialisiert_m, boMsmTreiberda_m (R)         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
