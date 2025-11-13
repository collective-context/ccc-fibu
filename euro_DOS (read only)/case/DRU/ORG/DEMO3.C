 #include <stdio.h>
 #include <conio.h>
 #include <ctype.h>

 #include "prttools.c"

 double SpacePerEntry = 1.575,  /* Pro Eintrag 1.575 Zoll Platz */
        SpacePerLine  = 3.35,   /* Pro Zeile 3.35 Zoll Platz    */
        CharsPerInch,           /* Zeichen pro inch             */
        InchesPerChar;          /* Inch pro Zeichen             */
 char   PrinterDriver[9],       /* Name des Druckertreibers     */
        DiscNr[5],              /* Nummer der Diskette          */
        DiscContent[10][31],    /* Die Einzelnen Eintr„ge       */
        Answer;                 /* J/N-Antwort                  */
 int    Entries = 0,            /* Anzahl vorhandener Eintr„ge  */
        LettersPerEntry,        /* Buchtsaben pro Zeile         */
        Count,                  /* Schleifenz„hler              */
        Count2;                 /* Schleifenz„hler              */


 void EmptyLine( void )
 {
      ptPrintLeftNow( "º", 0 );
      ptPrintLeftNow( "º",  ( SpacePerLine - InchesPerChar ) );

      ptSkipExactStandard( 4 );

 }

 void main( void )
 {

    clrscr();  /* Muá bei MS-Compilern entfallen */
    printf("\n\r\n\r Name der Druckertreiberdatei (max. 8 Buchstaben): ");
    scanf( "%s", PrinterDriver );

    if ( ptInitPrinter( PrinterDriver ) < 0 )
    {
         printf("\n\r\n\r ** Druckertreiber nicht gefunden/Drucker nicht ansprechbar ! **\n\r\n\r");
         exit(0);
    }
    ptDefineUnit( PT_INCH );
    ptChooseFont( ptGetInfo( PT_SMALLESTFONT ) );
    LettersPerEntry = (WORD) ( SpacePerEntry * ptGetInfo( PT_CHARSPERINCH ) );
    InchesPerChar   = (double) 1 / ptGetInfo( PT_CHARSPERINCH);
    CharsPerInch    = (double) ptGetInfo( PT_CHARSPERINCH );

    do
    {
         DiscNr[0] = 0;
         printf("\n\r\n\r Nummer der aktuellen Diskette (max. 4 Buchstaben): " );
         scanf( "%s", DiscNr );

         while (  Entries < 10 )
         {
               do
               {
                    DiscContent[Entries][0] = 0;
                    printf("\n\r Der %d. Eintrag (max. %d Buchstaben): ", Entries + 1,
                               LettersPerEntry );
                    scanf( "%s", DiscContent[Entries] );
               }
               while ( strlen( DiscContent[Entries] ) >
                           LettersPerEntry );
               Entries++;
         }

         ptPrintLeftNow( "É", 0 );
         for( Count=1; Count <= (WORD) ( ( SpacePerLine -
                  2 * InchesPerChar ) * CharsPerInch); Count++ )
              ptPrintNowChar( 'Í' );
         ptPrintLeftNow( "»", ( SpacePerLine - InchesPerChar ) );
         ptSkipExactStandard( 4 );

         EmptyLine();

         ptPrintLeftNow( "Discnr.: ", InchesPerChar * 2 );
         ptPrintNow( DiscNr );
         ptSkipExactStandard( 4 );

         ptPrintLeftNow( "Ì", 0 );
         for( Count=1; Count <= (WORD) ( ( SpacePerLine -
              2 * InchesPerChar ) * CharsPerInch); Count++ )
              ptPrintNowChar( 'Í' );
         ptPrintLeftNow( "¹", ( SpacePerLine - InchesPerChar ) );
         ptSkipExactStandard( 4 );

         Count = 0;

         while ( Count < 5 )
         {
               EmptyLine();

               if ( strlen( DiscContent[Count] ) > 0 )
                        ptPrintAutoNow( DiscContent[Count],
                        InchesPerChar*2, 5, PT_LEFT );
               if ( strlen( DiscContent[5 + Count] ) > 0 )
                        ptPrintAutoNow( DiscContent[5 + Count], 1.68, 5,
                        PT_LEFT );
               ptSkipExactStandard( 4 );

               Count++;
         }

         ptPrintLeftNow( "È", 0 );
         for( Count=1; Count <= (WORD) ( ( SpacePerLine -
                  2 * InchesPerChar ) * CharsPerInch); Count++ )
              ptPrintNowChar( 'Í' );
         ptPrintNowChar( '¼' );

         ptSkipExactStandard( 14 );

         printf("\n\r\n\r                           Ein weiteres Etikett (J/N) ? ");
         do
         {
             Answer = toupper( getch() );
         }
         while ( Answer != 'J' && Answer != 'N' );
         printf( "%c", Answer );
         Entries = 0;
    }
    while ( Answer == 'J' );

    ptClosePrinter();
 }
