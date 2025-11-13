// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Mn_Init ()                     Datum: 14.09.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT	SWORD  wCSInd_g;		  /* Nummer der Farbpaletten	      */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ MNU_Modul APP            DAS APPLIKATIONS-MENÜ                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC  ITEM itMenu[][20] =
{
  {
    { 100, "#F-Tasten",
           "Mit [\x19]/[\x18] die Funktion wählen und mit [\x11\xD9] starten." },
    { 101, "#Speichern und Ende   [ESC]",
           "1x [ESC]= Speichern --> 2x [ESC]= Programmende." },
    { 102, "#Matchcode             [F2]",
           "Damit aktivieren Sie die Suchfunktion."  },
    { 103, "#Löschen               [F3]",
           "Löschen des aktuellen Datensatzes." },
    MNU_TRENNER,
    { 104, "Memo-#Allgemein        [F4]",
           "Speichert allgemeine Notizen zum Datensatz."},
    { 105, "M#emo-Speziell [Umsch]+[F4]",
           "Speichert applikationsabhängige Notizen zum Datensatz."},
    { 106, "#Nachrichten    [Strg]+[F4]",
           "Schwarzes Brett mit Nachrichten an »Alle«."},
    { 107, "#Privates Fach   [Alt]+[F4]",
           "Arbeitsplatzabhängiges, privates Notizfach."},
    MNU_TRENNER,
    { 108, "#Kopie                 [F5]",
           "Damit können Sie einen Datensatz kopieren." },
    MNU_ENDE
  },
  {
    { 200, "#Optionen",
           "Verändert und steuert die Programmumgebung." },
    { 207, "#Felder vorbelegen",
	   "Bestimmte Eingabefelder werden automatisch vorbelegt." },
    { 208, "#Nächsten lesen",
	   "An speziellen Stellen den nächsten Eintrag automatisch lesen." },
    MNU_TRENNER,
    { 201, "#Maussteuerung",
           "Mausempfindlichkeit einstellen."},
    { 202 ,"#Warnton",
           "Ermöglicht das Einstellen des Warntons." },
    MNU_TRENNER,
    { 203, "#Rahmen doppelt ein",
           "Ein- oder ausschalten der doppelten Rahmen im Menü." },
    { 204, "#Schatten aus",
           "Den Schatten der Menüfenster ein- oder ausschalten."},
    { 205, "#Betriebssystem",
           "Vorübergehender Sprung ins Betriebssystem." },
    { 206, "#Testmodus ein",
           "Aktiviert oder deaktiviert den Testmodus." },
    MNU_ENDE
  },
  {
    { 31000, "F1=#Hilfe", "Eröffnet Zugang zum Hilfesystem."},
    { 31001, "Informationen Kurz#tasten [F9]",
             "Zeigt Hilfsbildschirm mit den wichtigsten Tasten." },
    { 31002, "Hilfe#index...      [Alt]+[F1]",
             "Zeigt Auswahlliste für die Hilfeunterstützung." },
    MNU_ENDE
  },
  { MNU_ENDE }
};

PITEM  pitMenu=itMenu[0];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ MNU_Modul APPMENU        MENÜ-ZUORDNUNG                                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Mn_Init(PPITEM ppitMenu)              /* MGLOBAL PITEM apAppMenu_m[10];*/
{
SREGISTER i;
CHAR strError[TB_MAX];
ppitMenu=&pitMenu;

sprintf(strError, "Mn_Init()");

/*
for(i=0, wEnd=NEIN; wEnd == NEIN; i++)
   {
   wEnd=(app[i].wOption==NULL) ? JA : NEIN;
   Ut_Calloc(*(apAppMenu+0), 20, ITEM);
   Ut_Calloc(apAppMenu[0][i].pstrText,
     strlen(app[i].pstrText)+1, CHAR);
   Ut_Calloc(apAppMenu[0][i].pstrHilfstext,
     strlen(app[i].pstrHilfstext)+1, CHAR);

   apAppMenu[0][i].wOption=app[i].wOption;
   strcpy(apAppMenu[0][i].pstrText, app[i].pstrText);
   strcpy(apAppMenu[0][i].pstrHilfstext, app[i].pstrHilfstext);
   }

apAppMenu[3]=NULL;
*/

Mn_SetSchatten(1, EIN);
Mn_SetPalette(1, wCSInd_g);

for(i=0; itMenu[i][0].wOption; i++)                  /* Hauptmenü zusammen-  */
  Mn_Einrichten(1, itMenu[i] );                      /* stellen              */

return;
}
