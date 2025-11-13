#include <stdio.h>
#include <conio.h>

#include "prttools.c"
#include "pcxtools.c"

CHAR Graph[12],        /* Name des Graphikdruckertreibers */
     Text[12];         /* Name des Textdruckertreibers    */

VOID ChooseDriver( VOID )
{
     CHAR zeichen;

     clrscr();  /* Muß bei MS-Compilern entfallen */

     printf( "\n\r                        PT-Druckertoolbox Demoprogramm 1" );
     printf( "\n\r                        --------------------------------" );
     printf( "\n\r\n\r\n\r Bitte wählen Sie den Druckertreiber: ");
     printf( "\n\r\n\r 1) EPSON-9-Nadler und kompatible, z.B. LX-800, FX, ...");
     printf( "\n\r 2) EPSON-24-Nadler und kompatible, z.B. LQ-Reihe, ...");
     printf( "\n\r 3) HP-Laserjet und kompatible" );
     printf( "\n\r 4) HP-Deskjet und kompatible" );

     printf( "\n\r\n\r -->");

     do zeichen = getch();
     while ( zeichen < '1' || zeichen > '4' );

     printf( "%c", zeichen );

     switch ( zeichen )
     {
            case '1' : strcpy( Graph, "ESC9x240" );
                       strcpy( Text,  "EPSON."      );
                       break;
            case '2' : strcpy( Graph, "ESC24x12" );
                       strcpy( Text,  "EPSON24."    );
                       break;
            case '3' : strcpy( Graph, "HPLASER"  );
                       strcpy( Text,  "HPLASER."    );
                       break;
            case '4' : strcpy( Graph, "DESKJET"  );
                       strcpy( Text,  "DESKJET."    );
     }
}

VOID main( VOID )
{
   SWORD a;
   CHAR  Feld[50],
         Feld2[10];

   /* User hat den passenden Druckertreiber auszuwählen ! */
   ChooseDriver();

   /* Druckertreiber wird aktiviert, tritt ein Fehler auf gilt allgemein: */
   /* Rückgabewert < 0                                                    */
   if ( ptInitPrinter( Text ) < 0 )
   {
      printf("Fehler beim Initialisieren des Druckers !");
      exit ( 1 );
   }

   /* Alle Maßangaben liegen in CM vor */
   ptDefineUnit( PT_CM );

   /* Breite einer Zeile: 20 cm */
   ptDefineHorLength( 20 );

   /* Für die Überschrift: Attribut "Unterstreichen" aktivieren */
   ptSwitchFontCharacter( PT_UNDERLINE, PT_ON );

   /* Ausgabe einer zentriert gedruckten Überschrift */
   ptPrintAutoBuffer( 2, 0, "PT-Druckertoolbox Demoausdruck", 0,
                      ptGetDoubleInfo( PT_HORLENGTH ), PT_CENTER );

   /* Für weiteren Text: Attribut "Unterstreichen" abschalten */
   ptSwitchFontCharacter( PT_UNDERLINE, PT_OFF );

   /* Vormerken: Ausgabe eines Logo-Bildes (PCX-Format) */
   ptPrintImageBuffer( 3, 0, "PT.PCX", 3, 3, 9 );


   /* 1. Zeitungsspalte: ab Zeile 14, 7 cm breit, 3 cm Platz am linken Rand */
   ptPrintAutoBuffer( 14, 0, "Dies kann eine Zeitungsspalte sein. Der Phantasie sind keine Grenzen gesetzt.", 3, 10,
                      PT_JUSTIFY );

   /* 2. Zeitungsspalte: ab Zeile 14, 7 cm breit, 11 cm Platz am linken Rand */
   ptPrintAutoBuffer( 14, 0, "Dies ist die letzte Zeitungsspalte. Auch hier gilt: We know no limits !", 11, 18,
                      PT_JUSTIFY );

   /* Für die Überschrift: Attribut "Fettdruck" aktivieren */
   ptSwitchFontCharacter( PT_BOLD, PT_ON );

   /* Zentrierte Ausgabe einer fettgedruckten Überschrift */
   ptPrintAutoBuffer( 20, 0, "Folgende Schriftarten werden geboten:", 0,
                      ptGetDoubleInfo( PT_HORLENGTH ), PT_CENTER );

   /* Für die Überschrift: Attribut "Fettdruck" abschalten */
   ptSwitchFontCharacter( PT_BOLD, PT_OFF );

   /* Alle verfügbaren Fonts (im Druckertreiber angegeben) der Reihe nach ausgeben */
   for (a=0; a<ptGetInfo( PT_NUMBEROFFONTS ); a++)
   {
       /* Font wählen */
       ptChooseFont( a );

       strcpy( Feld, "Dies ist Font Nr. " );
       itoa( a, Feld2, 10 );
       strcat( Feld, Feld2 );

       /* Text zentriert (Blattmitte) ausgaben */
       ptPrintAutoBuffer( 25+2*a, 0, Feld, 1, ptGetDoubleInfo( PT_HORLENGTH ), PT_CENTER );
   }

   /* Alle vorgemerkten Graphiken und Texte ausgeben */
   ptSendPCXBuffer( Graph );

   /* Blatt auswerfen */
   ptNextPage();

   /* Speicher, der von "..Buffer"-Befehlen belegt wurde, wieder freigeben */
   ptEmptyBuffer();

   /* Druckertoolbox schließen */
   ptClosePrinter();

}