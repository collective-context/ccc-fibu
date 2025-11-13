// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_DateinameGueltig()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_DateinameGueltig()    ... überprüft ob übergebener Dateiname gültig ist. ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
BOOL Ut_DateinameGueltig(pstrDateiname);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR pstrDateiname    Dateiname, der überprüft werden soll.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion überprüft, ob der übergebene Dateiname gültig oder nicht gültig
ist.

Hinweis: Der übergebene String darf neben dem Datei- auch den Pfadnamen ent-
halten. Wenn der Dateiname den DOS-Konventionen entspricht, gibt die Funktion
Funktion den Wert der Konstanten DN_GUELTIG zurück, sonst DN_UNGUELTIG.

Ein Laufwerksname alleine zählt nicht als gültiger Dateiname.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
BOOL  DN_GUELTIG   == gültiger Dateiname
      DN_UNGUELTIG == ungültiger Dateiname


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
keine

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>                                  /* Prototyp dieser Fkt. */
#endif

#include <string.h>                                 /* für str..-Funktionen */
#include <stdio.h>                                  /* für NULL             */
#include <ctype.h>                                  /* für isalpha()        */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define UNGUELTIGE_ZEICHEN "\\/[]{}:|<>+=,;.\"?*"

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	BOOL i_ErlaubtesZeichen (CHAR);
STATIC	SWORD i_TestN(PSSTR, BOOL);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
BOOL Ut_DateinameGueltig(PSSTR pstrDateiname)
{
    SREGISTER i;				     /* Schleifenzähler      */
    PSSTR pstrTmp;				     /* lokaler Zeiger	     */
    PSSTR pstrAnfang;
    PSSTR pstrEnde;
    BOOL boNameFertig = FALSCH;                     /* mit Dateinamen fert. */
    SWORD wResult;

    pstrAnfang  = pstrDateiname;                    /* Zeiger auf Anfang    */
    for (i=0; *pstrAnfang==' '; i++)                /* führende Leerzeichen */
        pstrAnfang++;                               /* ausschneiden         */

    if (i > 0)                                      /* wenn Leerzeichen da, */
    {
        strcpy(pstrDateiname, pstrAnfang);          /*     verkürzten Namen */
        pstrAnfang = pstrDateiname;                 /*     kopieren         */
    }

    pstrTmp = pstrDateiname+strlen(pstrDateiname);  /* Zeiger auf Ende des  */
    pstrEnde = pstrTmp;                             /* Stringende merken    */                                                /* Strings              */

    if( *(pstrTmp-1) == '\\' ||                     /* wenn dort Backslash  */
        *(pstrTmp-1) == ':' )                       /* oder Doppelpunkt     */
        return(DN_UNGUELTIG);                       /* Name ungültig        */


    if (*(pstrAnfang+1) == ':')                     /* wenn Doppelpunkt     */
        if (!isalpha(*pstrAnfang))                  /* muß Zeichen davor    */
            return(DN_UNGUELTIG);                   /* Buchstabe sein       */
        else
            pstrAnfang += 2;                        /* LW-Kennung überspri. */

    while (pstrTmp >= pstrAnfang)                   /* Dateinamen und evtl. */
    {                                               /* Verzeichnisnamen     */
        if (*pstrTmp == '\\')                       /* isolieren            */
            *pstrTmp = '\0';
        pstrTmp--;
    }

    pstrTmp = pstrEnde;                             /* Zeiger hinter letzt. */
                                                    /* Zeichen des Strings  */

    do
    {
        for (--pstrTmp;                             /* nächstes Teil holen  */
             *pstrTmp != '\0' && pstrTmp >= pstrAnfang;
             --pstrTmp)
            ;

        wResult = i_TestN(pstrTmp+1, boNameFertig); /* String überprüfen    */
        boNameFertig = JA;                          /* Dateiname erledigt   */

    } while (pstrTmp > pstrAnfang && wResult == DN_GUELTIG);


    for (; pstrAnfang < pstrEnde; pstrAnfang++)     /* Dateinamen wieder in */
        if (*pstrAnfang == '\0')                    /* Form bringen         */
            *pstrAnfang = '\\';

    return(wResult);

} /* end Ut_DateinameGueltig() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_TestN()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ PSSTR pstrName	Name, der überprüft werden soll 		     ║
  ║ BOOL boNamefertig  Dateiname oder Verzeichnisname			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_TestN(PSSTR pstrName, BOOL boNameFertig)

{
    SREGISTER i;
    BOOL boPktGef;                                  /* Punkt gefunden       */
    SWORD wMaxI = 8;				     /* Grenzwert für i      */

    if (pstrName == NULL)
        return(DN_UNGUELTIG);

    for (i=0, boPktGef=FALSCH;
         i<=wMaxI && *pstrName != '\0';
         pstrName++)
    {
        if (i_ErlaubtesZeichen(*pstrName))
            i++;                                    /* weitermachen         */
        else
            switch(*pstrName)
            {
                case '.':   if (boPktGef)           /* zweiter Punkt        */
                                i=wMaxI+1;          /* for-Schleife beenden */
                            else
                            {
                                boPktGef = WAHR;    /* Flag setzen          */
                                i = 0;              /* Zähler init.         */
                                wMaxI = 3;          /* MaxZeichen Extension */
                            }
                            break;

                case '*':
                case '?':   if (boNameFertig)
                                i=wMaxI+1;          /* for-Schleife beenden */
                            else
                                i++;                /* weitermachen         */
                            break;

                default:    i=wMaxI+1;              /* for-Schleife beenden */
            } /* end of switch */
    } /* end of for */

    if (i>wMaxI)
        return(DN_UNGUELTIG);
    else
	return(DN_GUELTIG);

} /* end i_TestN() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ErlaubtesZeichen()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ CHAR cZeichen      Zeichen, das überprüft werden soll.		    ║
  ║									    ║
  ║ Beschreibung:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion überprüft, ob das übergebene Zeichen in einem	    ║
  ║ DOS-Dateinamen vorkommen darf.					    ║
  ║									    ║
  ║ Die Zeichen, die das Ergebnis FALSCH liefern, sind in der Konstante     ║
  ║ UNGUELTIGE_ZEICHEN definiert.					    ║
  ║									    ║
  ║ Rückgabewert:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ BOOL   WAHR   gültiges Zeichen					    ║
  ║	   FALSCH ungültiges Zeichen					    ║
  ║									    ║
  ║ Benutzte globale Varaiblen (R/W)					    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ keine								    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
BOOL i_ErlaubtesZeichen (CHAR cZeichen)
{
    PCHAR pcTmp;

    if (cZeichen <= ' ')                            /* auf Leerzeichen      */
        return (FALSCH);                            /* testen               */

     for (pcTmp = UNGUELTIGE_ZEICHEN; *pcTmp != '\0'; pcTmp++)
        if (*pcTmp == cZeichen)
            return(FALSCH);

    return(WAHR);

} /* end i_ErlaubtesZeichen() */
