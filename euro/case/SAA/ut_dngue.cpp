// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_DateinameGueltig()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_DateinameGueltig()    ... berprft ob bergebener Dateiname gltig ist. บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
BOOL Ut_DateinameGueltig(pstrDateiname);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR pstrDateiname    Dateiname, der berprft werden soll.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion berprft, ob der bergebene Dateiname gltig oder nicht gltig
ist.

Hinweis: Der bergebene String darf neben dem Datei- auch den Pfadnamen ent-
halten. Wenn der Dateiname den DOS-Konventionen entspricht, gibt die Funktion
Funktion den Wert der Konstanten DN_GUELTIG zurck, sonst DN_UNGUELTIG.

Ein Laufwerksname alleine zhlt nicht als gltiger Dateiname.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
BOOL  DN_GUELTIG   == gltiger Dateiname
      DN_UNGUELTIG == ungltiger Dateiname


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
keine

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>                                  /* Prototyp dieser Fkt. */
#endif

#include <string.h>                                 /* fr str..-Funktionen */
#include <stdio.h>                                  /* fr NULL             */
#include <ctype.h>                                  /* fr isalpha()        */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define UNGUELTIGE_ZEICHEN "\\/[]{}:|<>+=,;.\"?*"

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	BOOL i_ErlaubtesZeichen (CHAR);
STATIC	SWORD i_TestN(PSSTR, BOOL);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
BOOL Ut_DateinameGueltig(PSSTR pstrDateiname)
{
    SREGISTER i;				     /* Schleifenzhler      */
    PSSTR pstrTmp;				     /* lokaler Zeiger	     */
    PSSTR pstrAnfang;
    PSSTR pstrEnde;
    BOOL boNameFertig = FALSCH;                     /* mit Dateinamen fert. */
    SWORD wResult;

    pstrAnfang  = pstrDateiname;                    /* Zeiger auf Anfang    */
    for (i=0; *pstrAnfang==' '; i++)                /* fhrende Leerzeichen */
        pstrAnfang++;                               /* ausschneiden         */

    if (i > 0)                                      /* wenn Leerzeichen da, */
    {
        strcpy(pstrDateiname, pstrAnfang);          /*     verkrzten Namen */
        pstrAnfang = pstrDateiname;                 /*     kopieren         */
    }

    pstrTmp = pstrDateiname+strlen(pstrDateiname);  /* Zeiger auf Ende des  */
    pstrEnde = pstrTmp;                             /* Stringende merken    */                                                /* Strings              */

    if( *(pstrTmp-1) == '\\' ||                     /* wenn dort Backslash  */
        *(pstrTmp-1) == ':' )                       /* oder Doppelpunkt     */
        return(DN_UNGUELTIG);                       /* Name ungltig        */


    if (*(pstrAnfang+1) == ':')                     /* wenn Doppelpunkt     */
        if (!isalpha(*pstrAnfang))                  /* muแ Zeichen davor    */
            return(DN_UNGUELTIG);                   /* Buchstabe sein       */
        else
            pstrAnfang += 2;                        /* LW-Kennung berspri. */

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
        for (--pstrTmp;                             /* nchstes Teil holen  */
             *pstrTmp != '\0' && pstrTmp >= pstrAnfang;
             --pstrTmp)
            ;

        wResult = i_TestN(pstrTmp+1, boNameFertig); /* String berprfen    */
        boNameFertig = JA;                          /* Dateiname erledigt   */

    } while (pstrTmp > pstrAnfang && wResult == DN_GUELTIG);


    for (; pstrAnfang < pstrEnde; pstrAnfang++)     /* Dateinamen wieder in */
        if (*pstrAnfang == '\0')                    /* Form bringen         */
            *pstrAnfang = '\\';

    return(wResult);

} /* end Ut_DateinameGueltig() */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_TestN()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ PSSTR pstrName	Name, der berprft werden soll 		     บ
  บ BOOL boNamefertig  Dateiname oder Verzeichnisname			    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_TestN(PSSTR pstrName, BOOL boNameFertig)

{
    SREGISTER i;
    BOOL boPktGef;                                  /* Punkt gefunden       */
    SWORD wMaxI = 8;				     /* Grenzwert fr i      */

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
                                i = 0;              /* Zhler init.         */
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_ErlaubtesZeichen()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ CHAR cZeichen      Zeichen, das berprft werden soll.		    บ
  บ									    บ
  บ Beschreibung:							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion berprft, ob das bergebene Zeichen in einem	    บ
  บ DOS-Dateinamen vorkommen darf.					    บ
  บ									    บ
  บ Die Zeichen, die das Ergebnis FALSCH liefern, sind in der Konstante     บ
  บ UNGUELTIGE_ZEICHEN definiert.					    บ
  บ									    บ
  บ Rckgabewert:							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ BOOL   WAHR   gltiges Zeichen					    บ
  บ	   FALSCH ungltiges Zeichen					    บ
  บ									    บ
  บ Benutzte globale Varaiblen (R/W)					    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ keine								    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
