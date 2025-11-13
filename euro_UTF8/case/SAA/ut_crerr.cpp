// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        Ut_crerr.c                     Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Diese Datei enthält die Routinen zur Installation und Deinstallation  ║
  ║  eines Critical Error Handlers (Interrupt 0x24).                       ║
  ║  Diese Routinen werden von der Funktion Dl_Laden() eingesetzt,         ║
  ║  können jedoch nach Modifizierung auch für andere Programme            ║
  ║  benutzt werden.                                                       ║
  ║                                                                        ║
  ║  Sie sollten diese Routinen erst dann ändern, wenn Sie mit den         ║
  ║  internen Abläufen beim Auftreten eines Critical Errors vertraut sind, ║
  ║  um einen Crash des Systems zu vermeiden.                              ║
  ║                                                                        ║
  ║  Hinweis: Weitere Informationen über Critical Error Handler finden     ║
  ║           Sie u.a. in:                                                 ║
  ║        ■  MS-DOS Programmierhandbuch, DOS 3.1, Seite 1-37 ff.,         ║
  ║                  Markt & Technik, 1986                                 ║
  ║        ■  Ray Duncan, Advanced MS-DOS, Seite 130 ff.,                  ║
  ║                  Microsoft Press, 1986                                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#include <eur_tool.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define ZEI         7                               /* Zeile des Dialoges   */
#define SPA        11                               /* Spalte des Dialoges  */
#define BRE        57                               /* Breite des Dialoges  */
#define HOE        11                               /* Höhe des Dialoges    */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    MODULGLOBALE VARIABLEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
   STATIC VOID (__interrupt __far *fpfnAlterHandler_m) (VOID);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║               MODULGLOBALE FUNKTIONEN - PROTOTYPEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
	  void __interrupt __far fpfnNeuerHandler();

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    MODULGLOBALE VARIABLEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD (*routine) (SWORD, SWORD, SWORD, SWORD);
STATIC CHAR *icon_ausruf[]=
{   "┌───────┐",
    "│  █ █  │",
    "│  █ █  │",
    "│  ▄ ▄  │",
    "└───────┘",
    NULL
};

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT BOOL     boBeepen_g;
IMPORT PSSTR	 pstrRetry_g;
IMPORT PSSTR	 pstrEsc_g;
IMPORT PSSTR	 pstrF1_g;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║               MODULGLOBALE FUNKTIONEN - PROTOTYPEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD LadenFehler(SWORD, SWORD, SWORD, SWORD);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_SetErrorHandler             Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wWelcher	    Routine, die installiert werden ║
  ║                                        soll                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion sichert den aktuellen Vektor auf     ║
  ║                    die Behandlungsroutine, legt aufgrund des           ║
  ║                    Parameters wWelcher die neue Routine fest und       ║
  ║                    trägt diese in die Interruptvektor-Tabelle ein.     ║
  ║                                                                        ║
  ║  Rückgabewert:     keiner                                              ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  fpfnAlterHandler   (W), routine (W)                 ║
  ║                    fpfnNeuerHandler                (R)                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_SetNeuErrorHandler(SWORD wWofuer)
{
fpfnAlterHandler_m = _dos_getvect(0x24);	      /* Interrupt-Vektor    */
						      /* sichern	     */
switch(wWofuer) 				      /* neue Adresse der    */
  {						      /* Routine festlegen   */
  case LADEN:	  routine = LadenFehler;
		  break;
  }

   _dos_setvect(0x24, fpfnNeuerHandler);	      // und in Int-Vektor-
						      // Tabelle eintragen

}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_SetAltErrorHandler          Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion restauriert die ursprüngliche        ║
  ║                    Routine zur Behandlung der kritischen Fehler.       ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  fpfnAlterHandler_m   Zeiger auf Interrupt-Routine   ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_SetAltErrorHandler(VOID)
{
    if (fpfnAlterHandler_m != 0)
        _dos_setvect(0x24, fpfnAlterHandler_m);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    fpfnNeuerHandler               Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        die für eine Interrupt-Routine benötigten           ║
  ║                    Register (der Compiler erkennt nur die Reihenfolge  ║
  ║                    der Argumente, nicht deren Namen)                   ║
  ║                                                                        ║
  ║  Beschreibung:     Von dieser Funktion aus wird die Behandlungs-       ║
  ║                    routine für den kritischen Fehler angesprungen.     ║
  ║                    Die Funktion erhält aus                             ║
  ║                                                                        ║
  ║                    DI  den von MS-DOS zur Verfügung gestellten         ║
  ║                        Fehlercode (niederwertiges Byte)                ║
  ║                    AX  Informationen über Gerätefehler                 ║
  ║                        AX > 0  Diskettenfehler                         ║
  ║                                AX & 0xFF ergibt Nummer des Laufwerks   ║
  ║                                          (1=A, 2=B usw.)               ║
  ║                           < 0  sonstiger Fehler                        ║
  ║                    BP:SI zeigt auf den Kopf des Gerätetreibers         ║
  ║                                                                        ║
  ║  Rückgabewert:     Wert im AX-Register als Information für DOS         ║
  ║                                                                        ║
  ║  Hinweis:          Weitere Informationen über den Modifizierer         ║
  ║                    interrupt finden Sie im Handbuch Ihres Compilers.   ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  routine  Enthält die Adresse der Routine, die       ║
  ║                             bei einem Fehler angesprungen wird         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
   void __interrupt __far fpfnNeuerHandler( unsigned _es, unsigned _ds, unsigned _di,
                                 unsigned _si, unsigned _bp, unsigned _sp,
                                 unsigned _bx, unsigned _dx, unsigned _cx,
                                 unsigned _ax, unsigned _ip, unsigned _cs,
                                 unsigned _flags )
{
    _ax = (*routine) (_di&0x00FF, _ax, _bp, _si);

	_es=_es; _ds=_ds;
	_sp=_sp;
	_bx=_bx; _dx=_dx; _cx=_cx;
	_ip=_ip; _cs=_cs; _flags=_flags;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    LadenFehler                    Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wFehler	    siehe Beschreibung zu	    ║
  ║		       SWORD  ax	    fpfnNeuerHandler		    ║
  ║		       SWORD  bp					    ║
  ║		       SWORD  si					    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Routine wird von fpfnNeuerHandler ange-       ║
  ║                    sprungen, wenn der kritische Fehler während         ║
  ║                    der Dialogbox Dl_Laden() auftritt.                  ║
  ║                    Wegen des Kontexts kann es sich hierbei nur         ║
  ║                    um einen Lesefehler bei einem Laufwerk handeln.     ║
  ║                    Aus diesem Grunde werden nicht alle der Funktion    ║
  ║                    übergebenen Argumente ausgewertet.                  ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  0 = ignorieren, bzw. Abbrechen		    ║
  ║                          1 = mißglückte Operation noch einmal          ║
  ║                              versuchen                                 ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boBeepen_g, pstrRetry_g, pstrEsc_g   (W)            ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD LadenFehler(SWORD wFehler, SWORD ax, SWORD bp, SWORD si)
{
    SREGISTER	 i;
    PWKB        pWkbDialog;
    EVENT       Event;
    PEVENT      pEvent;
    PPSTR       ppstrTmp;

wFehler=wFehler; bp=bp; si=si;

    if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE)) /* Fenster einrichten   */
        == NULL)
    {
        i_Beep();                                   /* Warnton ausgeben     */
        return (0);                                 /* evtl. Abbruch        */
    }

    i_Dl_Init(SPA, ZEI, BRE, HOE,                   /* Fenster initialis.   */
              "Systemfehler",
              pWkbDialog,
              SF_RETRYCANCEL,
              DLG_KRITISCH);

    for (ppstrTmp=icon_ausruf, i=2;                 /* Icon ausgeben        */
         *ppstrTmp != NULL;
         i++)
        Wi_Ss(3,i,*ppstrTmp++);

    Wi_SetCursorPos(14,3);                          /* Text ausgeben        */
    Wi_Printf("Lesen von Laufwerk %c: ist nicht möglich.", 'A'+ ax&0x00FF);
    Wi_Ss(14,4,"Bitte wählen Sie die gewünschte Aktion.");

    pEvent = &Event;                                /* Var. initialisieren  */

    ax &= 0xFF00;                                   /* AL-Register löschen  */
    for(;;)                                         /* Eingaberoutine       */
    {
        Ut_Event(pEvent);
        switch(pEvent->wArt)
        {
           case EVENT_TASTE:
              switch(pEvent->wTaste)
              {
               case T_ESC:
                    Wi_Entfernen(pWkbDialog);
                    return(ax+0);

               case T_RETURN:
                    Wi_Entfernen(pWkbDialog);
                    return(ax+1);

               default:
                    if (boBeepen_g)
                        i_Beep();
              } /* end of switch(pEvent->wTaste) */
              break;

           case EVENT_L_PRESS:
              if (pEvent->wZeile == 16)
              {
                 if (pEvent->wSpalte > SPA+2 &&
		     pEvent->wSpalte < SPA+3+(SWORD)strlen(pstrRetry_g))
                 {
                    Wi_Entfernen(pWkbDialog);
                    return(ax+1);
                 }
                 else if (pEvent->wSpalte > SPA+20 &&
		       pEvent->wSpalte < SPA+17+(SWORD)strlen(pstrEsc_g))
                 {
                    Wi_Entfernen(pWkbDialog);
                    return(ax+0);
                }
             }
              else if (boBeepen_g)
                 i_Beep();
             break;
        } /* end of switch(pEvent->wArt) */
    } /* end of for */

}
