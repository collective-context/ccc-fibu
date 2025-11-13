/*---------------------------------------------------------------------------

        Druckertoolbox für Textausgaben - direkt und speichergesteuert

                          (c) 1992/93 by Markus Mück

-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prtools.h"

CHAR     PrinterName[8];                  /* Name des Druckers = Filename                */
CHAR     Port[8] = { "" };                /* Druckerport, z.B. Lpt1                      */
WORD     VerticalMoveUnit = 0;            /* Kleinstmögliche vertikale Bewegungseinheit  */
WORD     VerUnitsPerLine  = 0;            /* Vertikale Bewegungseinheit für 1 Zeile      */
BYTE     VerMoveKind   = 0;               /* Art des Bewegungscodes: PT_HP oder PT_ESC   */
CHAR     *PreVerCode   = NULL;            /* Code vor vertikaler Bewegung                */
CHAR     *AftVerCode   = NULL;            /* Code nach vertikaler Bewegung               */
CHAR     *Reset        = NULL;            /* Zeiger auf String für Druckerreset          */
CHAR     *BoldOn       = NULL;            /* Zeiger auf String für Fettdruck ein         */
CHAR     *BoldOff      = NULL;            /* Zeiger auf String für Fettdruck aus         */
CHAR     *UnderlineOn  = NULL;            /* Zeiger auf String für Unterstreichen an     */
CHAR     *UnderlineOff = NULL;            /* Zeiger auf String für Unterstreichen aus    */
CHAR     *ItalicOn     = NULL;            /* Zeiger auf String für Kursiv ein            */
CHAR     *ItalicOff    = NULL;            /* Zeiger auf String für Kursiv aus            */
CHAR     BackSpace     = 0;               /* Code für "1 Zeichen zurück"                 */
CHAR     NewPage       = 0;               /* Code für "neue Seite"                       */
FILE     *Datei;                          /* Dateihandle                                 */
WORD     AktFont       = 0;               /* Nummer des aktuellen Fonts                  */
WORD     SetFont       = 0;               /* Font, der zuletzt an Drucker geschickt ward */
WORD     Fonts         = 0;               /* Anzahl verfügbarer Fonts                    */
BYTE     Chars_per_inch= 0;               /* Zeichen pro inch, wird beim Laden angegl.   */
DOUBLE   SpaltenBreiteCM=16.9;            /* Spaltenbreite in cm                         */
DOUBLE   LeftAdjustment= 0;               /* Linker Rand um .. INCH nach rechts versetzt */
FILE     *DruckerHandle         = NULL;   /* Druckerhandle                               */
WORD     AktLine                = 0;      /* Nummer der aktuellen Zeile                  */
WORD     AktMeasureUnit         = PT_CM;  /* Aktuelle Maßeinheit                         */
WORD     AnzahlColumns          = 0;      /* Anzahl vorhandener Spalten                  */
WORD     AnzahlBufferEintrage   = 0;      /* Anzahl vorhandener Buffereinträge           */
WORD     ActiveColumn           = 0;      /* Nummer der aktiven Spalte                   */
WORD     AnzahlTraps            = 0;      /* Anzahl vorhandener TRAPS                    */
WORD     ActiveLine             = 0;      /* Nummer der aktuellen Zeile                  */
WORD     VerUnitsMoved          = 0;      /* Anzahl Punkte nach unten                    */
WORD     PagesPrinted           = 0;      /* Anzahl gedruckter Seiten                    */
WORD     LinesPerPage           = 50;     /* 50 Zeilen pro Seite                         */
BYTE     SerialPort             = 0;      /* Ausgabeport: 1=Com1, 2=Com2, ...            */
BYTE     ParallelPort           = 0;      /* Ausgabeport: 1=lpt1, 2=lpt2, ...            */
BOOL     ActiveBold             = PT_FALSE;/*Standard: Fettdruck aus                     */
BOOL     ActiveUnderline        = PT_FALSE;/*Standard: Unterstreichen aus                */
BOOL     ActiveItalic           = PT_FALSE;/*Standard: Kursiv aus                        */
BOOL     ActiveSpecialChars     = PT_TRUE;/* "_^" für Underline/Bold ein                 */
COLUMNS  *ColumnZeiger          = NULL;   /* Zeiger auf Spalten-Daten                    */
TRAP     *TrapZeiger            = NULL;   /* Zeiger auf TRAP-Daten                       */
BUFFER   *BufferZeiger          = NULL;   /* Zeiger auf BUFFER-Daten                     */
FONTS    *FontZeiger            = NULL;   /* Zeiger auf FONT-Daten                       */

/***************************************************************************
 * Es folgen: Die Prototypen                                               *
 **************************************************************************/

 VOID   ptFunc                        ( VOID );
 SWORD  ptfputc                       ( CHAR Zeichen );
 SWORD  ptDefineLeftMargin            ( DOUBLE LeftMargin );
 DOUBLE ptGetDoubleInfo               ( BYTE Info );
 WORD   ptGetInfo                     ( BYTE Info );
 CHAR   *ptGetStrInfo                 ( BYTE Info );
 SWORD  ptSetSpecialChar              ( WORD Switch );
 SWORD  ptPrintNowChar                ( CHAR Character );
 SWORD  ptSendSequenz                 ( CHAR *Start );
 SWORD  ptResetPrinter                ( VOID );
 SWORD  ptSwitchOffFontCharacteristics( VOID );
 SWORD  ptSwitchOnFontCharacteristics ( VOID );
 SWORD  ptSwitchFontCharacter         ( WORD Character, WORD OnOff );
 SWORD  ptCheckForTrap                ( WORD Line );
 SWORD  ptDontUseTrap                 ( WORD Line );
 SWORD  ptUseTrap                     ( WORD Line );
 SWORD  ptDefineTrap                  ( WORD Line, VOID (*ptFunc)() );
 SWORD  ptDeleteTrap                  ( WORD Line );
 SWORD  ptCallTraps                   ( VOID );
 SWORD  ptMoveVerUnits                ( WORD Units );
 SWORD  ptSkip                        ( WORD Lines );
 SWORD  ptSkipExact                   ( WORD Units );
 SWORD  ptSkipExactStandard           ( WORD Units );
 SWORD  ptAnzahlWorte                 ( CHAR* Text, DOUBLE Left, DOUBLE Right );
 SWORD  ptMoveHeadToHorPosition       ( DOUBLE Pos );
 SWORD  ptPrintLeft                   ( CHAR* Text, DOUBLE Left, DOUBLE Right );
 SWORD  ptPrintCenterOrRight          ( BYTE Ausrichtung, CHAR* Text, DOUBLE Left, DOUBLE Right );
 SWORD  ptPrintJustify                ( CHAR* Text, DOUBLE Left, DOUBLE Right );
 DOUBLE ptCm2Inch                     ( DOUBLE Cm );
 DOUBLE ptInch2Cm                     ( DOUBLE Inch );
 SWORD  ptPrintAutoNow                ( CHAR *Text, DOUBLE Left, DOUBLE Right, DOUBLE Method );
 SWORD  ptMoveBack                    ( WORD Chars );
 SWORD  ptPrintNow                    ( CHAR *Text );
 SWORD  ptDefineUnit                  ( WORD Unit );
 SWORD  ptCheckForColumn              ( WORD Num );
 SWORD  ptDefineColumn                ( WORD Num, DOUBLE Left, DOUBLE Right );
 SWORD  ptDeleteColumn                ( WORD Num );
 DOUBLE ptGetColumn                   ( WORD Num, WORD Method );
 SWORD  ptPrintBetweenColumnsNow      ( CHAR *Text, WORD ColLeft, WORD ColRight, DOUBLE Method );
 SWORD  ptPrintInColumnNow            ( CHAR *Text, WORD Column, WORD Method );
 SWORD  ptFlush                       ( VOID );
 SWORD  ptMoveHorPos                  ( DOUBLE Pos );
 SWORD  ptPrintRightNow               ( CHAR *Text, DOUBLE Pos );
 SWORD  ptPrintLeftNow                ( CHAR *Text, DOUBLE Pos );
 SWORD  ptNextPage                    ( VOID );
 DOUBLE ptStringLen                   ( CHAR *Text  );
 CHAR*  ptSplitText                   ( CHAR *ZielText, CHAR *OrigText, DOUBLE Left, DOUBLE Right  );
 SWORD  ptChooseFont                  ( WORD FontNr );
 SWORD  ptSetSequenz                  ( BOOL Bold, BOOL Underline, BOOL Italic );
 SWORD  ptFontSize                    ( WORD FontNr );
 SWORD  ptChangePort                  ( CHAR *PortName );
 SWORD  ptSetLinesPerPage             ( WORD AnzLines );
 SWORD  ptDefineHorLength             ( DOUBLE Breite );
 SWORD  ptGetBufferMemory             ( WORD TextLength );
 SWORD  ptWriteToMemoryBuffer         ( WORD Zeile, WORD VerUnits, WORD AnzZeichen, CHAR *Text, DOUBLE Left,
                                        DOUBLE Right, WORD Method );
 SWORD  ptPrintAutoBuffer             ( WORD Zeile, WORD VerUnits, CHAR *Text, DOUBLE Left,
                                        DOUBLE Right, WORD Method );
 SWORD  ptPrintBufferWithLine         ( WORD XZeile );
 SWORD  ptPrintBetweenColumnsBuffer   ( WORD Zeile, WORD VerUnits, CHAR *Text, WORD ColLeft,
                                        WORD ColRight, WORD Method );
 SWORD  ptPrintInColumnBuffer         ( WORD Zeile, WORD VerUnits, CHAR *Text, WORD Column, WORD Method );
 SWORD  ptSendBuffer                  ( VOID );
 SWORD  ptEmptyBuffer                 ( VOID );
 VOID   ptClearLoadedData             ( VOID );
 SWORD  ptLoadCodes                   ( CHAR **Zeiger, FILE **Datei );
 SWORD  ptLoadString                  ( CHAR *Zeiger, FILE **Datei );
 SWORD  ptLoadParameters              ( FILE **Datei );
 SWORD  ptInitPrinter                 ( CHAR *Drucker );
 SWORD  ptScanInitPrinter             ( CHAR *Drucker );
 SWORD  ptClosePrinter                ( VOID );

/***************************************************************************
 * Es folgen: Die Funktionen selbst                                        *
 **************************************************************************/

/****************************************************************************
 *   Funktionsname: ptFunc                                                  *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : keine                                                       *
 *                                                                          *
 *   Beschreibung: Dummyfunktion für Verwaltung der Traps.                  *
 ****************************************************************************/
VOID ptFunc ( VOID )
{
}

/****************************************************************************
 *   Funktionsname: ptfputc                                                 *
 *                                                                          *
 *   Parameter: CHAR                                                        *
 *                                                                          *
 *   Rückgabe : SWORD                                                       *
 *                                                                          *
 *   Beschreibung: Ausgabefunktion für Toolbox                              *
 ****************************************************************************/
SWORD ptfputc ( CHAR Zeichen )
{
      BYTE Ergebnis,
           PortNr;

      if ( ParallelPort != 0 )
      {
           PortNr = ParallelPort - 1;
           _asm mov ah, 0x02
           _asm mov dl, PortNr
           _asm mov dh, 0x00
           _asm int 0x17
           _asm mov Ergebnis, ah

           if ( ( Ergebnis & 16 ) == 0 || ( Ergebnis & 32 ) > 0 ) return ( EOF );

           _asm mov ah, 0x00
           _asm mov al, Zeichen
           _asm mov dh, 0x00
           _asm mov dl, PortNr
           _asm int 0x17
           _asm mov Ergebnis, ah

           if ( ( Ergebnis & 16 ) == 0 || ( Ergebnis & 32 ) > 0 ) return ( EOF );
      }
      else
      if ( SerialPort != 0 )
      {
           PortNr = SerialPort - 1;
           _asm mov ah, 0x03
           _asm mov dl, PortNr
           _asm mov dh, 0x00
           _asm int 0x14
           _asm mov Ergebnis, ah

           if ( ( Ergebnis & 64 ) == 0 && ( Ergebnis & 32 ) == 0 &&
                ( Ergebnis & 1  ) == 0 ) return ( EOF );

           _asm mov ah, 0x01
           _asm mov al, Zeichen
           _asm mov dh, 0x00
           _asm mov dl, PortNr
           _asm int 0x14
           _asm mov Ergebnis, ah

           if ( ( Ergebnis & 64 ) == 0 && ( Ergebnis & 32 ) == 0 &&
                ( Ergebnis & 1  ) == 0 ) return ( EOF );
      }
      else
      {
          return ( fputc( Zeichen, DruckerHandle ) );
      }

      return( Zeichen );
}

/****************************************************************************
 *   Funktionsname: ptDefineLeftMargin [USER]                               *
 *                                                                          *
 *   Parameter: DOUBLE: Platz zwischen Blattrand und Text in inch, bzw. cm  *
 *                                                                          *
 *   Rückgabe : SWORD : PT_OK oder Fehlermeldung                            *
 *                                                                          *
 *   Beschreibung: Funktion gibt systeminterne Werte zur Weiterverarbeitung *
 *                 an den User weiter                                       *
 ****************************************************************************/
SWORD ptDefineLeftMargin ( DOUBLE LeftMargin )
{
       if ( AktMeasureUnit == PT_CM ) LeftMargin = ptCm2Inch( LeftMargin );

       if ( ptInch2Cm( LeftMargin) >= SpaltenBreiteCM ) return( PT_WRONGMETHOD );

       LeftAdjustment = LeftMargin;

       return( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptGetDoubleInfo [USER]                                  *
 *                                                                          *
 *   Parameter: BYTE  : Angabe über Art der Information (PT_LINE, o.ä.)     *
 *                                                                          *
 *   Rückgabe : DOUBLE: Angeforderte Information                            *
 *                                                                          *
 *   Beschreibung: Funktion gibt systeminterne Werte zur Weiterverarbeitung *
 *                 an den User weiter                                       *
 ****************************************************************************/
DOUBLE ptGetDoubleInfo ( BYTE Info )
{
       if ( Info == PT_HORLENGTH )
       {
            if ( AktMeasureUnit == PT_INCH ) return( ptCm2Inch( SpaltenBreiteCM ) );
            return( SpaltenBreiteCM );
       }

       return( PT_WRONGPARAMETER );
}

/****************************************************************************
 *   Funktionsname: ptGetInfo [USER]                                        *
 *                                                                          *
 *   Parameter: BYTE  : Angabe über Art der Information (PT_LINE, o.ä.)     *
 *                                                                          *
 *   Rückgabe : WORD  : Angeforderte Information                            *
 *                                                                          *
 *   Beschreibung: Funktion gibt systeminterne Werte zur Weiterverarbeitung *
 *                 an den User weiter                                       *
 ****************************************************************************/
WORD ptGetInfo ( BYTE Info )
{
     WORD HZahl,
          FontSize;

     if ( Info == PT_LINE            ) return( ActiveLine           );
     if ( Info == PT_MEASUREUNIT     ) return( AktMeasureUnit       );
     if ( Info == PT_NUMBEROFCOLUMNS ) return( AnzahlColumns        );
     if ( Info == PT_BUFFERENTRIES   ) return( AnzahlBufferEintrage );
     if ( Info == PT_COLUMN          ) return( ActiveColumn         );
     if ( Info == PT_NUMBEROFTRAPS   ) return( AnzahlTraps          );
     if ( Info == PT_VERUNITS        ) return( VerUnitsMoved        );
     if ( Info == PT_PAGESPRINTED    ) return( PagesPrinted         );
     if ( Info == PT_LINESPERPAGE    ) return( LinesPerPage         );
     if ( Info == PT_BOLD            ) return( ActiveBold           );
     if ( Info == PT_UNDERLINE       ) return( ActiveUnderline      );
     if ( Info == PT_ITALIC          ) return( ActiveItalic         );
     if ( Info == PT_SPECIALCHARS    ) return( ActiveSpecialChars   );
     if ( Info == PT_VERUNIT         ) return( VerticalMoveUnit     );
     if ( Info == PT_VERUNITSPERLINE ) return( VerUnitsPerLine      );
     if ( Info == PT_PRINTERTYPE     ) return( VerMoveKind          );
     if ( Info == PT_AKTFONT         ) return( AktFont              );
     if ( Info == PT_NUMBEROFFONTS   ) return( Fonts                );
     if ( Info == PT_CHARSPERINCH    ) return( Chars_per_inch       );
     if ( Info == PT_CHARSPERLINE    )
          return( (WORD) ( Chars_per_inch * ptCm2Inch( SpaltenBreiteCM ) ) );
     if ( Info == PT_BIGGESTFONT    )
     {
          if ( Fonts == 0 ) return( PT_WRONGMETHOD );
          FontSize = 32000;
          for ( HZahl=0; HZahl<Fonts; HZahl++)
                if ( ( FontZeiger + HZahl )->Chars_per_inch < FontSize )
                       FontSize = ( FontZeiger + HZahl )->Chars_per_inch;
          return( FontSize );
     }
     if ( Info == PT_SMALLESTFONT     )
     {
          if ( Fonts == 0 ) return( PT_WRONGMETHOD );
          FontSize = 0;
          for ( HZahl=0; HZahl<Fonts; HZahl++)
                if ( ( FontZeiger + HZahl )->Chars_per_inch > FontSize )
                       FontSize = ( FontZeiger + HZahl )->Chars_per_inch;
          return( FontSize );
     }

     return( PT_WRONGPARAMETER );
}

/****************************************************************************
 *   Funktionsname: ptGetStrInfo [USER]                                     *
 *                                                                          *
 *   Parameter: BYTE  : Angabe über Art der Information (PT_LINE, o.ä.)     *
 *                                                                          *
 *   Rückgabe : CHAR* : Zeiger auf geforderten String oder NULL-Zeiger      *
 *                                                                          *
 *   Beschreibung: Funktion gibt systeminternen String z. Weiterverarbeitung*
 *                 an den User weiter                                       *
 ****************************************************************************/
CHAR *ptGetStrInfo ( BYTE Info )
{
     if ( Info == PT_PRINTER  ) return( PrinterName );
     if ( Info == PT_PORT     ) return( Port        );
     if ( Info == PT_FONTNAME ) return( ( FontZeiger + AktFont ) -> FontName );

     return( NULL );
}

/****************************************************************************
 *   Funktionsname: ptSetSpecialChar [USER]                                 *
 *                                                                          *
 *   Parameter: WORD  : PT_ON oder PT_OFF                                   *
 *                                                                          *
 *   Rückgabe : SWORD : PT_OK oder Fehlermeldung                            *
 *                                                                          *
 *   Beschreibung: Diese Funktion ist nur für Sofortausgabebefehle (d.h.    *
 *                 NICHT für Bufferbefehle) interessant !                   *
 *                 Diese Funktion schaltet die Wirkung der Beiden Zeichen   *
 *                 '_' (für Underline an/aus) und '^' (für Fettdruck an/aus)*
 *                 ein (PT_ON) oder aus (PT_OFF) !
 ****************************************************************************/
SWORD ptSetSpecialChar ( WORD Switch )
{
     if ( Switch == PT_ON )
     {
          ActiveSpecialChars = PT_TRUE;
     }
     else
     if ( Switch == PT_OFF )
     {
          ActiveSpecialChars = PT_FALSE;
     }
     else
     {
          return ( PT_WRONGPARAMETER );
     }

     return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintNowChar [USER]                                   *
 *                                                                          *
 *   Parameter: Character : An aktueller Position auszugebendes Zeichen     *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, an der aktuellen Druckkopfpos-   *
 *                 ition eine Zeichen auszugeben.                           *
 ****************************************************************************/

SWORD  ptPrintNowChar ( CHAR Character )
{
       BOOL SpecialChar = PT_FALSE;

       if ( ActiveSpecialChars == PT_TRUE )
       {
            if ( Character == '_' && ActiveUnderline == PT_FALSE )
            {
                 if ( ptSwitchFontCharacter( PT_UNDERLINE, PT_ON ) < 0 ) return ( PT_PRINTERERROR );
                 SpecialChar = PT_TRUE;
            }
            else
            if ( Character == '_' && ActiveUnderline == PT_TRUE )
            {
                 if ( ptSwitchFontCharacter( PT_UNDERLINE, PT_OFF ) < 0 ) return ( PT_PRINTERERROR );
                 SpecialChar = PT_TRUE;
            }
            else
            if ( Character == '^' && ActiveBold == PT_FALSE )
            {
                 if ( ptSwitchFontCharacter( PT_BOLD, PT_ON ) < 0 ) return ( PT_PRINTERERROR );
                 SpecialChar = PT_TRUE;
            }
            else
            if ( Character == '^' && ActiveBold == PT_TRUE )
            {
                 if( ptSwitchFontCharacter( PT_BOLD, PT_OFF ) < 0 ) return ( PT_PRINTERERROR );
                 SpecialChar = PT_TRUE;
            }
       }

       if ( SpecialChar == PT_FALSE )
       {
            if ( ptfputc( (char) Character ) == EOF ) return ( PT_PRINTERERROR );
       }

       return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptSendSequenz                                           *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Sequenzbeginn                           *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion sendet eine Druckersequenz.               *
 ****************************************************************************/

SWORD  ptSendSequenz ( CHAR *Start )
{
       BYTE  HZahl;

       if ( *Start > 0 )
            for( HZahl=1; HZahl<=*Start; HZahl++)
            {
                  if ( ptPrintNowChar( *(Start+HZahl) ) < 0 ) return ( PT_PRINTERERROR );
            }
       return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptResetPrinter [USER]                                   *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion sendet die Reset-Sequenz an den Drucker.  *
 ****************************************************************************/

SWORD  ptResetPrinter ( VOID )
{
       return ( ptSendSequenz( Reset ) );
}

/****************************************************************************
 *   Funktionsname: ptSwitchOffFontCharacteristics [USER]                   *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion schaltet alle FOnt-Charakteristica aus.   *
 ****************************************************************************/

SWORD  ptSwitchOffFontCharacteristics( VOID )
{
       if ( ActiveBold == PT_TRUE )
       {
            if ( ptSendSequenz( BoldOff ) < 0 ) return( PT_PRINTERERROR );
       }
       if ( ActiveUnderline == PT_TRUE )
       {
            if ( ptSendSequenz( UnderlineOff ) < 0 ) return( PT_PRINTERERROR );
       }
       if ( ActiveItalic == PT_TRUE )
       {
            if ( ptSendSequenz( ItalicOff ) < 0 ) return( PT_PRINTERERROR );
       }

       return ( PT_OK );

}

/****************************************************************************
 *   Funktionsname: ptSwitchOnFontCharacteristics [USER]                    *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion schaltet alle Font-Charakteristica ein.   *
 ****************************************************************************/

SWORD  ptSwitchOnFontCharacteristics( VOID )
{
       if ( ActiveBold == PT_TRUE )
       {
            if ( ptSendSequenz( BoldOn ) < 0 ) return ( PT_PRINTERERROR );
       }
       if ( ActiveUnderline == PT_TRUE )
       {
            if ( ptSendSequenz( UnderlineOn ) < 0 ) return ( PT_PRINTERERROR );
       }
       if ( ActiveItalic == PT_TRUE )
       {
            if ( ptSendSequenz( ItalicOn ) < 0 ) return ( PT_PRINTERERROR );
       }

       return ( PT_OK );

}

/****************************************************************************
 *   Funktionsname: ptSwitchFontCharacter [USER]                            *
 *                                                                          *
 *   Parameter: WORD : PT_BOLD, PT_UNDERLINE oder PT_ITALIC                 *
 *              WORD : PT_ON oder PT_OFF                                    *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Ändern des Schriftcharakters (Fettdruck, Unterstreichen, *
 *                 Kursivschrift).                                          *
 ****************************************************************************/

SWORD ptSwitchFontCharacter ( WORD Character, WORD OnOff )
{
      if ( Character == PT_BOLD && OnOff == PT_ON )
      {
           if ( ptSendSequenz( BoldOn ) < 0 ) return ( PT_PRINTERERROR );
                ActiveBold = PT_TRUE;
      }
      else
      if ( Character == PT_BOLD && OnOff == PT_OFF )
      {
           if ( ptSendSequenz( BoldOff ) < 0 ) return ( PT_PRINTERERROR );
                ActiveBold = PT_FALSE;
      }
      else
      if ( Character == PT_UNDERLINE && OnOff == PT_ON )
      {
           if ( ptSendSequenz( UnderlineOn ) < 0 ) return ( PT_PRINTERERROR );
                ActiveUnderline = PT_TRUE;
      }
      else
      if ( Character == PT_UNDERLINE && OnOff == PT_OFF )
      {
           if ( ptSendSequenz( UnderlineOff ) < 0 ) return ( PT_PRINTERERROR );
                ActiveUnderline = PT_FALSE;
      }
      else
      if ( Character == PT_ITALIC && OnOff == PT_ON )
      {
           if ( ptSendSequenz( ItalicOn ) < 0 ) return ( PT_PRINTERERROR );
                ActiveItalic = PT_TRUE;
      }
      else
      if ( Character == PT_ITALIC && OnOff == PT_OFF )
      {
           if ( ptSendSequenz( ItalicOff ) < 0 ) return ( PT_PRINTERERROR );
                ActiveItalic = PT_FALSE;
      }
      else
      {
           return ( PT_WRONGPARAMETER );
      }

      return ( PT_OK );

}

/****************************************************************************
 *   Funktionsname: ptCheckForTrap [USER]                                   *
 *                                                                          *
 *   Parameter: Word   : Nummer der zu untersuchenden zeile                 *
 *                                                                          *
 *   Rückgabe : PT_TRUE (Trap existiert) oder PT_FALSE (Trap existiert      *
 *                      nicht)                                              *
 *                                                                          *
 *   Beschreibung: Diese Funktion prüft, ob eine bestimmte Zeile schon mit  *
 *                 einem Trap belegt ist.                                   *
 ****************************************************************************/

SWORD ptCheckForTrap ( WORD Line )
{
      WORD HZahl = 0;

      if ( AnzahlTraps == 0 )
      {
           return( PT_FALSE );
      }
      while ( HZahl < AnzahlTraps )
      {
           if ( ( TrapZeiger + HZahl ) -> Line == Line &&
                ( TrapZeiger + HZahl ) -> Activated == PT_TRUE )
           {
                return ( PT_TRUE );
           }
           HZahl++;
      }
      return ( PT_FALSE );
}

/****************************************************************************
 *   Funktionsname: ptDontUseTrap [USER]                                    *
 *                                                                          *
 *   Parameter: Word   : Nummer der Zeile, deren Trap demarkiert werden soll*
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion deaktiviert den Trap einer bestimmten     *
 *                 Zeile (falls definiert), bis er mit ptUseTrap wieder     *
 *                 aktiviert wird.                                          *
 ****************************************************************************/

SWORD ptDontUseTrap ( WORD Line )
{
      WORD HZahl = 0;

      if ( AnzahlTraps == 0 )
      {
           return( PT_TRAPNOTDEFINED );
      }
      while ( HZahl < AnzahlTraps )
      {
           if ( ( TrapZeiger + HZahl ) -> Line == Line )
           {
                ( TrapZeiger + HZahl ) -> Activated = PT_FALSE;
                return ( PT_OK );
           }
           HZahl++;
      }
      return ( PT_TRAPNOTDEFINED );
}

/****************************************************************************
 *   Funktionsname: ptUseTrap [USER]                                        *
 *                                                                          *
 *   Parameter: Word   : Nummer der Zeile, deren Trap verwendet werden soll *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion aktiviert den Trap einer bestimmten       *
 *                 Zeile (falls definiert), wenn er mit ptDontUseTrap zuvor *
 *                 deaktiviert wurde.                                       *
 ****************************************************************************/

SWORD ptUseTrap ( WORD Line )
{
      WORD HZahl = 0;

      if ( AnzahlTraps == 0 )
      {
           return( PT_TRAPNOTDEFINED );
      }
      while ( HZahl < AnzahlTraps )
      {
           if ( ( TrapZeiger + HZahl ) -> Line == Line )
           {
                ( TrapZeiger + HZahl ) -> Activated = PT_TRUE;
                return ( PT_OK );
           }
           HZahl++;
      }
      return ( PT_TRAPNOTDEFINED );
}


/****************************************************************************
 *   Funktionsname: ptDefineTrap [USER]                                     *
 *                                                                          *
 *   Parameter: Word   : Nummer der Zeile                                   *
 *              VOID   : Zeiger auf eine Prozedur, die keine Parameter haben*
 *                       und keine Werte zurückgeben darf.                  *
 *                       (z.B. VOID FUNKTION ( VOID ) )                     *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt die Definition eines Traps.       *
 ****************************************************************************/

SWORD ptDefineTrap ( WORD Line, VOID (*ptFunc)() )
{
      TRAP *HZeiger;

      if ( Line == 0 )
      {
           return ( PT_WRONGPARAMETER );
      }

      if ( ptCheckForTrap( Line ) == PT_TRUE )
      {
           return ( PT_ALREADYDEFINED );
      }
      if ( AnzahlTraps == 0 )
      {
           TrapZeiger = malloc( sizeof( TRAP ) );
           if ( TrapZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           AnzahlTraps = 1;
      }
      else
      {
           HZeiger = TrapZeiger;
           HZeiger = realloc ( HZeiger, ( AnzahlTraps + 1 ) * sizeof ( TRAP ) );
           if ( HZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           TrapZeiger = HZeiger;
           AnzahlTraps++;
      }

      ( TrapZeiger + AnzahlTraps - 1 ) -> Line       = Line;
      ( TrapZeiger + AnzahlTraps - 1 ) -> Activated  = PT_TRUE;
      ( TrapZeiger + AnzahlTraps - 1 ) -> ptFunc     = ptFunc;

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptDeleteTrap [USER]                                     *
 *                                                                          *
 *   Parameter: Word   : Nummer der Zeile                                   *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, eine zuvor definierte Traps zu   *
 *                 löschen.                                                 *
 ****************************************************************************/

SWORD ptDeleteTrap ( WORD Line )
{
      TRAP   *HZeiger;
      WORD    HTrap,
           NewTraps;

      if ( ptCheckForTrap( Line ) == PT_FALSE || AnzahlTraps == 0 )
      {
           return ( PT_TRAPNOTDEFINED );
      }
      if ( AnzahlTraps == 1 )
      {
           free ( TrapZeiger );
           AnzahlTraps = 0;
           return ( PT_OK );
      }
      else
      {
           HZeiger = calloc  ( ( AnzahlTraps - 1 ), sizeof ( TRAP ) );
           if ( HZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           HTrap = AnzahlTraps;
           AnzahlTraps--;
           NewTraps = 0;
           while ( HTrap > 0 )
           {
                 if ( ( TrapZeiger + HTrap - 1 ) -> Line != Line )
                 {
                      ( HZeiger + NewTraps ) -> Line      = ( TrapZeiger + HTrap - 1 ) -> Line;
                      ( HZeiger + NewTraps ) -> Activated = ( TrapZeiger + HTrap - 1 ) -> Activated;
                      ( HZeiger + NewTraps ) -> ptFunc    = ( TrapZeiger + HTrap - 1 ) -> ptFunc;
                      NewTraps++;
                 }
                 HTrap--;
           }
           free( TrapZeiger );
           TrapZeiger = HZeiger;
      }

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptCallTraps                                             *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : Ok oder Fehlermeldung                                       *
 *                                                                          *
 *   Beschreibung: Diese Funktion ruft - falls vorhanden - eine zu einer    *
 *                 Zeile gehörige Funktion auf.                             *
 ****************************************************************************/

SWORD ptCallTraps ( VOID )
{
      WORD    HTrap;

      if ( AnzahlTraps == 0 )
      {
           return ( PT_TRAPNOTDEFINED );
      }

      HTrap = AnzahlTraps;

      while ( HTrap > 0 && ( ( TrapZeiger + HTrap ) -> Line != ActiveLine ||
            ( TrapZeiger + HTrap ) -> Activated != PT_TRUE ) )
      {
           HTrap--;
      }

      if ( ( TrapZeiger + HTrap ) -> Line == ActiveLine &&
           ( TrapZeiger + HTrap ) -> Activated == PT_TRUE )
      {
           ( TrapZeiger + HTrap ) -> ptFunc();
      }

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptMoveVerUnits                                          *
 *                                                                          *
 *   Parameter: WORD   : Anzahl vertikaler Bewegungseinheiten               *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion übergeht die angegebene Anzahl vertikaler *
 *                 Bewegungseinheiten (normal n/300 '' bei Laserdruckern)   *
 ****************************************************************************/

SWORD  ptMoveVerUnits( WORD Units )
{
       WORD Zaehler;
       CHAR ASCIIZahl[6];

       if ( *PreVerCode > 0 )
       {
           for (Zaehler=0; Zaehler<*PreVerCode; Zaehler++)
           {
               if ( ptfputc( (char) *( PreVerCode + Zaehler + 1) ) == EOF )
                    return ( PT_PRINTERERROR );
           }
       }
       if ( VerMoveKind == PT_ESC )
       {
           VerUnitsMoved += Units;

           /* EOF (ASCII 1A = 26 - Problem ) */

           if ( Units == 26 )
           {
               if ( ptfputc( (char) 25 ) == EOF ) return ( PT_PRINTERERROR );
               Units = 1;
               if ( *AftVerCode > 0 )
               {
                    for (Zaehler=0; Zaehler<*AftVerCode; Zaehler++)
                    {
                         if ( ptfputc( (char) *( AftVerCode + Zaehler + 1 ) ) == EOF )
                              return ( PT_PRINTERERROR );
                    }
               }
               if ( *PreVerCode > 0 )
               {
                    for (Zaehler=0; Zaehler<*PreVerCode; Zaehler++)
                    {
                         if ( ptfputc( (char) *( PreVerCode + Zaehler + 1 ) ) == EOF )
                              return ( PT_PRINTERERROR );
                     }
               }
           }
           if ( ptfputc( (char) Units ) == EOF ) return ( PT_PRINTERERROR );
       }
       else
       if ( VerMoveKind == PT_HP )
       {
            VerUnitsMoved += Units;
            itoa( VerUnitsMoved + ActiveLine*VerUnitsPerLine , ASCIIZahl, 10 );
            for ( Zaehler=0; Zaehler<strlen( ASCIIZahl ); Zaehler++)
            {
                  if ( ptfputc( (char) ASCIIZahl[Zaehler] ) == EOF ) return ( PT_PRINTERERROR );
            }
       }
       else
       {
            return ( PT_UNITNOTKNOWN );
       }

       if ( *AftVerCode > 0 )
       {
            for (Zaehler=0; Zaehler<*AftVerCode; Zaehler++)
            {
                 if ( ptfputc( (char) *( AftVerCode + Zaehler + 1) ) == EOF )
                      return ( PT_PRINTERERROR );
            }
       }

       return( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptSkip [USER]                                           *
 *                                                                          *
 *   Parameter: WORD   : Anzahl Zeilen                                      *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion übergeht die angegebene Anzahl Zeilen.    *
 ****************************************************************************/

SWORD  ptSkip( WORD Lines )
{
       WORD StillToMove;

       if ( VerUnitsMoved == 0 )
       {
            while ( Lines > 0 && ActiveLine < LinesPerPage )
            {
                  if ( ptfputc( (char) PT_CR ) == EOF ) return ( PT_PRINTERERROR );
                  if ( ptfputc( (char) PT_LF ) == EOF ) return ( PT_PRINTERERROR );
                  ActiveLine++;
                  Lines--;
                  ptCallTraps();
            }
       }
       else
       {
            while ( Lines > 0 && ActiveLine < LinesPerPage )
            {
                  StillToMove = VerUnitsMoved;
                  if ( ptMoveVerUnits( VerUnitsPerLine - VerUnitsMoved ) < 0 ) return ( PT_PRINTERERROR );
                  VerUnitsMoved = 0;
                  ActiveLine++;
                  Lines--;
                  ptCallTraps();
                  if ( ActiveLine < LinesPerPage )
                  {
                       if ( ptMoveVerUnits( StillToMove ) < 0 ) return ( PT_PRINTERERROR );
                  }
            }
       }

       if ( ActiveLine == LinesPerPage )
       {
            ActiveLine    = 0;
            VerUnitsMoved = 0;
            if ( ptfputc( (char) NewPage ) == EOF ) return ( PT_PRINTERERROR );
            PagesPrinted++;
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptSkipExact [USER]                                      *
 *                                                                          *
 *   Parameter: WORD   : Anzahl Units ( definiert in VerticalMoveUnit)      *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion übergeht die angegebene Anzahl Units in   *
 *                 vertikaler Richtung ( Beispiel Laserdrucker: n/300 Zoll )*
 ****************************************************************************/

SWORD  ptSkipExact( WORD Units )
{
       WORD UnitsLeft;

       if ( Units > VerUnitsPerLine )
       {
            if ( ptSkip ( (WORD) ( Units / VerUnitsPerLine ) )  < 0 ) return ( PT_PRINTERERROR );
            Units = Units % VerUnitsPerLine;
       }

       if ( VerUnitsMoved + Units > VerUnitsPerLine )
       {
            UnitsLeft = ( VerUnitsMoved+Units ) - VerUnitsPerLine;
            if ( ptMoveVerUnits( VerUnitsPerLine - VerUnitsMoved ) < 0 ) return ( PT_PRINTERERROR );
            VerUnitsMoved = 0;
            ActiveLine++;
            ptCallTraps();
            if ( ActiveLine < LinesPerPage )
            {
                 if ( ptMoveVerUnits( UnitsLeft ) < 0 ) return ( PT_PRINTERERROR );
            }
            if ( ActiveLine == LinesPerPage )
            {
                 ActiveLine    = 0;
                 VerUnitsMoved = 0;
                 if ( ptfputc( (char) NewPage )  == EOF ) return ( PT_PRINTERERROR );
                 PagesPrinted++;
            }
       }
       else
       {
            if ( ptMoveVerUnits( Units )< 0 ) return ( PT_PRINTERERROR );
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptSkipExactStandard [USER]                              *
 *                                                                          *
 *   Parameter: WORD   : Anzahl Units ( 1 Unit = 1/48 Zoll )                *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion übergeht die angegebene Anzahl Units in   *
 *                 vertikaler Richtung. Dabei ist diese Funktion unab-      *
 *                 hängig vom Drucker Modell ( nicht so ptSkipExact ! ).    *
 *                 Eine Unit entspricht in jedem Falle 1/48 Zoll.           *
 ****************************************************************************/

SWORD  ptSkipExactStandard( WORD Units )
{
       return ( ptSkipExact( (WORD) ( (DOUBLE) Units * (DOUBLE) ( (DOUBLE) VerticalMoveUnit / 48 ) ) ) );
}


/****************************************************************************
 *   Funktionsname: ptAnzahlWorte                                           *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Textstring                              *
 *              DOUBLE : Linke Spalte in inch                               *
 *              DOUBLE : Rechte Spalte in inch                              *
 *                                                                          *
 *   Rückgabe : SWORD  : Anzahl Worte, die zwischen Left und Right passen.  *
 *                                                                          *
 *   Beschreibung: Diese Funktion zählt die Anzahl Worte, welche zwischen   *
 *                 den Grenzen Left und Right Platz findet. Liegt ein       *
 *                 CR und/oder LF vor, ist der Rückgabewert negativ.        *
 ****************************************************************************/

SWORD ptAnzahlWorte( CHAR* Text, DOUBLE Left, DOUBLE Right )
{
      DOUBLE Space;
      WORD   ZeichenNr  = 0,
             MengeWorte = 0;

      Space = ( Right - Left ) * (DOUBLE) Chars_per_inch;
      if ( Space < 1 )
      {
           return ( 0 );
      }
      while ( Text[ZeichenNr] == PT_SPACE)
      {
            ZeichenNr++;
      }
      while ( ZeichenNr < Space )
      {
            if ( Text[ZeichenNr] == PT_CR || Text[ZeichenNr] == PT_LF )
            {
                 Text[ZeichenNr] = PT_SPACE;
                  if ( Text[ZeichenNr + 1] == PT_CR || Text[ZeichenNr + 1] == PT_LF )
                  {
                       Text[ZeichenNr + 1] = PT_SPACE;
                  }
                  if ( Text[ZeichenNr - 1] != PT_SPACE || Text[ZeichenNr + 2] == PT_CR ||
                       Text[ZeichenNr + 2] == PT_LF || ( MengeWorte == 0 && Text[ZeichenNr] == PT_SPACE) )
                  {
                       return ( - ( MengeWorte + 1 ) );
                  }
                  else
                  {
                       return ( - MengeWorte );
                  }
            }
            if ( Text[ZeichenNr] == PT_EOS )
            {
                 return ( MengeWorte + 1);
            }
            if ( Text[ZeichenNr] == PT_SPACE )
            {
                 MengeWorte++;
            }
            ZeichenNr++;
      }

      return ( MengeWorte );
}

/****************************************************************************
 *   Funktionsname: ptMoveHeadToHorPosition                                 *
 *                                                                          *
 *   Parameter: DOUBLE : Linker Rand (inch INCH !!)                         *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion fährt den Druckkopf von jeder Position    *
 *                 zu der übergebenen Stelle.                               *
 ****************************************************************************/

SWORD  ptMoveHeadToHorPosition( DOUBLE Pos )
{
       WORD HZahl,
            HZahl2;

       if ( ptSwitchOffFontCharacteristics() < 0 ) return ( PT_PRINTERERROR );

       if ( ptfputc( (char) PT_CR ) == EOF ) return ( PT_PRINTERERROR );

       Pos += LeftAdjustment;  /* Linken Rand, wenn befohlen, einrücken */

       if ( Pos > 1/(DOUBLE) Chars_per_inch )
       {
            if ( (DOUBLE) ( Pos * (DOUBLE) Chars_per_inch) -
                 (DOUBLE) ( Pos * (DOUBLE) Chars_per_inch) >= 0.5 )
            {
                 HZahl2 = 1;
            }
            else
            {
                 HZahl2 = 0;
            }
            for( HZahl = 1; HZahl <= (Pos * (DOUBLE) Chars_per_inch) + HZahl2; HZahl++ )
            {
                if ( ptfputc( (char) PT_SPACE ) == EOF ) return ( PT_PRINTERERROR );
            }
       }

       return ( ptSwitchOnFontCharacteristics() );
}

/****************************************************************************
 *   Funktionsname: ptPrintLeft                                             *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand in inch                                *
 *              DOUBLE : Rechter Rand in inch                               *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text zwischen den  *
 *                 angegebenen Spalten linksbündig aus. Paßt der Text nicht *
 *                 in eine Zeile, findet automatisch ein entsprechender Um- *
 *                 bruch statt, bis der komplette Text ausgegeben ist.      *
 ****************************************************************************/

SWORD  ptPrintLeft( CHAR* Text, DOUBLE Left, DOUBLE Right )
{
       CHAR* TextZeiger;
       WORD  Buchstaben  = 0,
             GesamtLang,
             HZahl;
       SWORD DruckeWorte;

       TextZeiger = Text;
       GesamtLang = strlen( Text );

       if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) ) return ( PT_WRONGMARGINS );

       while ( Buchstaben < GesamtLang )
       {
             while ( TextZeiger[0] == PT_SPACE )
             {
                   TextZeiger++;
                   Buchstaben++;
             }
             if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );
             DruckeWorte = ptAnzahlWorte( TextZeiger, Left, Right );
             if ( DruckeWorte < 0 )
             {
                  DruckeWorte = - DruckeWorte;
             }
             if ( DruckeWorte == 0 )
             {
                  if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );
                  for (HZahl = 1; HZahl <= ( Right - Left ) * (DOUBLE) Chars_per_inch; HZahl++)
                  {
                       if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                       TextZeiger++;
                  }
                  Buchstaben += ( Right - Left ) * (DOUBLE) Chars_per_inch;
             }
             else
             {
                  while ( DruckeWorte > 0 && Buchstaben < GesamtLang )
                  {
                          if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                          if ( TextZeiger[0] == PT_SPACE )
                          {
                               DruckeWorte--;
                          }
                          TextZeiger++;
                          Buchstaben++;
                  }
             }
             if ( Buchstaben < GesamtLang )
             {
                  if ( ptSkip( 1 ) < 0 ) return ( PT_PRINTERERROR );
             }
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintCenterOrRight                                    *
 *                                                                          *
 *   Parameter: BYTE   : PT_RIGHT (rechtsbündig), PT_CENTER (zentriert)     *
 *              CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand in inch                                *
 *              DOUBLE : Rechter Rand in inch                               *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text zwischen den  *
 *                 angegebenen Spalten rechtsbündig, bzw. zentriert aus.    *
 *                 Paßt der Text nicht in eine Zeile, findet automatisch ein*
 *                 entsprechender Umbruch statt, bis der komplette Text     *
 *                 ausgegeben ist.                                          *
 ****************************************************************************/

SWORD  ptPrintCenterOrRight( BYTE Ausrichtung, CHAR* Text, DOUBLE Left, DOUBLE Right )
{
       CHAR* TextZeiger;
       WORD  Buchstaben  = 0,
             GesamtLang,
             HZahl,
             HZahl2,
             NeueBuchstaben,
             DruckeBuchstaben;
       SWORD DruckeWorte;

       TextZeiger = Text;
       GesamtLang = strlen( Text );

       if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) ) return ( PT_WRONGMARGINS );

       while ( Buchstaben < GesamtLang )
       {
             while ( TextZeiger[0] == PT_SPACE )
             {
                   TextZeiger++;
                   Buchstaben++;
             }
             DruckeWorte = ptAnzahlWorte( TextZeiger, Left, Right );
             if ( DruckeWorte < 0 )
             {
                  DruckeWorte = - DruckeWorte;
             }
             if ( DruckeWorte == 0 )
             {
                  if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );

                  if ( (DOUBLE) ( (Right - Left) * (DOUBLE) Chars_per_inch ) -
                       (WORD) ( (Right - Left) * (DOUBLE) Chars_per_inch ) >= 0.5 )
                  {
                       HZahl2 = 1;
                  }
                  else
                  {
                       HZahl2 = 0;
                  }
                  for (HZahl = 1; HZahl <= ( Right - Left ) * (DOUBLE) Chars_per_inch + HZahl2; HZahl++)
                  {
                       if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                       TextZeiger++;
                  }
                  Buchstaben += ( Right - Left ) * (DOUBLE) Chars_per_inch + HZahl2;
             }
             else
             {
                  NeueBuchstaben = 0;
                  while ( DruckeWorte > 0 && Buchstaben < GesamtLang )
                  {
                        if ( TextZeiger[NeueBuchstaben] == PT_SPACE || TextZeiger[NeueBuchstaben] == PT_EOS)
                        {
                             DruckeWorte--;
                        }
                        NeueBuchstaben++;
                        Buchstaben++;
                  }

                  DruckeBuchstaben = NeueBuchstaben;
                  while ( DruckeBuchstaben > 0 && ( TextZeiger[DruckeBuchstaben-1] == PT_SPACE ||
                          TextZeiger[DruckeBuchstaben-1] == PT_EOS ) )
                  {
                        DruckeBuchstaben--;
                  }

                  if ( ptMoveHeadToHorPosition( Left + (DOUBLE) ( ( ( (DOUBLE) ( Right - Left ) -
                       (DOUBLE) ( DruckeBuchstaben / (DOUBLE) Chars_per_inch ) ) ) / Ausrichtung ) )
                       < 0 ) return ( PT_PRINTERERROR );

                  while ( DruckeBuchstaben > 0 )
                  {
                        if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                        TextZeiger++;
                        DruckeBuchstaben--;
                        NeueBuchstaben--;
                  }

                  TextZeiger += NeueBuchstaben;
             }
             if ( Buchstaben < GesamtLang )
             {
                  if ( ptSkip( 1 )< 0 ) return ( PT_PRINTERERROR );
             }
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintJustify                                          *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand in inch                                *
 *              DOUBLE : Rechter Rand in inch                               *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text zwischen den  *
 *                 angegebenen Spalten im Blocksatz aus.                    *
 *                 Paßt der Text nicht in eine Zeile, findet automatisch ein*
 *                 entsprechender Umbruch statt, bis der komplette Text     *
 *                 ausgegeben ist.                                          *
 ****************************************************************************/

SWORD  ptPrintJustify( CHAR* Text, DOUBLE Left, DOUBLE Right )
{
       CHAR*  TextZeiger;
       WORD   Buchstaben  = 0,
              GesamtLang,
              HZahl,
              NeueBuchstaben,
              DruckeBuchstaben,
              GesamtSpaces,
              AktuellSpaces,
              GesamtNeueSpaces,
              NeueSpaces;
       DOUBLE SpacesperSpace,
              SpaceCount,
              SpacesperLetter;
       SWORD  DruckeWorte;

       if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) ) return ( PT_WRONGMARGINS );

       TextZeiger = Text;
       GesamtLang = strlen( Text );

       while ( Buchstaben < GesamtLang )
       {
             while ( TextZeiger[0] == PT_SPACE )
             {
                   TextZeiger++;
                   Buchstaben++;
             }

             DruckeWorte = ptAnzahlWorte( TextZeiger, Left, Right );
             if ( DruckeWorte >= 0 )
             {
                  HZahl = DruckeWorte;
             }
             else
             {
                  HZahl = - DruckeWorte;
             }
             NeueBuchstaben = 0;
             while ( HZahl > 0 )
             {
                   if ( TextZeiger[NeueBuchstaben] == PT_SPACE || TextZeiger[NeueBuchstaben] == PT_EOS)
                   {
                        HZahl--;
                   }
                   if ( TextZeiger[NeueBuchstaben] != PT_EOS )
                   {
                        NeueBuchstaben++;
                   }
             }
             while ( NeueBuchstaben >= 2 && TextZeiger[NeueBuchstaben - 1] == PT_SPACE &&
                     TextZeiger[NeueBuchstaben - 2] == PT_SPACE && DruckeWorte > 0 )
             {
                   NeueBuchstaben--;
                   DruckeWorte--;
             }
             if ( DruckeWorte < 0  )
             {
                  if ( ptMoveHeadToHorPosition( Left )< 0 ) return ( PT_PRINTERERROR );
                  DruckeWorte = - DruckeWorte;
                  while ( DruckeWorte > 0 )
                  {
                        if ( TextZeiger[0] == PT_SPACE )
                        {
                             DruckeWorte--;
                        }
                        if ( ptPrintNowChar( TextZeiger[0] )< 0 ) return ( PT_PRINTERERROR );
                        TextZeiger++;
                        Buchstaben++;
                  }
             }
             else
             if ( DruckeWorte == 0 )
             {
                  if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );
                  for (HZahl = 1; HZahl <= ( Right - Left ) * (DOUBLE) Chars_per_inch; HZahl++)
                  {
                       if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                       TextZeiger++;
                  }
                  Buchstaben += ( Right - Left ) * (DOUBLE) Chars_per_inch;
             }
             else
             if ( DruckeWorte == 1 )
             {
                  DruckeBuchstaben = 0;
                  while ( TextZeiger[DruckeBuchstaben] != PT_SPACE && TextZeiger[DruckeBuchstaben] != PT_EOS )
                  {
                        DruckeBuchstaben++;
                        Buchstaben++;
                  }
                  GesamtNeueSpaces= ( ( Right - Left ) * (DOUBLE) Chars_per_inch ) - DruckeBuchstaben;
                  if ( DruckeBuchstaben > 1 )
                  {
                       SpacesperLetter = ( ( ( Right - Left ) * (DOUBLE) Chars_per_inch ) -
                                             (DOUBLE) DruckeBuchstaben ) / (DOUBLE) ( DruckeBuchstaben - 1 );
                  }
                  else
                  {
                       SpacesperLetter = ( ( ( Right - Left ) * (DOUBLE) Chars_per_inch ) - 1);
                  }
                  SpaceCount      = 0;
                  AktuellSpaces   = 0;

                  if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );

                  while ( DruckeBuchstaben > 0 )
                  {
                        if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                        TextZeiger++;
                        SpaceCount += SpacesperLetter;
                        while ( SpaceCount >= 1 && DruckeBuchstaben > 1)
                        {
                              if ( ptPrintNowChar( PT_SPACE ) < 0 ) return ( PT_PRINTERERROR );
                              SpaceCount--;
                              AktuellSpaces++;
                        }
                        if ( DruckeBuchstaben <= 2 && DruckeBuchstaben > 1 )
                        {
                             while ( (SWORD) (GesamtNeueSpaces-AktuellSpaces) >= 1 )
                             {
                                   if ( ptPrintNowChar( PT_SPACE ) < 0 ) return ( PT_PRINTERERROR );
                                   AktuellSpaces++;
                             }
                        }
                        DruckeBuchstaben--;
                  }
             }
             else
             {
                  NeueBuchstaben = 0;
                  GesamtSpaces   = 0;
                  while ( DruckeWorte > 0 && Buchstaben < GesamtLang )
                  {
                        if ( TextZeiger[NeueBuchstaben] == PT_SPACE || TextZeiger[NeueBuchstaben] == PT_EOS)
                        {
                             DruckeWorte--;
                        }
                        if ( TextZeiger[NeueBuchstaben] == PT_SPACE )
                        {
                             GesamtSpaces++;
                        }
                        NeueBuchstaben++;
                        Buchstaben++;
                  }

                  DruckeBuchstaben = NeueBuchstaben;
                  while ( DruckeBuchstaben > 0 && ( TextZeiger[DruckeBuchstaben-1] == PT_SPACE ||
                          TextZeiger[DruckeBuchstaben-1] == PT_EOS ) )
                  {
                        if ( TextZeiger[DruckeBuchstaben-1] == PT_SPACE && GesamtSpaces > 0 )
                        {
                             GesamtSpaces--;
                        }
                        DruckeBuchstaben--;
                  }

                  if ( ptMoveHeadToHorPosition( Left ) < 0 ) return ( PT_PRINTERERROR );

                  AktuellSpaces    = 0;
                  GesamtNeueSpaces = ( ( Right - Left ) - (DOUBLE) ( DruckeBuchstaben /
                                       (DOUBLE) Chars_per_inch ) ) * (DOUBLE) Chars_per_inch;
                  if ( GesamtSpaces == 0 )
                  {
                       SpacesperSpace = 0;
                  }
                  else
                  {
                       SpacesperSpace   = ( ( ( ( Right - Left ) - (DOUBLE) ( DruckeBuchstaben /
                                            (DOUBLE) Chars_per_inch ) ) * (DOUBLE) Chars_per_inch ) /
                                            (DOUBLE) GesamtSpaces );
                  }
                  SpaceCount       = 0;
                  NeueSpaces       = 0;
                  while ( DruckeBuchstaben > 0 )
                  {
                        if ( ptPrintNowChar( TextZeiger[0] ) < 0 ) return ( PT_PRINTERERROR );
                        if ( TextZeiger[0] == PT_SPACE )
                        {
                             AktuellSpaces++;
                             SpaceCount += SpacesperSpace;
                             while ( SpaceCount >= 1 )
                             {
                                   if ( ptPrintNowChar( PT_SPACE ) < 0 ) return ( PT_PRINTERERROR );
                                   SpaceCount--;
                                   NeueSpaces++;
                             }
                             if ( AktuellSpaces == GesamtSpaces )
                             {
                                  while ( NeueSpaces < GesamtNeueSpaces )
                                  {
                                        if ( ptPrintNowChar( PT_SPACE ) < 0 ) return ( PT_PRINTERERROR );
                                        AktuellSpaces++;
                                        NeueSpaces++;
                                  }
                             }
                        }
                        TextZeiger++;
                        DruckeBuchstaben--;
                        NeueBuchstaben--;
                  }

                  TextZeiger += NeueBuchstaben;
             }
             if ( Buchstaben < GesamtLang )
             {
                  if ( ptSkip( 1 ) < 0 ) return ( PT_PRINTERERROR );
             }
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptCm2Inch                                               *
 *                                                                          *
 *   Parameter: DOUBLE : Maßeinheit in cm                                   *
 *                                                                          *
 *   Rückgabe : DOUBLE : Maßeinheit in inch ( 1 Inch = 1 Zoll = 2,54 cm )   *
 *                                                                          *
 *   Beschreibung: Diese Funktion rechnet einen cm-Wert in einen inch-Wert  *
 *                 um.                                                      *
 ****************************************************************************/

DOUBLE ptCm2Inch( DOUBLE Cm )
{
       return ( (DOUBLE) ( Cm / 2.54 ) );
}

/****************************************************************************
 *   Funktionsname: ptInch2Cm                                                 *
 *                                                                          *
 *   Parameter: DOUBLE : Maßeinheit in inch                                 *
 *                                                                          *
 *   Rückgabe : DOUBLE : Maßeinheit in cm ( 1 Inch = 1 Zoll = 2,54 cm )     *
 *                                                                          *
 *   Beschreibung: Diese Funktion rechnet einen inch-Wert in einen cm-Wert  *
 *                 um.                                                      *
 ****************************************************************************/

DOUBLE ptInch2Cm( DOUBLE Inch )
{
       return ( (DOUBLE) ( Inch * 2.54 ) );
}

/****************************************************************************
 *   Funktionsname: ptPrintAutoNow [USER]                                   *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand                                        *
 *              DOUBLE : Rechter Rand                                       *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) SOFORT aus, ohne ihn im Speicher abzulegen.        *
 ****************************************************************************/

SWORD  ptPrintAutoNow( CHAR *Text, DOUBLE Left, DOUBLE Right, DOUBLE Method )
{

       if ( AktMeasureUnit == PT_CM )
       {
            Left  = ptCm2Inch( Left  );
            Right = ptCm2Inch( Right );
       }
       if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) ) return ( PT_WRONGMARGINS );
       if ( Method == PT_LEFT )
       {
            if ( ptPrintLeft( Text, Left, Right ) < 0 ) return ( PT_PRINTERERROR );
       }
       else
       if ( Method == PT_RIGHT )
       {
            if ( ptPrintCenterOrRight( PT_RIGHT, Text, Left, Right ) < 0 ) return ( PT_PRINTERERROR );
       }
       else
       if ( Method == PT_CENTER )
       {
            if ( ptPrintCenterOrRight( PT_CENTER, Text, Left, Right ) < 0 ) return ( PT_PRINTERERROR );
       }
       else
       if ( Method == PT_JUSTIFY )
       {
            if ( ptPrintJustify( Text, Left, Right ) < 0 ) return ( PT_PRINTERERROR );
       }
       else
       {
            return ( PT_WRONGMETHOD );
       }

       return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptMoveBack [USER]                                       *
 *                                                                          *
 *   Parameter: Word   : Anzahl Zeichen                                     *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, den Druckkopf um eine bestimmte  *
 *                 Anzahl Zeichen zurückzubewegen.                          *
 ****************************************************************************/

SWORD  ptMoveBack ( WORD Chars )
{
       while ( Chars > 0 )
       {
             if ( ptfputc( (char) BackSpace ) == EOF ) return ( PT_PRINTERERROR );
             Chars--;
       }
       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintNow [USER]                                       *
 *                                                                          *
 *   Parameter: Text   : An aktueller Position auszugebender Text           *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, an der aktuellen Druckkopfpos-   *
 *                 ition einen Text auszugeben.                             *
 ****************************************************************************/

SWORD  ptPrintNow ( CHAR *Text )
{
       WORD Zeiger = 0;

       while ( Text[Zeiger] != 0 )
       {
               if ( ptPrintNowChar( Text[Zeiger] ) < 0 ) return ( PT_PRINTERERROR );
               Zeiger++;
       }
       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptDefineUnit [USER]                                     *
 *                                                                          *
 *   Parameter: Word   : PT_CM für cm oder PT_INCH für inch                 *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion setzt die aktuelle Maßeinheit.            *
 ****************************************************************************/

SWORD ptDefineUnit( WORD Unit )
{
       if ( Unit == PT_CM )
       {
            AktMeasureUnit = PT_CM;
            return ( PT_OK );
       }
       else
       if ( Unit == PT_INCH )
       {
            AktMeasureUnit = PT_INCH;
            return ( PT_OK );
       }

       return ( PT_UNITNOTKNOWN );
}

/****************************************************************************
 *   Funktionsname: ptCheckForColumn [USER]                                 *
 *                                                                          *
 *   Parameter: Word   : Nummer der Spalte                                  *
 *                                                                          *
 *   Rückgabe : PT_TRUE (Spalte existiert) oder PT_FALSE (Spalte existiert  *
 *                      nicht)                                              *
 *                                                                          *
 *   Beschreibung: Diese Funktion prüft, ob eine bestimmte Spaltennummer    *
 *                 schon belegt ist.                                        *
 ****************************************************************************/

SWORD ptCheckForColumn ( WORD Num )
{
      WORD HZahl = 0;

      if ( AnzahlColumns == 0 )
      {
           return( PT_FALSE );
      }
      while ( HZahl < AnzahlColumns )
      {
            if ( ( ColumnZeiger + HZahl ) -> CNumber == Num )
            {
                 return ( PT_TRUE );
            }
            HZahl++;
      }

      return ( PT_FALSE );
}

/****************************************************************************
 *   Funktionsname: ptDefineColumn [USER]                                   *
 *                                                                          *
 *   Parameter: Word   : Nummer der Spalte                                  *
 *              DOUBLE : Linke Spalte (in inch !)                           *
 *              DOUBLE : Rechte Spalte (in inch !)                          *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt die Definition einer Spalte. Ob   *
 *                 die Maßeinheit für die Ränder cm oder inch ist, wird     *
 *                 über die Funktion ptDefineUnit (Standard: inch) bestimmt.*
 ****************************************************************************/

SWORD ptDefineColumn ( WORD Num, DOUBLE Left, DOUBLE Right )
{
      COLUMNS *HZeiger;

      if ( Num == 0 || Left >= Right)
      {
           return ( PT_WRONGPARAMETER );
      }

      if ( ptCheckForColumn( Num ) == PT_TRUE )
      {
           return ( PT_ALREADYDEFINED );
      }
      if ( AktMeasureUnit == PT_CM )
      {
           Left  = ptCm2Inch( Left  );
           Right = ptCm2Inch( Right );
      }
      if ( AnzahlColumns == 0 )
      {
           ColumnZeiger = malloc( sizeof( COLUMNS ) );
           if ( ColumnZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           AnzahlColumns = 1;
      }
      else
      {
           HZeiger = ColumnZeiger;
           HZeiger = realloc ( HZeiger, ( AnzahlColumns + 1 ) * sizeof ( COLUMNS ) );
           if ( HZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           ColumnZeiger = HZeiger;
           AnzahlColumns++;
      }
      ( ColumnZeiger + AnzahlColumns - 1 ) -> CNumber = Num;
      ( ColumnZeiger + AnzahlColumns - 1 ) -> Left    = Left;
      ( ColumnZeiger + AnzahlColumns - 1 ) -> Right   = Right;

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptDeleteColumn [USER]                                   *
 *                                                                          *
 *   Parameter: Word   : Nummer der Spalte                                  *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, eine zuvor definierte Spalte zu  *
 *                 löschen.                                                 *
 ****************************************************************************/

SWORD ptDeleteColumn ( WORD Num )
{
      COLUMNS *HZeiger;
      WORD    HColumn,
              NewColumns;

      if ( ptCheckForColumn( Num ) == PT_FALSE || AnzahlColumns == 0 )
      {
           return ( PT_COLUMNNOTDEFINED );
      }
      if ( AnzahlColumns == 1 )
      {
           free ( ColumnZeiger );
           AnzahlColumns = 0;
           return ( PT_OK );
      }
      else
      {
           HZeiger = calloc  ( ( AnzahlColumns - 1 ), sizeof ( COLUMNS ) );
           if ( HZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }
           HColumn = AnzahlColumns;
           AnzahlColumns--;
           NewColumns = 0;
           while ( HColumn > 0 )
           {
                 if ( ( ColumnZeiger + HColumn - 1 ) -> CNumber != Num )
                 {
                      ( HZeiger + NewColumns ) -> CNumber = ( ColumnZeiger + HColumn - 1 ) -> CNumber;
                      ( HZeiger + NewColumns ) -> Left    = ( ColumnZeiger + HColumn - 1 ) -> Left;
                      ( HZeiger + NewColumns ) -> Right   = ( ColumnZeiger + HColumn - 1 ) -> Right;
                      NewColumns++;
                 }
                 HColumn--;
           }
           free( ColumnZeiger );
           ColumnZeiger = HZeiger;
      }

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptGetColumn                                             *
 *                                                                          *
 *   Parameter: WORD   : Nummer der Spalte                                  *
 *              WORD   : PT_LEFT oder PT_RIGHT                              *
 *                                                                          *
 *   Rückgabe : Wert ( in inch ) oder Fehlermeldung ( DOUBLE !! )           *
 *                                                                          *
 *   Beschreibung: Diese Funktion erlaubt, eine zuvor definierte Spalte ab- *
 *                 zurufen.                                                 *
 ****************************************************************************/

DOUBLE ptGetColumn ( WORD Num, WORD Method )
{
      WORD    HColumn;

      if ( ptCheckForColumn( Num ) == PT_FALSE || AnzahlColumns == 0 )
      {
           return ( PT_COLUMNNOTDEFINED );
      }

      HColumn = AnzahlColumns;

      while ( HColumn > 0 && ( ColumnZeiger + HColumn ) -> CNumber != Num )
      {
            HColumn--;
      }

      if ( Method == PT_LEFT )
      {
           return ( ( ColumnZeiger + HColumn ) -> Left );
      }
      else
      if ( Method == PT_RIGHT )
      {
           return ( ( ColumnZeiger + HColumn ) -> Right );
      }

      return ( PT_WRONGMETHOD );
}

/****************************************************************************
 *   Funktionsname: ptPrintBetweenColumnsNow [USER]                         *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linke Spaltennummer ( Column )                     *
 *              DOUBLE : Rechte Spaltennummer ( Column )                    *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) ZWISCHEN zwei Columns aus, d.h. rechts von der     *
 *                 linken und links von der rechten Column.                 *
 ****************************************************************************/

SWORD  ptPrintBetweenColumnsNow( CHAR *Text, WORD ColLeft, WORD ColRight, DOUBLE Method )
{
       SWORD   Result;
       WORD    OldUnit;

       if ( ptCheckForColumn( ColLeft ) == PT_FALSE || ptCheckForColumn( ColRight) == PT_FALSE )
       {
         return ( PT_WRONGCOLUMN );
       }

       /* Prüfen, ob der Platz zwischen links und rechts ausreichend für */
       /* mindestens 1 Zeichen ist, geschieht in ptPrintAutoNow          */

       OldUnit = ptGetInfo( PT_MEASUREUNIT );
       ptDefineUnit( PT_INCH );

       Result = ptPrintAutoNow( Text, ptGetColumn( ColLeft, PT_RIGHT ) + 1/(DOUBLE) Chars_per_inch,
                ptGetColumn( ColRight, PT_LEFT ) - 1/(DOUBLE) Chars_per_inch, Method );

       ptDefineUnit( OldUnit );

       return( Result);
}

/****************************************************************************
 *   Funktionsname: ptPrintInColumnNow [USER]                               *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *              WORD   : Spaltennummer ( Column )                           *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK Fehlermeldung                                *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) INNERHALB einer Column aus.                        *
 ****************************************************************************/

SWORD  ptPrintInColumnNow( CHAR *Text, WORD Column, WORD Method )
{
       SWORD   Result;
       WORD    OldUnit;

       if ( ptCheckForColumn( Column ) == PT_FALSE )
       {
            return ( PT_WRONGCOLUMN );
       }

       OldUnit = ptGetInfo( PT_MEASUREUNIT );
       ptDefineUnit( PT_INCH );

       Result = ptPrintAutoNow( Text, ptGetColumn( Column, PT_LEFT ), ptGetColumn( Column, PT_RIGHT ), Method );

       ptDefineUnit( OldUnit );

       return( Result );
}

/****************************************************************************
 *   Funktionsname: ptFlush [USER]                                          *
 *                                                                          *
 *   Parameter: keine                                                       *
 *   Rückgabe : SWORD  : PT_OK Fehlermeldung                                *
 *                                                                          *
 *   Beschreibung: Diese Funktion leer den Druckerbuffer über die C-Funktion*
 *                 fflush(). In manchen Fällen ist keine Wirkung bemerkbar. *
 ****************************************************************************/

SWORD  ptFlush( VOID )
{
       if ( fflush( DruckerHandle ) == 0 )
       {
            return ( PT_OK );
       }

       return ( PT_PRINTERERROR );
}

/****************************************************************************
 *   Funktionsname: ptMoveHorPos [USER]                                     *
 *                                                                          *
 *   Parameter: DOUBLE : Position in CM oder INCH                           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion fährt den Druckkopf von jeder Position    *
 *                 zu der übergebenen Stelle.                               *
 ****************************************************************************/

SWORD  ptMoveHorPos( DOUBLE Pos )
{
       if ( AktMeasureUnit == PT_CM )
       {
            if ( ptMoveHeadToHorPosition( ptCm2Inch( Pos ) ) < 0 ) return ( PT_PRINTERERROR );
       }
       else
       {
            if ( ptMoveHeadToHorPosition( Pos ) < 0 ) return ( PT_PRINTERERROR );
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintRightNow [USER]                                  *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf auszugebenden Text                      *
 *           DOUBLE : Position in CM oder INCH                              *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt einen String rechtsbündig ab       *
 *                 Spaltenanfang bis zur angegebenen Position aus.          *
 ****************************************************************************/

SWORD  ptPrintRightNow( CHAR *Text, DOUBLE Pos )
{
       return ( ptPrintAutoNow( Text, 0, Pos, PT_RIGHT ) );
}

/****************************************************************************
 *   Funktionsname: ptPrintLeftNow [USER]                                   *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf auszugebenden Text                      *
 *           DOUBLE : Position in CM oder INCH                              *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt einen String linksbündig  ab       *
 *                 der angegebenen Position bis SPaltenende aus.            *
 ****************************************************************************/

SWORD  ptPrintLeftNow( CHAR *Text, DOUBLE Pos)
{
       if ( AktMeasureUnit == PT_CM )
       {
            return ( ptPrintAutoNow( Text, Pos, SpaltenBreiteCM, PT_LEFT ) );
       }

       return ( ptPrintAutoNow( Text, Pos, ptCm2Inch( SpaltenBreiteCM ), PT_LEFT ) );
}

/****************************************************************************
 *   Funktionsname: ptNextPage [USER]                                       *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion läßt den Drucker die aktuelle Seite       *
 *                 "ausspucken".                                            *
 ****************************************************************************/

SWORD  ptNextPage( VOID )
{
       ActiveLine++;

       while ( ActiveLine > 0 )
       {
             ptSkip( 1 );
       }

       ActiveLine = 0;
       PagesPrinted++;

       if ( ptSkipExact( 1 ) < 0 ) return ( PT_PRINTERERROR );

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptStringLen [USER]                                      *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Text                                    *
 *                                                                          *
 *   Rückgabe : DOUBLE : StringLänge in CM oder INCH, je nach aktivierter   *
 *                       Einheit.                                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion berechnet die Länge eines Strings auf dem *
 *                 Papier ( inc Cm oder Inch ).                             *
 ****************************************************************************/

DOUBLE ptStringLen( CHAR *Text  )
{

       if ( AktMeasureUnit == PT_INCH )
       {
            return ( (DOUBLE) ( strlen( Text ) * 1/(DOUBLE) Chars_per_inch ) );
       }
       else
       {
            return ( (DOUBLE) ptInch2Cm ( (DOUBLE) ( strlen( Text ) * 1/(DOUBLE) Chars_per_inch ) ) );
       }
}

/****************************************************************************
 *   Funktionsname: ptSplitText [USER]                                      *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Zielspeicher                            *
 *              CHAR*  : Zeiger auf Ausgangstext                            *
 *              DOUBLE : Linker Rand     in cm oder Inch, je nach bestimmter*
 *              DOUBLE : Rechter Rand    Einheit                            *
 *                                                                          *
 *   Rückgabe : CHAR*  : Zeiger auf Rest, bei Fehler: NULL-Zeiger           *
 *                                                                          *
 *   Beschreibung: Diese Funktion kopiert aus dem Ausgangstext soviele      *
 *                 Zeichen in den Zielspeicher, wie in zwischen die ange-   *
 *                 gebenen Grenzen passen. Die Funktion gibt einen Zeiger   *
 *                 auf die restlichen Zeichen im Ausgangstext zurück.       *
 ****************************************************************************/

CHAR* ptSplitText( CHAR *ZielText, CHAR *OrigText, DOUBLE Left, DOUBLE Right  )
{

      WORD AnzChr,
           HZahl;

      if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) )
      {
           return ( NULL );
      }

      if ( AktMeasureUnit == PT_CM )
      {
           AnzChr = (WORD) ( ptCm2Inch( Right - Left ) * (DOUBLE) Chars_per_inch );
      }
      else
      {
           AnzChr = (WORD) ( Right - Left ) * (DOUBLE) Chars_per_inch;
      }

      HZahl = 0;

      while ( HZahl < AnzChr && *( OrigText + HZahl ) != 0 )
      {
              *( ZielText + HZahl ) = *( OrigText + HZahl );
              HZahl++;
      }

      *( ZielText + HZahl ) = 0;

      return ( OrigText + HZahl );
}

/****************************************************************************
 *   Funktionsname: ptChooseFont [USER]                                     *
 *                                                                          *
 *   Parameter: Fontnummer                                                  *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Aktivieren eines neuen Fonts.                            *
 ****************************************************************************/

SWORD ptChooseFont ( WORD FontNr )
{

      if ( FontNr > ( Fonts - 1 ) ) return( PT_WRONGPARAMETER );

      if ( ptSendSequenz( ( FontZeiger + FontNr ) -> InitCode ) < 0 ) return ( PT_PRINTERERROR );

      SetFont = AktFont = FontNr;
      Chars_per_inch = ( FontZeiger + FontNr ) -> Chars_per_inch;

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptSetSequenz [USER]                                     *
 *                                                                          *
 *   Parameter: BOOL   : Fettdruck an/aus ( PT_TRUE/PT_FALSE )              *
 *              BOOL   : Unterstreichen an/aus ( PT_TRUE/PT_FALSE )         *
 *              BOOL   : Kursivdruck an/aus ( PT_TRUE/PT_FALSE )            *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion korrigiert, falls notwendig, die einzelnen*
 *                 Schriftcharacteristica.                                  *
 ****************************************************************************/

SWORD ptSetSequenz ( BOOL Bold, BOOL Underline, BOOL Italic )
{
      if ( Bold == PT_TRUE && ActiveBold == PT_FALSE )
      {
           if ( ptSwitchFontCharacter( PT_BOLD, PT_ON ) < 0 ) return ( PT_PRINTERERROR );
      }
      if ( Bold == PT_FALSE && ActiveBold == PT_TRUE )
      {
           if ( ptSwitchFontCharacter( PT_BOLD, PT_OFF ) < 0 ) return ( PT_PRINTERERROR );
      }
      if ( Underline == PT_TRUE && ActiveUnderline == PT_FALSE )
      {
           if ( ptSwitchFontCharacter( PT_UNDERLINE, PT_ON ) < 0 ) return ( PT_PRINTERERROR );
      }
      if ( Underline == PT_FALSE && ActiveUnderline == PT_TRUE )
      {
           if ( ptSwitchFontCharacter( PT_UNDERLINE, PT_OFF ) < 0 ) return ( PT_PRINTERERROR );
      }
      if ( Italic == PT_TRUE && ActiveItalic == PT_FALSE )
      {
           if ( ptSwitchFontCharacter( PT_ITALIC, PT_ON ) < 0 ) return ( PT_PRINTERERROR );
      }
      if ( Italic == PT_FALSE && ActiveItalic == PT_TRUE )
      {
           if ( ptSwitchFontCharacter( PT_ITALIC, PT_OFF ) < 0 ) return ( PT_PRINTERERROR );
      }

      return ( PT_OK );
}

 /***************************************************************************
 *   Funktionsname: ptFontSize [USER]                                       *
 *                                                                          *
 *   Parameter: WORD  : Fontnummer ( 0 bis .. )                             *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Über diese Funktion kann der User die Größe eines be-    *
 *                 stimmten Fonts in Characters per Inch erfahren.          *
 *                 Die Werte sind Druckertreiberspezifisch !                *
 ****************************************************************************/

 SWORD ptFontSize( WORD FontNr )
 {
       if ( FontNr > Fonts ) return( PT_WRONGPARAMETER );

       return( ( FontZeiger + FontNr )->Chars_per_inch );
 }

 /***************************************************************************
 *   Funktionsname: ptChangePort [USER]                                     *
 *                                                                          *
 *   Parameter: CHAR* : Zeiger auf Portname (max. 8 Zeichen)                *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Über diese Funktion kann der User den Ausgangsport der   *
 *                 Druckdaten nach Initialisierung durch "ptInitPrinter"    *
 *                 verändern und dadurch die Druckdaten beispielsweise in   *
 *                 eine Datei umleiten !        *
 ****************************************************************************/

 SWORD ptChangePort( CHAR *PortName )
 {
       if ( strlen( PortName ) > 8 || strlen( PortName ) <= 0 )
            return( PT_WRONGPARAMETER );
       strcpy( Port, PortName );

       return( PT_OK );
 }

 /***************************************************************************
 *   Funktionsname: ptSetLinesPerPage [USER]                                *
 *                                                                          *
 *   Parameter: WORD  : Anzahl Zeilen pro Seite, bei Endlospapier:          *
 *                      PT_NOEND                                            *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Über diese Funktion kann der User die Anzahl Zeilen pro  *
 *                 Seite ( Standardwert: 50 ) festsetzen.                   *
 ****************************************************************************/

 SWORD ptSetLinesPerPage( WORD AnzLines )
 {
       if ( AnzLines > 32000 ) return ( PT_WRONGPARAMETER );

       LinesPerPage = AnzLines;

       return( PT_OK );
 }

 /***************************************************************************
 *   Funktionsname: ptDefineHorLength [USER]                                *
 *                                                                          *
 *   Parameter: DOUBLE: Breite einer Zeile in cm oder inch, je nach         *
 *                      definierter Einheit.                                *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Über diese Funktion kann der User die Breite einer       *
 *                 Standardzeile verändern. Die Angabe der Breite erfolgt   *
 *                 in cm oder inch, je nach definierter Einheit (s.         *
 *                 ptDefineUnit)                 *
 ****************************************************************************/

 SWORD ptDefineHorLength( DOUBLE Breite )
 {
       if ( Breite < 0 ) return ( PT_WRONGPARAMETER );

       if ( AktMeasureUnit == PT_INCH )
            SpaltenBreiteCM = ptInch2Cm( Breite );
       else
            SpaltenBreiteCM = Breite;

       return( PT_OK );
 }


/***************************************************************************
 *                                                                         *
 *  Es folgen: Bufferbefehle. Die übergebenen Daten werden nicht direkt an *
 *  den Drucker weitergeleitet, sondern verbleiben im Speicher, bis sie mit*
 *  ptSendBuffer an den Drucker gesandt werden. Dieses Verfahren bietet    *
 *  zwei Vorteile:                                                         *
 *  1. Die einzelnen Zeilen müssen nicht in der korrekten Reihenfolge an   *
 *     den Drucker gegeben werden. Der Computer sendet alle Zeilen auto-   *
 *     matisch in der Richtigen Reihenfolge an den Drucker. Es ist also    *
 *     möglich, zuerst die letzte und DANACH die erste Zeile einer Seite   *
 *     (im Speicher) zu drucken - der Computer erledigt den Rest.          *
 *  2. Eine einmal im Speicher befindliche Seite kann beliebig oft aus-    *
 *     gedruckt werden. Der Computer "vergißt" die Daten erst mit dem      *
 *     ptEmptyBuffer - Kommando.                                           *
 *                                                                         *
 ***************************************************************************/


/****************************************************************************
 *   Funktionsname: ptGetBufferMemory                                       *
 *                                                                          *
 *   Parameter: WORD   : Anzahl Bytes für Texteintrag                       *
 *                                                                          *
 *   Rückgabe : SWORD  : Nummer des reservierten Buffers oder Fehlermeldung *
 *                                                                          *
 *   Beschreibung: Diese Funktion reserviert ( wenn ausreichend Speicher    *
 *                 vorhanden ) Speicher für einen neuen Buffereintrag und   *
 *                 gibt die aktuelle Buffernummer - 1 ( zur direkten Ver-   *
 *                 wendung ) zurück.                                        *
 ****************************************************************************/

SWORD ptGetBufferMemory ( WORD TextLength )
{
      BUFFER *HZeiger;

      if ( TextLength == 0 )
      {
           return ( PT_WRONGPARAMETER );
      }

      if ( AnzahlBufferEintrage == 0 )
      {
           BufferZeiger = calloc( 1, sizeof( BUFFER ) );
           if ( BufferZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }

           BufferZeiger -> Text = (CHAR*) malloc( TextLength + 1 );

           if ( BufferZeiger -> Text == NULL )
           {
                free( BufferZeiger );
                return ( PT_NOTENOUGHMEMORY );
           }
           AnzahlBufferEintrage = 1;
      }
      else
      {
           HZeiger = BufferZeiger;
           HZeiger = realloc ( HZeiger, ( AnzahlBufferEintrage + 1 ) * sizeof ( BUFFER ) );
           if ( HZeiger == NULL )
           {
                return ( PT_NOTENOUGHMEMORY );
           }

           ( HZeiger + AnzahlBufferEintrage ) -> Text =  (CHAR*) malloc( TextLength + 1 );

           if ( ( HZeiger + AnzahlBufferEintrage ) -> Text == NULL )
           {
                free( HZeiger );
                return ( PT_NOTENOUGHMEMORY );
           }

           BufferZeiger = HZeiger;
           AnzahlBufferEintrage++;
      }

      return ( AnzahlBufferEintrage - 1);
}

/****************************************************************************
 *   Funktionsname: ptWriteToMemoryBuffer                                   *
 *                                                                          *
 *   Parameter: WORD   : Zeile, ab der der Text dargestellt werden soll     *
 *              WORD   : Anzahl "MinVerUnit"s davon nach unten, < 1 Zeile !!*
 *                       1 MinVerUnit = 1 SkipExact (Druckerabhängig !!)    *
 *              WORD   : Anzahl Zeichen, die übernommen werden sollen       *
 *              CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand in inch                                *
 *              DOUBLE : Rechter Rand in inch                               *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : Anzahl übernommener Buchstaben oder Fehlermeldung  *
 *                                                                          *
 *   Beschreibung: Diese Funktion reserviert einen Buffer und schreibt die  *
 *                 übergebenen Daten in den reservierten SPeicherbereich.   *
 ****************************************************************************/

SWORD ptWriteToMemoryBuffer ( WORD Zeile, WORD VerUnits, WORD AnzZeichen, CHAR *Text, DOUBLE Left,
      DOUBLE Right, WORD Method )
{

      SWORD NewBuffer,
            AnzahlWords;
      WORD  AktChar,
            HAktChar;
      CHAR  Zeichen;

      NewBuffer = ptGetBufferMemory( AnzZeichen );
      if ( NewBuffer < 0 ) return ( NewBuffer );

      AnzahlWords = ptAnzahlWorte( Text, Left, Right );

      if ( AnzahlWords == 0 ) AnzahlWords = 1;

      AktChar = 0;

      while ( AnzahlWords > 0 && AktChar < AnzZeichen)
      {
              Zeichen = *( Text + AktChar );
              if ( Zeichen == PT_SPACE ) AnzahlWords--;
              *( ( ( BufferZeiger + NewBuffer ) -> Text ) + AktChar ) = Zeichen;
              AktChar++;
      }

      *( ( ( BufferZeiger + NewBuffer ) -> Text ) + AktChar ) = 0;

      HAktChar = AktChar;

      AktChar--;

      while ( AktChar > 0 && *( ( ( BufferZeiger + NewBuffer ) -> Text ) + AktChar ) == PT_SPACE )
      {
            *( ( ( BufferZeiger + NewBuffer ) -> Text ) + AktChar ) = 0;
            AktChar--;
      }

      if ( ActiveBold == PT_TRUE )
      {
           ( BufferZeiger + NewBuffer ) -> BoldPrint = PT_TRUE;
      }
      else
      {
           ( BufferZeiger + NewBuffer ) -> BoldPrint = PT_FALSE;
      }
      if ( ActiveUnderline == PT_TRUE )
      {
           ( BufferZeiger + NewBuffer ) -> Underline = PT_TRUE;
      }
      else
      {
           ( BufferZeiger + NewBuffer ) -> Underline = PT_FALSE;
      }
      if ( ActiveItalic == PT_TRUE )
      {
           ( BufferZeiger + NewBuffer ) -> Italic = PT_TRUE;
      }
      else
      {
           ( BufferZeiger + NewBuffer ) -> Italic = PT_FALSE;
      }

      ( BufferZeiger + NewBuffer ) -> Ausrichtung = Method;
      ( BufferZeiger + NewBuffer ) -> Line        = Zeile;
      ( BufferZeiger + NewBuffer ) -> ExactVerPos = VerUnits;
      ( BufferZeiger + NewBuffer ) -> Left        = Left;
      ( BufferZeiger + NewBuffer ) -> Right       = Right;
      ( BufferZeiger + NewBuffer ) -> Font        = AktFont;

      return ( HAktChar );
}

/****************************************************************************
 *   Funktionsname: ptPrintAutoBuffer                                       *
 *                                                                          *
 *   Parameter: WORD   : Zeile, ab der der Text dargestellt werden soll     *
 *              WORD   : Anzahl minimaler vertikaler Einheiten nach unten   *
 *                       von dieser Zeile aus ( normal 0 )                  *
 *                       1 Einheit = 1 ptSkipExactStandard = 1/48 Zoll      *
 *              CHAR*  : Zeiger auf Text                                    *
 *              DOUBLE : Linker Rand in inch oder cm                        *
 *              DOUBLE : Rechter Rand in inch oder cm                       *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion übergibt den gegebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) dem Speicher, um mit ptSendBuffer ausgedruckt zu   *
 *                 werden.                                                  *
 *                 Über diese Funktion wird auch ein PCX-Bild im Buffer ge- *
 *                 speichert. Dabei werden die Parameter wie folgt ver-     *
 *                 wendet:                                                  *
 *                 WORD   : s. o.                                           *
 *                 WORD   : s. o.                                           *
 *                 CHAR * : Name des PCX-Files                              *
 *                 DOUBLE : Vertikale Größe in inch                         *
 *                 DOUBLE : Horizontale Größe in inch                       *
 *                 WORD   : Filetyp: PT_PCX oder PT_PCC                     *
 *                 aktueller Font: Hor. Position in inch * 100              *
 ****************************************************************************/

SWORD ptPrintAutoBuffer ( WORD Zeile, WORD VerUnits, CHAR *Text, DOUBLE Left,
      DOUBLE Right, WORD Method )
{
      WORD  Chars_per_Line,
            AktLine,
            CharsCopied;
      SWORD Result           = PT_OK;

      VerUnits = VerUnits * VerticalMoveUnit / 48;  /* ptSkipExactStandard -> ptSkipExcat */

      if ( VerUnits > VerUnitsPerLine )
      {
           Zeile    += (WORD) ( VerUnits / VerUnitsPerLine );
           VerUnits -= (WORD) ( ( (WORD) ( VerUnits / VerUnitsPerLine ) ) *
           VerUnitsPerLine );
      }

      if ( AktMeasureUnit == PT_CM )
      {
           Right = ptCm2Inch( Right );
           Left  = ptCm2Inch( Left  );
      }

      /* Das Ablegen von PCX/PCC-Image-Daten verlangt besondere Beachtung !*/

      if ( Method == PT_PCX || Method == PT_PCC )
      {
           if ( ( Result = ptWriteToMemoryBuffer( Zeile, VerUnits,
                12, Text, Left, Right, Method ) ) < 0 )
                return( Result);
           else
                return( PT_OK );
      }

      if ( (Right - Left ) < (DOUBLE) ( 1/(DOUBLE) Chars_per_inch ) )
      {
           return ( PT_WRONGPARAMETER );
      }

      Chars_per_Line = ( Right - Left ) * (DOUBLE) Chars_per_inch;

      AktLine = 0;
      CharsCopied = 0;

      while ( strlen( Text+CharsCopied ) > Chars_per_Line )
      {
              Result = ptWriteToMemoryBuffer( Zeile + AktLine, VerUnits,
                       Chars_per_Line, Text + CharsCopied, Left, Right, Method );
              if ( Result < 0 ) return ( Result );
              AktLine++;
              CharsCopied += Result;
      }

      if ( strlen( Text+CharsCopied ) > 0 )
      {
           Result = ptWriteToMemoryBuffer( Zeile + AktLine, VerUnits, strlen( Text+CharsCopied ),
                    Text + CharsCopied, Left, Right, Method );
           if ( Result < 0 ) return ( Result );
      }

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintBufferWithLine                                   *
 *                                                                          *
 *   Parameter: WORD   : Zeile                                              *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt alle im Speicher befindlichen Daten  *
 *                 einer Zeile an den Drucker aus. Dabei werden die einzel- *
 *                 nen "Minizeilen" nach ihrem ptSkipExact-Wert sortiert.   *
 ****************************************************************************/

SWORD ptPrintBufferWithLine ( WORD XZeile )
{

      SWORD LastUnit;
      WORD  AktUnit,
            TakeBuffer,
            UnitsToSkip,
            AktBuffer      = 0;
      BOOL  NoMoreExacts   = PT_FALSE,
            NewUnit        = PT_FALSE;

      LastUnit    = PT_NOUNITCHOSEN;
      UnitsToSkip = VerUnitsPerLine;

      while ( NoMoreExacts == PT_FALSE )
      {
            AktBuffer = 0;
            AktUnit   = VerUnitsPerLine + 1;
            while ( AktBuffer < AnzahlBufferEintrage )
            {
                  if ( ( ( BufferZeiger + AktBuffer ) -> Line == XZeile ) &&
                       ( ( BufferZeiger + AktBuffer ) -> ExactVerPos < AktUnit ) &&
                       ( ( BufferZeiger + AktBuffer ) -> ExactVerPos > LastUnit ) )
                  {
                       AktUnit   = ( BufferZeiger + AktBuffer ) -> ExactVerPos;
                       AktBuffer = 0;
                  }
                  else
                  {
                       AktBuffer++;
                  }
            }

            if ( AktUnit == VerUnitsPerLine + 1 )
            {
                 NoMoreExacts = PT_TRUE;
            }

            NewUnit = PT_TRUE;
            AktBuffer = 0;
            while ( AktBuffer < AnzahlBufferEintrage && NoMoreExacts == PT_FALSE )
            {
                    if ( ( BufferZeiger + AktBuffer ) -> Line == XZeile &&
                         ( BufferZeiger + AktBuffer ) -> ExactVerPos == AktUnit )
                    {
                           TakeBuffer = AktBuffer;

                           if ( AktUnit <= VerUnitsPerLine )
                           {
                                if ( LastUnit != PT_NOUNITCHOSEN && NewUnit == PT_TRUE )
                                {
                                     if ( ptSkipExact( AktUnit-LastUnit ) < 0 ) return ( PT_PRINTERERROR );
                                     UnitsToSkip -= AktUnit - LastUnit;
                                }
                                else
                                {
                                     if ( ( BufferZeiger + TakeBuffer ) -> ExactVerPos > 0 &&
                                          NewUnit == PT_TRUE )
                                     {
                                          if ( ptSkipExact( ( BufferZeiger + TakeBuffer ) -> ExactVerPos )
                                               < 0 ) return ( PT_PRINTERERROR );
                                          UnitsToSkip -= ( BufferZeiger + TakeBuffer ) -> ExactVerPos;
                                     }
                                }
                                if ( ptSetSequenz(  ( BufferZeiger + TakeBuffer ) -> BoldPrint,
                                                    ( BufferZeiger + TakeBuffer ) -> Underline,
                                                    ( BufferZeiger + TakeBuffer ) -> Italic ) < 0 )
                                                    return ( PT_PRINTERERROR );

                                if ( ( BufferZeiger + TakeBuffer ) -> Font != SetFont )
                                {
                                     if ( ptChooseFont( ( BufferZeiger + TakeBuffer ) -> Font ) < 0 )
                                     return ( PT_PRINTERERROR );
                                }

                                if ( ptPrintAutoNow( ( BufferZeiger + TakeBuffer ) -> Text,
                                                     ( BufferZeiger + TakeBuffer ) -> Left,
                                                     ( BufferZeiger + TakeBuffer ) -> Right,
                                                     ( BufferZeiger + TakeBuffer ) -> Ausrichtung
                                                     ) < 0 ) return ( PT_PRINTERERROR );
                                LastUnit = AktUnit;
                           }
                    }
                    AktBuffer++;
                    NewUnit = PT_FALSE;
            }
      }

      if ( UnitsToSkip != VerUnitsPerLine )
      {
           if ( ptSkipExact( UnitsToSkip ) < 0 ) return ( PT_PRINTERERROR );
      }
      else
      {
           if ( ptSkip ( 1 ) < 0 ) return ( PT_PRINTERERROR );
      }

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintBetweenColumnsBuffer [USER]                      *
 *                                                                          *
 *   Parameter: WORD   : Zeile, ab der der Text dargestellt werden soll     *
 *              WORD   : Anzahl minimaler vertikaler Einheiten nach unten   *
 *                       von dieser Zeile aus ( normal 0 )                  *
 *                       (ptSkipExactStandard-Einheiten zu 1/48 Zoll)       *
 *              CHAR*  : Zeiger auf Text                                    *
 *              WORD   : Linke Spaltennummer ( Column )                     *
 *              WORD   : Rechte Spaltennummer ( Column )                    *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) ZWISCHEN zwei Columns aus, d.h. rechts von der     *
 *                 linken und links von der rechten Column.                 *
 *                 Der Text wird dabei zunächst im Speicher aufgearbeitet   *
 *                 und beim Aufruf von ptSendBuffer ausgegeben.             *
 ****************************************************************************/

SWORD  ptPrintBetweenColumnsBuffer( WORD Zeile, WORD VerUnits, CHAR *Text, WORD ColLeft,
       WORD ColRight, WORD Method )
{
               /* ptAutoBuffer verlangt "VerUnits", daher an dieser Stelle */
               /* keine Umberechnung von 1/48'' in min. vert. Einheiten    */

       if ( ptCheckForColumn( ColLeft ) == PT_FALSE || ptCheckForColumn( ColRight) == PT_FALSE )
       {
            return ( PT_WRONGCOLUMN );
       }

       return ( ptPrintAutoBuffer( Zeile, VerUnits, Text, (WORD) ( ptGetColumn( ColLeft, PT_RIGHT ) +
                1/(DOUBLE) Chars_per_inch ), (WORD) ( ptGetColumn( ColRight, PT_LEFT ) -
                1/(DOUBLE) Chars_per_inch ), Method ) );
}

/****************************************************************************
 *   Funktionsname: ptPrintInColumnBuffer [USER]                            *
 *                                                                          *
 *   Parameter: WORD   : Zeile, ab der der Text dargestellt werden soll     *
 *              WORD   : Anzahl minimaler vertikaler Einheiten nach unten   *
 *                       von dieser Zeile aus ( normal 0 )                  *
 *                       (ptSkipExactStandard-Einheiten zu 1/48 Zoll)       *
 *              CHAR*  : Zeiger auf Text                                    *
 *              WORD   : Spaltennummer ( Column )                           *
 *              WORD   : PT_LEFT, PT_RIGHT, PT_CENTER, PT_JUSTIFY           *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion druckt den übergebenen Text in der gewün- *
 *                 schten Art (linksbündig, rechtsbündig, zentriert, Block- *
 *                 satz) INNERHALB einer Column aus.                        *
 *                 Der Text wird dabei zunächst im Speicher aufgearbeitet   *
 *                 und beim Aufruf von ptSendBuffer ausgegeben.             *
 ****************************************************************************/

SWORD  ptPrintInColumnBuffer( WORD Zeile, WORD VerUnits, CHAR *Text, WORD Column, WORD Method )
{
       if ( ptCheckForColumn( Column ) == PT_FALSE )
       {
            return ( PT_WRONGCOLUMN );
       }

               /* ptAutoBuffer verlangt "VerUnits", daher an dieser Stelle */
               /* keine Umberechnung von 1/48'' in min. vert. Einheiten    */

       return ( ptPrintAutoBuffer( Zeile, VerUnits, Text, ptGetColumn( Column, PT_LEFT ),
                ptGetColumn( Column, PT_RIGHT ), Method ) );
}


/****************************************************************************
 *   Funktionsname: ptSendBuffer [BUFFER]                                   *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt alle im Speicher befindlichen Daten  *
 *                 ( soweit vorhanden ) an den Drucker aus.                 *
 ****************************************************************************/


SWORD ptSendBuffer ( VOID )
{

       WORD  AktZeile,
             AktBuffer,
             OldUnit;
       BOOL  NewLineDone,
             Old_ActiveSpecialChars;

       if ( AnzahlBufferEintrage == 0 )
       {
            return ( PT_NOBUFFERENTRIES );
       }

       OldUnit = ptGetInfo( PT_MEASUREUNIT );
       ptDefineUnit( PT_INCH );

       Old_ActiveSpecialChars = ActiveSpecialChars;
       if ( ptSetSpecialChar( PT_OFF ) < 0 )
       {
            ptDefineUnit( OldUnit );
            return ( PT_PRINTERERROR );
       }

       if ( ptResetPrinter() < 0 )
       {
            ptDefineUnit( OldUnit );
            return ( PT_PRINTERERROR );
       }
       if ( ptSetSequenz( PT_FALSE, PT_FALSE, PT_FALSE ) < 0 )
       {
            ptDefineUnit( OldUnit );
            return ( PT_PRINTERERROR );
       }
       AktZeile = 1;

       if ( VerMoveKind == PT_HP )
       {
           if ( ptSkipExact( 1 ) < 0 )
           {
                ptDefineUnit( OldUnit );
                return( PT_PRINTERERROR );
           }
           if ( ptSkipExact( VerUnitsPerLine - 1 ) < 0 )
           {
                ptDefineUnit( OldUnit );
                return ( PT_PRINTERERROR );
           }
       }

       while ( AktZeile <= LinesPerPage )
       {
               AktBuffer = 0;
               NewLineDone = PT_FALSE;
               while ( AktBuffer < AnzahlBufferEintrage && NewLineDone == PT_FALSE)
               {
                       if ( ( BufferZeiger + AktBuffer ) -> Line == AktZeile )
                       {
                            if ( ptPrintBufferWithLine ( AktZeile ) < 0 )
                            {
                                 ptDefineUnit( OldUnit );
                                 return ( PT_PRINTERERROR );
                            }
                            NewLineDone = PT_TRUE;
                       }
                       AktBuffer++;
               }
               AktZeile++;
               if ( NewLineDone == PT_FALSE && AktLine < LinesPerPage )
               {
                    if ( ptSkip ( 1 ) < 0 )
                    {
                         ptDefineUnit( OldUnit );
                         return ( PT_PRINTERERROR );
                    }
               }
       }

       ptDefineUnit( OldUnit );

       if ( Old_ActiveSpecialChars == PT_TRUE )
       {
            if ( ptSetSpecialChar( PT_ON ) < 0 ) return ( PT_PRINTERERROR );
       }

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptEmptyBuffer [USER]                                    *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK                                              *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt den Speicher frei, der von           *
 *                 ptGetBufferMemory für die speicherinterne Druckdatenver- *
 *                 waltung belegt wurde. Wurde kein Speicher belegt,        *
 *                 passiert nichts.                                         *
 ****************************************************************************/

SWORD ptEmptyBuffer ( VOID )
{

      WORD HZahl;

      if ( AnzahlBufferEintrage == 0 )
      {
           return ( PT_OK );
      }

      for (HZahl=0; HZahl<AnzahlBufferEintrage; HZahl++)
      {
           free ( ( BufferZeiger + HZahl ) -> Text );
      }

      free ( BufferZeiger );

      return ( PT_OK );
}

/****************************************************************************
 *   Die folgenden Routinen werden zum Laden von Treibern benötigt !!!      *
 ****************************************************************************/

/****************************************************************************
 *   Funktionsname: ptClearLoadedData                                       *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : keine                                                       *
 *                                                                          *
 *   Beschreibung: Diese Funktion löscht alle im Speicher befindlichen      *
 *                 druckerspezifischen Daten, welche über eine Druckerdatei *
 *                 eingeladen wurden.                                       *
 ****************************************************************************/

VOID ptClearLoadedData( VOID )
{
      if ( PreVerCode    != NULL )
      {
           free  ( PreVerCode    );
           PreVerCode = NULL;
      }
      if ( AftVerCode    != NULL )
      {
           free  ( AftVerCode    );
           AftVerCode = NULL;
      }
      if ( Reset         != NULL )
      {
           free  ( Reset         );
           Reset = NULL;
      }
      if ( BoldOn        != NULL )
      {
           free  ( BoldOn        );
           BoldOn = NULL;
      }
      if ( BoldOff       != NULL )
      {
           free  ( BoldOff       );
           BoldOff = NULL;
      }
      if ( UnderlineOn   != NULL )
      {
           free  ( UnderlineOn   );
           UnderlineOn = NULL;
      }
      if ( UnderlineOff  != NULL )
      {
           free  ( UnderlineOff  );
           UnderlineOff = NULL;
      }
      if ( ItalicOn      != NULL )
      {
           free  ( ItalicOn      );
           ItalicOn = NULL;
      }
      if ( ItalicOff     != NULL )
      {
           free  ( ItalicOff     );
           ItalicOff = NULL;
      }
      if ( DruckerHandle != NULL )
      {
           fclose( DruckerHandle );
           DruckerHandle = NULL;
      }
      if ( Datei         != NULL )
      {
           fclose( Datei         );
           Datei = NULL;
      }
}

/****************************************************************************
 *   Funktionsname: ptLoadCodes                                             *
 *                                                                          *
 *   Parameter: CHAR** : Doppelzeiger auf Speicher für Zieldaten            *
 *              FILE** : Zeiger auf Dateihandle                             *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt Druckersequenzen.                    *
 ****************************************************************************/

SWORD ptLoadCodes ( CHAR **Zeiger, FILE **Datei )
{
     BYTE  Zahl2, Zahl;
     SWORD Loaded;

     Zahl2 = fgetc( *Datei );
     *Zeiger = malloc ( Zahl2 + 1 );
     if ( *Zeiger == NULL ) return ( PT_NOTENOUGHMEMORY );
     **Zeiger = Zahl2;
     if ( Zahl2 > 0 )
          for( Zahl=1; Zahl<=Zahl2; Zahl++)
          {
               if ( ( Loaded = fgetc( *Datei ) ) == EOF ) return ( PT_FILEERROR );
               *(*Zeiger + Zahl) = (CHAR) Loaded;
          }
     return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptLoadString                                            *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Stringzieladresse                       *
 *              FILE*  : Dateihandle                                        *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen nullterminierten String.       *
 ****************************************************************************/

SWORD ptLoadString ( CHAR *Zeiger, FILE **Datei )
{
     BYTE  Zahl;
     SWORD Zahl2;

     Zahl = 0;
     do
     {
          if ( ( Zahl2 = fgetc( *Datei ) ) == EOF ) return ( PT_FILEERROR );
          Zeiger[Zahl] = (CHAR) Zahl2;
          Zahl++;
     }
     while ( (CHAR) Zahl2 != 0 );

     return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptLoadParameters                                        *
 *                                                                          *
 *   Parameter: FILE** : Doppelzeiger auf Dateihandle                       *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptLoadParameters( FILE **Datei )
{
     SWORD Zahl,
           Zahl2;

     if ( ptLoadString( Port, Datei ) < 0 ) return ( PT_PRINTERERROR );

     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     if ( ( Zahl2 = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );

     VerticalMoveUnit = Zahl * 256 + Zahl2;

     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     if ( ( Zahl2 = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );

     VerUnitsPerLine  = Zahl * 256 + Zahl2;

     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     VerMoveKind = (CHAR) Zahl;
     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     BackSpace = (CHAR) Zahl;
     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     NewPage = (CHAR) Zahl;

     if ( ptLoadCodes( &PreVerCode, Datei )    < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &AftVerCode, Datei )    < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &Reset, Datei )         < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &BoldOn, Datei )        < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &BoldOff, Datei )       < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &UnderlineOn, Datei )   < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &UnderlineOff, Datei )  < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &ItalicOn, Datei )      < 0 ) return ( PT_NOTENOUGHMEMORY );
     if ( ptLoadCodes( &ItalicOff, Datei )     < 0 ) return ( PT_NOTENOUGHMEMORY );

     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return ( PT_PRINTERERROR );
     Fonts = (WORD) Zahl;

     FontZeiger = calloc( Fonts, sizeof( FONTS ) );

     if ( FontZeiger == NULL ) return ( PT_NOTENOUGHMEMORY );

     for( Zahl=1; Zahl<=Fonts; Zahl++)
     {
          if ( ptLoadString( ( FontZeiger + Zahl - 1 ) -> FontName, Datei ) < 0 )
               return ( PT_PRINTERERROR );
          if ( ptLoadCodes( &((FontZeiger + Zahl - 1 ) -> InitCode), Datei ) < 0 )
               return ( PT_NOTENOUGHMEMORY );
          if ( ( Zahl2 = fgetc( *Datei ) ) == EOF ) return ( PT_FILEERROR );
          ( FontZeiger + Zahl - 1 ) -> Chars_per_inch = (CHAR) Zahl2;
     }

     Chars_per_inch  = FontZeiger -> Chars_per_inch;

     if ( fclose( *Datei ) != 0 ) return ( PT_FILEERROR );
     *Datei = NULL;

     return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptInitPrinter [USER]                                    *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Druckername (=Dateiname)                *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptInitPrinter ( CHAR *Drucker )
{
      FILE *Datei;

      if ( DruckerHandle != NULL ) ptClearLoadedData();

      Datei = fopen( Drucker, "rb" );

      if ( Datei == NULL ) return ( PT_FILEERROR );

      ParallelPort = 0;
      SerialPort   = 0;

      strncpy( PrinterName, Drucker, 8 );
      PrinterName[8] = PT_EOS;

      if ( ptLoadParameters( &Datei ) < 0 )
      {
           ptClearLoadedData();
           return( PT_FILEERROR );
      }

      if ( ( DruckerHandle = fopen( Port, "wb" ) ) == NULL ) return ( PT_PRINTERERROR );

      if ( ptSendSequenz( Reset ) < 0 ) return ( PT_PRINTERERROR );
      if ( ptSendSequenz( FontZeiger -> InitCode ) < 0 ) return ( PT_PRINTERERROR );
      if ( ptSkipExact(1) < 0 ) return( PT_PRINTERERROR );

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptScanInitPrinter [USER]                                *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Druckername (=Dateiname)                *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptScanInitPrinter ( CHAR *Drucker )
{
      FILE *Datei;
      WORD count;

      if ( DruckerHandle != NULL ) ptClearLoadedData();

      Datei = fopen( Drucker, "rb" );

      if ( Datei == NULL ) return ( PT_FILEERROR );

      strncpy( PrinterName, Drucker, 8 );
      PrinterName[8] = PT_EOS;

      if ( ptLoadParameters( &Datei ) < 0 )
      {
           ptClearLoadedData();
           return( PT_FILEERROR );
      }

      if ( strlen( Port ) > 0 )
         for ( count=0; count < strlen( Drucker ); count++ )
             Port[count] = toupper( Port[count] );

      ParallelPort = 0;
      SerialPort   = 0;

      if ( strcmp( Port, "LPT1" ) == 0 ) ParallelPort = 1;
      else
      if ( strcmp( Port, "LPT2" ) == 0 ) ParallelPort = 2;
      else
      if ( strcmp( Port, "LPT3" ) == 0 ) ParallelPort = 3;
      else
      if ( strcmp( Port, "COM1" ) == 0 ) SerialPort = 1;
      else
      if ( strcmp( Port, "COM2" ) == 0 ) SerialPort = 2;
      else
      if ( strcmp( Port, "COM3" ) == 0 ) SerialPort = 3;
      else
      if ( ( DruckerHandle = fopen( Port, "wb" ) ) == NULL ) return ( PT_PRINTERERROR );

      if ( ptSendSequenz( Reset ) < 0 ) return ( PT_PRINTERERROR );
      if ( ptSendSequenz( FontZeiger -> InitCode ) < 0 ) return ( PT_PRINTERERROR );
      if ( ptSkipExact(1) < 0 ) return( PT_PRINTERERROR );

      return ( PT_OK );
}



/****************************************************************************
 *   Funktionsname: ptClosePrinter [USER]                                   *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : PT_OK                                                       *
 *                                                                          *
 *   Beschreibung: Diese Funktion löscht jeglichen von dieser Toolbox       *
 *                 allokierten Speicher und schließt alle Streams.          *
 ****************************************************************************/

 SWORD ptClosePrinter( VOID )
 {
       WORD HZahl;

       ptClearLoadedData();
       ptEmptyBuffer();

       ParallelPort = 0;
       SerialPort   = 0;

       if ( ColumnZeiger != NULL )
       {
            free( ColumnZeiger );
            AnzahlColumns = 0;
            ColumnZeiger  = NULL;
       }
       if ( TrapZeiger != NULL )
       {
            free( TrapZeiger );
            AnzahlTraps = 0;
            TrapZeiger  = NULL;
       }
       if ( FontZeiger != NULL )
       {
            for ( HZahl=1; HZahl<=Fonts; HZahl++)
                  free( ( FontZeiger + HZahl - 1 )->InitCode );
            free( FontZeiger );
            Fonts = 0;
            FontZeiger = NULL;
       }

       return ( PT_OK );
 }

