/*---------------------------------------------------------------------------

                     PCX - Dateien auf einem EPSON LX/FX

                          (c) 1992/93 by Markus Mück

-----------------------------------------------------------------------------*/

BYTE     PrinterType   = 0;               /* Art des Bewegungscodes: PT_HP oder PT_ESC.. */
CHAR     *NoLineSpace  = NULL;            /* [EPSON] Code für Zeilenabstand 7/72 ''      */
CHAR     *NewLine      = NULL;            /* [EPSON] Code für neue Zeile                 */
CHAR     *GraphicOn    = NULL;            /* [EPSON,HP] Code für 'Graphikmodus ein'      */
CHAR     *LineSpace    = NULL;            /* [EPSON] Code für normalen Zeilenabstand     */
WORD     Resolution    = 0;               /* [EPSON,HP] Horizontale Auflösung der Graphik*/
WORD     YResolution   = 0;               /* [EPSON,HP] Vertikale Auflösung der Grafik   */
CHAR     *PreDPIRes    = NULL;            /* [HP] Code VOR Druckauflösung in DPI         */
CHAR     *AftDPIRes    = NULL;            /* [HP] Code NACH Druckauflösung in DPI        */
CHAR     *PreBytes     = NULL;            /* [HP] Code VOR Anzahl Graphicbytes           */
CHAR     *AftBytes     = NULL;            /* [HP] Code NACH Anzahl Graphicbytes          */
CHAR     *CodeEnd      = NULL;            /* [HP] Code für Graphic-Ende                  */
BYTE     *Zeile[25][5];                   /* Zeiger auf Zeile 1 bis 8/Plane 1-4          */
WORD     AuflosungHor;                    /* Horizontale Größe eines Bildes in Pixel     */
WORD     AuflosungVer;                    /* Vertikale Größe eines Bildes in Pixel       */
FILE     *FileHandle;                     /* Handle für Bilddatei                        */
BYTE     Planes;                          /* Anzahl Planes                               */
BYTE     BitProPixel;                     /* Anzahl Bits pro Pixel (EGA: 1, CGA: 2)      */
BYTE     PCXAnzahl;                       /* Variable für interne Verwendung             */
BYTE     PCXWert;                         /* Variable für interne Verwendung             */

/* Druckmuster für PCX-Graphiken, Farbe 1 bis 16 */

BYTE     DruckMuster[16][8]  = { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
                                 { 0x81, 0x42, 0x24, 0x10, 0x08, 0x24, 0x42, 0x81 },
                                 { 0x30, 0x48, 0x48, 0x30, 0x0C, 0x12, 0x12, 0x0C },
                                 { 0x18, 0x7E, 0x62, 0xD3, 0xCB, 0x46, 0x7E, 0x18 },
                                 { 0x30, 0x48, 0x84, 0x82, 0x41, 0x21, 0x12, 0x0C },
                                 { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 },
                                 { 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF },
                                 { 0x42, 0xA5, 0x42, 0x18, 0x18, 0x42, 0xA5, 0x42 },
                                 { 0x49, 0x92, 0x24, 0x49, 0x92, 0x24, 0x49, 0x92 },
                                 { 0x92, 0x49, 0x24, 0x92, 0x49, 0x24, 0x92, 0x49 },
                                 { 0x00, 0x66, 0x66, 0x00, 0x66, 0x66, 0x00, 0x00 },
                                 { 0x24, 0x18, 0x24, 0x42, 0x81, 0x42, 0x24, 0x18 },
                                 { 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF },
                                 { 0x00, 0x49, 0x00, 0x22, 0x00, 0x49, 0x00, 0x22 },
                                 { 0x92, 0x00, 0x29, 0x80, 0x12, 0x40, 0x08, 0xA1 },
                                 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
                               };

/****************************************************************************
 *   Funktionsname: ptClearLoadedPCXData                                    *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : keine                                                       *
 *                                                                          *
 *   Beschreibung: Diese Funktion löscht alle im Speicher befindlichen      *
 *                 druckerspezifischen Daten, welche über eine Druckerdatei *
 *                 eingeladen wurden.                                       *
 ****************************************************************************/

VOID ptClearLoadedPCXData( VOID )
{
      if ( NoLineSpace   != NULL )
      {
           free  ( NoLineSpace   );
           NoLineSpace = NULL;
      }
      if ( NewLine       != NULL )
      {
           free  ( NewLine       );
           NewLine = NULL;
      }
      if ( GraphicOn     != NULL )
      {
           free  ( GraphicOn     );
           GraphicOn = NULL;
      }
      if ( LineSpace     != NULL )
      {
           free  ( LineSpace     );
           LineSpace = NULL;
      }
      if ( PreDPIRes     != NULL )
      {
           free  ( PreDPIRes     );
           PreDPIRes = NULL;
      }
      if ( AftDPIRes     != NULL )
      {
           free  ( AftDPIRes     );
           AftDPIRes = NULL;
      }
      if ( PreBytes      != NULL )
      {
           free  ( PreBytes      );
           PreBytes = NULL;
      }
      if ( AftBytes      != NULL )
      {
           free  ( AftBytes      );
           AftBytes = NULL;
      }
      if ( CodeEnd       != NULL )
      {
           free  ( CodeEnd       );
           CodeEnd = NULL;
      }

      if ( FileHandle    != NULL )
      {
           fclose( FileHandle );
           FileHandle = NULL;
      }
}

/****************************************************************************
 *   Funktionsname: ptCloseFileAndError                                     *
 *                                                                          *
 *   Parameter: SWORD  : FehlerNummer                                       *
 *                                                                          *
 *   Rückgabe : SWORD  : Obige Fehlernummer                                 *
 *                                                                          *
 *   Beschreibung: Diese Funktion schließt eine geöffnete PCX/PCC-Datei und *
 *                 gibt die übergebene Fehlernummer wieder zurück.          *
 *                 Verwendungsbeispiel:                                     *
 *                 if ( fputc( 'c', Handle ) == EOF )                       *
 *                      return( CloseFileAndError ( PT_FILEERROR ) );       *
 ****************************************************************************/

 SWORD ptCloseFileAndError ( SWORD ErrorNum )
 {
       ptClearLoadedPCXData();
       return( ErrorNum );
 }

 /****************************************************************************
 *   Funktionsname: ptSendPCXSequenz                                        *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Sequenzbeginn                           *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion sendet eine Druckersequenz.               *
 ****************************************************************************/

SWORD  ptSendPCXSequenz ( CHAR *Start )
{
       BYTE  HZahl;

       for( HZahl=1; HZahl<=*Start; HZahl++)
       {
            if ( ptfputc( *(Start+HZahl) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
       }
       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptOpenFile                                              *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Dateiname einer PCX- oder PCC-Datei     *
 *                                                                          *
 *   Rückgabe : SWORD  : File-Fehlermeldungen oder PT_OK                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion öfnnet eine PCX- oder PCC-Datei und liest *
 *                 den Kopf ein.                                             *
 ****************************************************************************/

SWORD ptOpenFile ( CHAR *DateiName )
{
     SWORD HZahl,
           Zeichen;

     PCXAnzahl = 0;
     PCXWert   = 0;

     if ( DruckerHandle == NULL ) DruckerHandle = fopen ( Port, "wb" );
     if ( DruckerHandle == NULL ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

     FileHandle = fopen ( DateiName, "rb" );
     if ( FileHandle == NULL ) return( ptCloseFileAndError( PT_FILEERROR ) );

     Zeichen = fgetc( FileHandle );
     if ( Zeichen != PT_PCXKENNUNG ) return ( ptCloseFileAndError( PT_NOPCXFILE ) );

     if ( ( ( Zeichen = fgetc( FileHandle ) ) == EOF ) ||
          ( ( Zeichen = fgetc( FileHandle ) ) == EOF ) ||
          ( ( Zeichen = fgetc( FileHandle ) ) == EOF ) )
          return ( ptCloseFileAndError( PT_FILEERROR ) );
     if ( Zeichen == 1 || Zeichen == 4)
     {
          BitProPixel = 1;
     }
     else
     {
          BitProPixel = 2;
     }

     for (HZahl=1; HZahl<=6; HZahl++)
          if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
                 return ( ptCloseFileAndError ( PT_PRINTERERROR ) );

     if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
            return ( ptCloseFileAndError ( PT_PRINTERERROR ) );
     AuflosungVer = (WORD) Zeichen;

     if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
            return ( ptCloseFileAndError ( PT_PRINTERERROR ) );
     AuflosungVer = AuflosungVer + (WORD) Zeichen*256;

     for (HZahl=1; HZahl<=53; HZahl++)
          if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
                 return ( ptCloseFileAndError ( PT_PRINTERERROR ) );

     if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
            return ( ptCloseFileAndError ( PT_PRINTERERROR ) );
     Planes = Zeichen;

     if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
            return ( ptCloseFileAndError ( PT_PRINTERERROR ) );
     AuflosungHor = (WORD) Zeichen;

     if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
            return ( ptCloseFileAndError ( PT_PRINTERERROR ) );
     AuflosungHor = AuflosungHor + (WORD) Zeichen*256;

     AuflosungHor *= 8;

     for (HZahl=1; HZahl<=60; HZahl++)
          if ( ( Zeichen = fgetc( FileHandle ) ) == EOF )
                 return ( ptCloseFileAndError ( PT_PRINTERERROR ) );

     return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptGetMemory                                             *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : SWORD  : Speicher -Fehlermeldungen oder PT_OK               *
 *                                                                          *
 *   Beschreibung: Diese Funktion versucht, Speicher für die Konvertierung  *
 *                 PCX->Drucker zu belegen.                                 *
 ****************************************************************************/

SWORD ptGetMemory( VOID )
{
      SWORD HZahl1,
            HZahl2;
      SWORD Ergebnis = PT_OK;

      for (HZahl1=1; HZahl1<=24; HZahl1++)
      {
           for (HZahl2=1; HZahl2<=Planes; HZahl2++)
           {
                Zeile[HZahl1][HZahl2] = (BYTE*) calloc( AuflosungHor*BitProPixel/8, sizeof ( BYTE ) );
                if ( Zeile[HZahl1][HZahl2] == NULL ) Ergebnis = PT_ERROR;
           }
      }

      if ( Ergebnis == PT_ERROR )
      {
           for (HZahl1=1; HZahl1<=8; HZahl1++)
           {
                for (HZahl2=1; HZahl2<=Planes; HZahl2++)
                {
                     if ( Zeile[HZahl1][HZahl2] != NULL ) free ( Zeile[HZahl1][HZahl2] );
                }
           }
      }

      return ( Ergebnis );
}

/****************************************************************************
 *   Funktionsname: ptCloseFilesAndFreeMemory                               *
 *                                                                          *
 *   Parameter: keine                                                       *
 *                                                                          *
 *   Rückgabe : kein                                                        *
 *                                                                          *
 *   Beschreibung: Diese Funktion schließt alle von dieser Unit geöffneten  *
 *                 Dateien und gibt den Speicher wieder frei.               *
 ****************************************************************************/

VOID  ptCloseFilesAndFreeMemory( VOID )
{
      SWORD HZahl1,
            HZahl2;

      for (HZahl1=1; HZahl1<=24; HZahl1++)
      {
           for (HZahl2=1; HZahl2<=Planes; HZahl2++)
           {
                if ( Zeile[HZahl1][HZahl2] != NULL )
                {
                     free ( Zeile[HZahl1][HZahl2] );
                     Zeile[HZahl1][HZahl2] = NULL;
                }
           }
      }

      fclose( FileHandle );
      FileHandle = NULL;
}

/****************************************************************************
 *   Funktionsname: ptLoadZeilen                                            *
 *                                                                          *
 *   Parameter: MaxZeilen = Zum künstlichen Dehnen von Bildern ( gibt die   *
 *              Anzahl Linien an, die von Disk gelesen werden und mit       *
 *              denen 24 Zeilen aufgefüllt werden ).                        *
 *                                                                          *
 *   Rückgabe : File-Fehlermeldungen oder PT_OK                             *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt die nächsten  Zeilen Graphik von     *
 *                 Disk.                                                    *
 ****************************************************************************/

SWORD ptLoadZeilen( WORD MaxZeilen )
{
      WORD    Plane,
              Byte,
              AktZeile,
              AnzahlNeueZeilen = 0,
              CopyLine;
      SWORD   Zeichen,
              Zeichen2 = 1;
      DOUBLE  EinfugZeile,
              UbergehWert;


      EinfugZeile = (DOUBLE) 24 / MaxZeilen;
      UbergehWert = (DOUBLE) MaxZeilen / 24;
      AktZeile    = 1;

      while ( AktZeile <= 24 && AnzahlNeueZeilen < MaxZeilen )
      {
              AnzahlNeueZeilen++;
              for (Plane=1; Plane<=Planes; Plane++)
              {
                   Byte =  1;
                   while ( Byte <= AuflosungHor*BitProPixel/8 )
                   {
                         if ( PCXAnzahl == 0 )
                         {
                              Zeichen = (SWORD) fgetc( FileHandle );

                              if ( Zeichen == EOF )
                              {
                                   while ( AktZeile <= 24 )
                                   {
                                         for (Plane=1; Plane <= Planes; Plane++)
                                         {
                                              memset( Zeile[AktZeile][Plane], 0xff,
                                                      AuflosungHor*BitProPixel/8 );
                                         }
                                         AktZeile++;
                                   }
                                   return ( AnzahlNeueZeilen );
                              }

                              if ( ( Zeichen & 0x00C0 ) == 0x00C0 )
                              {
                                   if ( ( Zeichen2  = (SWORD) fgetc( FileHandle ) ) == EOF )
                                          return ( ptCloseFileAndError ( PT_FILEERROR ) );

                                   PCXAnzahl = (BYTE) Zeichen & 0x003F;
                                   PCXWert   = (BYTE) Zeichen2;
                              }
                              else
                              {
                                   PCXAnzahl = 1;
                                   PCXWert   = (BYTE) Zeichen;
                              }
                         }
                         while ( PCXAnzahl > 0 && Byte <= AuflosungHor*BitProPixel/8 )
                         {
                               *(Zeile[AktZeile][Plane]+Byte-1) = (BYTE) PCXWert;

                               Byte++;
                               PCXAnzahl--;
                         }
                   }
                   if ( Zeichen == EOF || Zeichen2 == EOF) break;
              }

              while ( EinfugZeile > AktZeile && UbergehWert < AktZeile )
              {
                    for (Plane=1; Plane<=Planes; Plane++)
                    {
                         if ( AktZeile < 24 )
                              memcpy( Zeile[AktZeile+1][Plane], Zeile[AktZeile][Plane],
                                      AuflosungHor*BitProPixel/8 );

                    }
                    AktZeile++;
              }

              if ( Zeichen == EOF || Zeichen2 == EOF)
              {
                   while ( AktZeile <= 24 )
                   {
                           for (Plane=1; Plane <= Planes; Plane++)
                           {
                                memset( Zeile[AktZeile][Plane], 0xff, AuflosungHor*BitProPixel/8 );
                           }
                           AktZeile++;
                   }
                   return ( AnzahlNeueZeilen );
              }
              EinfugZeile += (DOUBLE) 24 / MaxZeilen;
              if ( UbergehWert <= AktZeile )
              {
                   AktZeile++;
                   UbergehWert += (DOUBLE) MaxZeilen / 24;
              }
              else
              {
                   UbergehWert -= (DOUBLE) MaxZeilen / 24;
              }
      }

      while ( MaxZeilen > AnzahlNeueZeilen )
      {
            for (Plane=1; Plane<=Planes; Plane++)
            {
                 Byte = 1;
                 while ( Byte <= AuflosungHor*BitProPixel/8 )
                 {
                       if ( PCXAnzahl == 0 )
                       {
                            /* Auch EOF = 0x1A muß möglich sein, daher */
                            /* keine Überprüfung !                     */

                            Zeichen = fgetc( FileHandle );

                            if ( ( Zeichen & 0x00C0 ) == 0x00C0 )
                            {
                                 /* Auch EOF = 0x1A muß möglich sein, daher */
                                 /* keine Überprüfung !                     */

                                 Zeichen2 = fgetc( FileHandle );

                                 PCXAnzahl = (BYTE) Zeichen & 0x003F;
                                 PCXWert   = (BYTE) Zeichen2;
                            }
                            else
                            {
                                 PCXAnzahl = 1;
                                 PCXWert   = (BYTE) Zeichen;
                            }
                       }
                       while ( PCXAnzahl > 0 && Byte <= AuflosungHor*BitProPixel/8 )
                       {
                             Byte++;
                             PCXAnzahl--;
                       }
                 }
                 if ( Zeichen == EOF || Zeichen2 == EOF)
                      break;
            }
            AnzahlNeueZeilen++;
      }

      CopyLine = AnzahlNeueZeilen;

      while ( AnzahlNeueZeilen < MaxZeilen )
      {
            for (Plane=1; Plane<=Planes; Plane++)
            {
                 if ( AnzahlNeueZeilen < 24 )
                      memcpy( Zeile[AnzahlNeueZeilen+1][Plane], Zeile[CopyLine][Plane],
                              AuflosungHor*BitProPixel/8 );

            }
            AnzahlNeueZeilen++;
      }

      return ( AnzahlNeueZeilen );
}

/****************************************************************************
 *   Funktionsname: ptDetectPixel                                           *
 *                                                                          *
 *   Parameter: AktSpalte : Horizontale Pixelposition in Pixel              *
 *              SktZeile  : Vertikale Spaltenposition ( 1-8 )               *
 *                                                                          *
 *   Rückgabe : Pixelfarbwert oder Fehlermeldung                            *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt die Farbe eines Pixels aus einer der *
 *                 eingelesenen Zeilen                                      *
 ****************************************************************************/

SWORD ptDetectPixel ( WORD AktSpalte, WORD AktZeile )
{
    BYTE PixByte[5],
         PixelByte;
    WORD HorByte,
         ShiftLeft;
    SWORD HZahl;

      if ( Planes == 1 && BitProPixel == 2 )             /* CGA-Auflösung, 4-farbig */
      {
           HorByte   = ( AktSpalte - 1 ) / 4;
           ShiftLeft = ( ( AktSpalte - 1 ) % 4 ) * 2;
           PixelByte = *( Zeile[AktZeile][1]+HorByte ) << ShiftLeft;

           return ( PixelByte >> 6 );
      }
      else
      if ( Planes == 1 && BitProPixel == 1 )             /* CGA-Auflösung, 2-farbig */
      {
           HorByte   = ( AktSpalte - 1 ) / 8;
           ShiftLeft = ( AktSpalte - 1 ) % 8;
           PixelByte = *( Zeile[AktZeile][1]+HorByte ) << ShiftLeft;

           return ( PixelByte >> 7 );
      }
      else
      if ( Planes == 4 && BitProPixel == 1 )             /* EGA-Auflösung, 16-farbig */
      {
           HorByte   = ( AktSpalte - 1 ) / 8;
           ShiftLeft = ( AktSpalte - 1 ) % 8;
           PixByte[1] = *( Zeile[AktZeile][1]+HorByte ) << ShiftLeft;
           PixByte[1] = PixByte[1] >> 7;
           PixByte[2] = *( Zeile[AktZeile][2]+HorByte ) << ShiftLeft;
           PixByte[2] = ( PixByte[2] >> 6 ) & 0xFE;
           PixByte[3] = *( Zeile[AktZeile][3]+HorByte ) << ShiftLeft;
           PixByte[3] = ( PixByte[3] >> 5 ) & 0xFC;
           PixByte[4] = *( Zeile[AktZeile][4]+HorByte ) << ShiftLeft;
           PixByte[4] = ( PixByte[4] >> 4 ) & 0xF8;

           HZahl = (SWORD) ( PixByte[1] | PixByte[2] | PixByte[3] | PixByte[4] );

           return ( HZahl );
      }
      else
      {
           return( ptCloseFileAndError( PT_PCXFORMATNOTSUPPORTED ) );
      }
}

/****************************************************************************
 *   Funktionsname: ptPrintSpalte                                           *
 *                                                                          *
 *   Parameter: AktSpalte : Horizontale Pixelposition in Pixel              *
 *              AktZeile  : Vertikale Byte-Position ( 1-3 )                 *
 *                                                                          *
 *   Rückgabe : Fehlermeldung oder PT_OK                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt die Spalte AKTSPALTE der zulest ge-  *
 *                 ladenen 8 Grafikzeilen auf dem Nadel - Drucker aus.      *
 ****************************************************************************/

SWORD ptPrintSpalte ( WORD AktSpalte, WORD AktZeile )
{
      BYTE   PixelColor,
             PrinterByte = 0;
      WORD   HZahl;
      SWORD  AktPixel;

      for (HZahl=1; HZahl<=8; HZahl++)
      {
           AktPixel = (SWORD) ptDetectPixel ( AktSpalte, HZahl + ( ( AktZeile - 1 ) * 8) );

           if ( AktPixel < 0 )
                return ( ptCloseFileAndError ( AktPixel ) );

           PixelColor  = (BYTE) AktPixel;
           AktPixel    = DruckMuster[PixelColor][HZahl-1];
           AktPixel    = ( AktPixel << ( AktSpalte % 8 ) ) & 128;
           AktPixel    = AktPixel >> ( HZahl - 1 );
           PrinterByte = PrinterByte | AktPixel;
      }

      if (PrinterByte == 26) PrinterByte=27;

      if ( ( ptfputc( (char) PrinterByte ) ) == EOF )
             return ( ptCloseFileAndError ( PT_PRINTERERROR ) );

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintImage                                            *
 *                                                                          *
 *   Parameter: WORD      : Breite des Bildes in Pixel auf dem Drucker      *
 *              WORD      : Länge des Bildes in Pixel auf dem Drucker       *
 *              DOUBLE    : Horizontale Position des Bildes in cm oder inch *
 *                          (je nach definierter Einheit)                   *
 *                                                                          *
 *   Rückgabe : Fehlermeldung oder PT_OK                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt eine Graphik auf dem Nadel-Drucker   *
 *                 mit vorgegebener Größe aus.                              *
 ****************************************************************************/

SWORD  ptPrintImage( WORD XSpalten, WORD YZeilen, DOUBLE HorPos )
{
       WORD    PrintedSpalt,
               PrintedZeil = 0,
               Aufl = 0;
       DOUBLE  BreitPos,
               LangPos  = 1,
               BreitFaktor,
               LangFaktor,
               LoadedZeile;

       BreitFaktor = (DOUBLE) AuflosungHor / XSpalten;
       LangFaktor  = (DOUBLE) AuflosungVer / YZeilen;

       /* Kein freier Platz zwischen Zeilen */

       if ( ptSendPCXSequenz( NoLineSpace ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       while ( LangPos < AuflosungVer && ( PrintedZeil * 8 ) < YZeilen)
       {
             if ( ptLoadZeilen( (WORD) ( 24 * LangFaktor ) ) < 0 )
                  return( ptCloseFileAndError( PT_FILEERROR ) );

             for (LoadedZeile=1; LoadedZeile<=3; LoadedZeile++)
             {
                  Aufl=Aufl+ (8 * LangFaktor);

                  /* Neue Zeile                        */
                  if ( ptSendPCXSequenz( NewLine)    < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
                  /* Horizontale Position bestimmen    */
                  if ( ptMoveHorPos( HorPos )        < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );;
                  /* Graphikmodus aktivieren           */
                  if ( ptSendPCXSequenz( GraphicOn ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

                  if ( ptfputc( (char) ( XSpalten % 256 ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
                  if ( ptfputc( (char) ( XSpalten / 256 ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

                  PrintedSpalt = 0;
                  BreitPos     = 1;
                  while ( PrintedSpalt < XSpalten )
                  {
                        if ( ptPrintSpalte( (WORD) BreitPos, (WORD) LoadedZeile ) < 0 )
                             return( ptCloseFileAndError( PT_PRINTERERROR ) );
                        BreitPos += BreitFaktor;
                        PrintedSpalt++;
                  }

                  LangPos += LangFaktor;
                  PrintedZeil++;
             }
       }

       /* Normaler Zeilenabstand            */
       if ( ptSendPCXSequenz( LineSpace ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrint24Image                                          *
 *                                                                          *
 *   Parameter: XSpalten  : Breite des Bildes in Pixel auf dem Drucker      *
 *              YSpalten  : Länge des Bildes in Pixel auf dem Drucker       *
 *              HorPos    : Horizontale Position des Bildes in cm oder inch *
 *                          (je nach definierter Einheit)                   *
 *                                                                          *
 *   Rückgabe : Fehlermeldung oder PT_OK                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt eine Graphik auf dem Nadel-Drucker   *
 *                 mit vorgegebener Größe aus ( 24 Nadeln ).                *
 ****************************************************************************/

SWORD  ptPrint24Image( WORD XSpalten, WORD YZeilen, DOUBLE HorPos )
{
       WORD    PrintedSpalt,
               PrintedZeil = 0,
               Aufl = 0;
       DOUBLE  BreitPos,
               LangPos  = 1,
               BreitFaktor,
               LangFaktor,
               LoadedZeile;

       BreitFaktor = (DOUBLE) AuflosungHor / XSpalten;
       LangFaktor  = (DOUBLE) AuflosungVer / YZeilen;

       /* Kein freier Platz zwischen Zeilen */
       if ( ptSendPCXSequenz( NoLineSpace ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       while ( LangPos < AuflosungVer && ( PrintedZeil * 24 ) < YZeilen)
       {
             if ( ptLoadZeilen( (WORD) ( 24 * LangFaktor ) ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

             Aufl=Aufl+ (24 * LangFaktor);

             /* Neue Zeile                        */
             if ( ptSendPCXSequenz( NewLine)    < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
             /* Horizontale Position              */
             if ( ptMoveHorPos( HorPos )        < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );;
             /* Graphikmodus aktivieren           */
             if ( ptSendPCXSequenz( GraphicOn ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

             if ( ptfputc( (char) ( XSpalten % 256 ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
             if ( ptfputc( (char) ( XSpalten / 256 ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

             PrintedSpalt = 0;
             BreitPos     = 1;
             while ( PrintedSpalt < XSpalten )
             {
                   for (LoadedZeile=1; LoadedZeile<=3; LoadedZeile++)
                   {
                        if ( ptPrintSpalte( (WORD) BreitPos, (WORD) LoadedZeile ) < 0 )
                             return( ptCloseFileAndError( PT_PRINTERERROR ) );
                   }
                   BreitPos += BreitFaktor;
                   PrintedSpalt++;
             }

             LangPos += LangFaktor;
             PrintedZeil++;
       }
       /* Normaler Zeilenabstand            */
       if ( ptSendPCXSequenz( LineSpace ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintLaser                                            *
 *                                                                          *
 *   Parameter: XSpalten  : Breite des Bildes in Pixel auf dem Drucker      *
 *              YSpalten  : Länge des Bildes in Pixel auf dem Drucker       *
 *              HorPos    : Horizontale Position des Bildes in cm oder inch *
 *                          (je nach definierter Einheit)                   *
 *                                                                          *
 *   Rückgabe : Fehlermeldung oder PT_OK                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt eine Graphik auf dem Laser-Drucker   *
 *                 mit vorgegebener Größe aus.                              *
 ****************************************************************************/

SWORD  ptPrintLaser( WORD XSpalten, WORD YZeilen, DOUBLE HorPos )
{
       WORD    PrintedZeil  = 0,
               PrintedSpalt = 0,
               HZahl,
               AktZeile,
               AusgleichZeile = 0;
       SWORD   PixelColor;
       DOUBLE  BreitPos,
               LangPos  = 1,
               BreitFaktor,
               LangFaktor,
               Ausgleich;
       BYTE    PrinterByte,
               AktPixel;
       CHAR    PrinterOrder[10];


       XSpalten    = XSpalten - XSpalten % 8;
       BreitFaktor = (DOUBLE) AuflosungHor / XSpalten;
       LangFaktor  = (DOUBLE) AuflosungVer / YZeilen;
       Ausgleich   = (DOUBLE) ( ( (DOUBLE) AuflosungVer - ( (DOUBLE) ( (DOUBLE) ( (int) ( 24 * LangFaktor ) * YZeilen ) / 24 ) ) ) / (DOUBLE) YZeilen );

       if ( ptMoveHorPos( HorPos )      < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );;

       if ( ptSendPCXSequenz( PreDPIRes ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
       sprintf( PrinterOrder, "%d", Resolution );
       for (HZahl = 0; HZahl < strlen( PrinterOrder ); HZahl++)
       {
            if ( ( ptfputc( (char) PrinterOrder[HZahl] ) ) == EOF )
                 return( ptCloseFileAndError( PT_FILEERROR ) );
       }

       if ( ptSendPCXSequenz( AftDPIRes ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       if ( ptSendPCXSequenz( GraphicOn ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       sprintf( PrinterOrder, "%d", XSpalten / 8 );

       while ( PrintedZeil <= YZeilen )
       {
             if ( ptLoadZeilen( (WORD) ( (WORD) ( 24 * LangFaktor ) ) +
                  (WORD) ( Ausgleich * (DOUBLE) AusgleichZeile ) ) < 0 )
                  return( ptCloseFileAndError( PT_FILEERROR ) );

             if ( Ausgleich * (DOUBLE) AusgleichZeile > 1 )
             {
                  AusgleichZeile -= (DOUBLE) ( 1 / Ausgleich );
             }
             AktZeile = 1;

             while ( AktZeile <= 24 && PrintedZeil <= YZeilen )
             {
                   if ( ptSendPCXSequenz( PreBytes ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

                   for (HZahl = 0; HZahl < strlen( PrinterOrder ); HZahl++)
                        if ( ptfputc( (char) PrinterOrder[HZahl] ) == EOF )
                             return( ptCloseFileAndError( PT_PRINTERERROR ) );

                   if ( ptSendPCXSequenz( AftBytes ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

                   BreitPos     = 1;
                   PrintedSpalt = 0;
                   while ( BreitPos <= AuflosungHor && PrintedSpalt <= ( XSpalten / 8 ))
                   {
                         PrinterByte = 0;
                         for ( HZahl=1; HZahl<=8; HZahl++)
                         {
                               if ( (WORD) BreitPos <= AuflosungHor )
                               {
                                    if ( ( PixelColor  = ptDetectPixel( (WORD) BreitPos, AktZeile ) ) < 0 )
                                         return( ptCloseFileAndError ( PixelColor ) );
                               }
                               else
                               {
                                    if ( ( PixelColor  = ptDetectPixel( (WORD) AuflosungHor, AktZeile ) ) < 0 )
                                         return( ptCloseFileAndError ( PixelColor ) );
                               }

                               AktPixel    = DruckMuster[PixelColor][( (WORD) LangPos % 8)];
                               AktPixel    = ( AktPixel << ( ( HZahl - 1) % 8 ) ) & 128;
                               AktPixel    = AktPixel >> ( HZahl - 1 );
                               PrinterByte = PrinterByte | AktPixel;
                               BreitPos   += BreitFaktor;
                         }
                         if ( PrinterByte == 26 )
                         {
                              PrinterByte = 27;
                         }
                         if ( ptfputc( (char) PrinterByte ) == EOF )
                              return( ptCloseFileAndError( PT_PRINTERERROR ) );
                         PrintedSpalt++;
                   }
                   LangPos += LangFaktor;
                   AktZeile++;
                   PrintedZeil++;
                   AusgleichZeile++;
             }
       }

       if ( ptSendPCXSequenz( CodeEnd ) < 0 ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

       return( ptSkipExactStandard( 48 * YZeilen / Resolution ) );
}

/****************************************************************************
 *   Funktionsname: ptLoadPCXParameters                                     *
 *                                                                          *
 *   Parameter: FILE** : Doppelzeiger auf Dateihandle                       *
 *                                                                          *
 *   Rückgabe : PT_OK oder Fehlermeldung                                    *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptLoadPCXParameters( FILE **Datei )
{
     SWORD Zahl,
           Zahl2,
           Zeichen;

     if ( ( Zeichen = fgetc( *Datei ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
     PrinterType = Zeichen;

     if ( ( Zahl  = fgetc( *Datei ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );
     if ( ( Zahl2 = fgetc( *Datei ) ) == EOF ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

     Resolution = Zahl * 256 + Zahl2;

     if ( ptLoadCodes( &GraphicOn, Datei )   < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );

     if ( PrinterType == PT_ESC_9 || PrinterType == PT_ESC_24 )
     {
          if ( ptLoadCodes( &NoLineSpace, Datei )   < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &NewLine, Datei )       < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &LineSpace, Datei )     < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );

          if ( PrinterType == PT_ESC_9 )
          {
               YResolution = 82;  /* Bei Neunnadlern: 8 Pixel/Zeile (=8 Pixel pro 7/72 Zoll) -> Aufl. = 8 * 1/(7/72) = 82 */
          }
          else
          {
               YResolution = 246; /* Bei 24-Nadlern: 24 Pixel/Zeile -> Aufl. = 24 * 1/(7/72) = 246 */
          }
     }
     else
     {
          if ( ptLoadCodes( &PreDPIRes, Datei )     < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &AftDPIRes, Datei )     < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &PreBytes, Datei )      < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &AftBytes, Datei )      < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );
          if ( ptLoadCodes( &CodeEnd, Datei )       < 0 ) return( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );

          YResolution = Resolution;   /* Bei HP: X-Auflösung = Y-Auflösung */
     }
     if ( fclose( *Datei ) != 0 ) return( ptCloseFileAndError( PT_FILEERROR ) );

     *Datei = NULL;

     if ( DruckerHandle == NULL )
          if ( ( DruckerHandle = fopen( Port, "wb" ) ) == NULL ) return( ptCloseFileAndError( PT_PRINTERERROR ) );

     return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptInitPCXPrinter                                          *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Druckername (=Dateiname)                *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptInitPCXPrinter ( CHAR *Drucker )
{
      FILE *Datei;
      CHAR PrinterName[8+3];
      WORD Array1,
           Array2;

      strncpy( PrinterName, Drucker, 8 );
      PrinterName[8] = PT_EOS;
      strcat( PrinterName, ".PX");

      for ( Array1=0; Array1<=24; Array1++ )
            for ( Array2=0; Array2<=4; Array2++ )
                  Zeile[Array1][Array2] = NULL;

      Datei = fopen( PrinterName, "rb" );

      if ( Datei == NULL ) return( ptCloseFileAndError( PT_FILEERROR ) );

      if ( ptLoadPCXParameters( &Datei ) < 0 )  return( ptCloseFileAndError( PT_FILEERROR ) );

      return ( PT_OK );
}


/****************************************************************************
 *   Funktionsname: ptPrintImageNow  [USER]                                 *
 *                                                                          *
 *   Parameter: CHAR*  : Zeiger auf Druckertreibername (=Dateiname+".PX")   *
 *              CHAR*  : Zeiger auf Bildname ( incl. PCX-Endung ! )         *
 *              DOUBLE : Horizontale Größe des Bildes in inch oder cm       *
 *              DOUBLE : Vertikale Größe des Bildes in inch oder cm         *
 *              HorPos : Horizontale Position des Bildes in cm oder inch    *
 *                       (je nach definierter Einheit)                      *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion lädt einen neuen Druckertreiber.          *
 ****************************************************************************/

SWORD ptPrintImageNow ( CHAR *Drucker, CHAR *PicName, DOUBLE XRes,
                        DOUBLE YRes, DOUBLE HorPos )
{
      SWORD Resultat;

      if ( ptInitPCXPrinter( Drucker ) < 0 ) return( ptCloseFileAndError( PT_FILEERROR ) );

      if ( ptOpenFile( PicName ) < 0 ) return ( ptCloseFileAndError( PT_FILEERROR ) );

      if ( ptGetMemory() < 0 ) return ( ptCloseFileAndError( PT_NOTENOUGHMEMORY ) );

      if ( AktMeasureUnit == PT_CM )
      {
           XRes = ptCm2Inch( XRes );
           YRes = ptCm2Inch( YRes );
      }

      if ( PrinterType == PT_HP )
      {
           if ( ( Resultat = ptPrintLaser( (WORD) ( (DOUBLE) XRes * (DOUBLE) Resolution ),
                                           (WORD) ( (DOUBLE) YRes * (DOUBLE) Resolution ), HorPos ) ) < 0 )
                return( ptCloseFileAndError( Resultat ) );
      }
      else
      if ( PrinterType == PT_ESC_9 )
      {
           if ( ( Resultat = ptPrintImage( (WORD) ( (DOUBLE) XRes * (DOUBLE) Resolution ),
                                           (WORD) ( (DOUBLE) YRes * (DOUBLE) YResolution), HorPos ) ) < 0 )

                return( ptCloseFileAndError( Resultat ) );
      }
      else
      {
           if ( ( Resultat = ptPrint24Image( (WORD) ( (DOUBLE) XRes * (DOUBLE) Resolution ),
                                             (WORD) ( (DOUBLE) YRes * (DOUBLE) YResolution), HorPos ) ) < 0 )
                return( ptCloseFileAndError( Resultat ) );
      }

      ptClearLoadedPCXData();

      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintPCXBufferWithLine                                *
 *                                                                          *
 *   Parameter: WORD   : Zeile                                              *
 *              CHAR*  : Zeiger auf Druckertreibername                      *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt alle im Speicher befindlichen Daten  *
 *                 einer Zeile an den Drucker aus. Dabei werden die einzel- *
 *                 nen "Minizeilen" nach ihrem ptSkipExact-Wert sortiert.   *
 ****************************************************************************/

SWORD ptPrintPCXBufferWithLine ( WORD XZeile, CHAR *PrinterDriver )
{

      WORD  AktBuffer      = 0,
            UnitsToSkip,
            AktUnit,
            TakeBuffer,
            OldUnit,
            YPixel;
      SWORD LastUnit;
      BOOL  NoMoreExacts   = PT_FALSE,
            NewUnit        = PT_FALSE;

      LastUnit    = PT_NOUNITCHOSEN;
      UnitsToSkip = VerUnitsPerLine;

      OldUnit = AktMeasureUnit;
      ptDefineUnit( PT_INCH );

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
                                 if ( ptSkipExact( AktUnit-LastUnit ) < 0 )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return ( PT_PRINTERERROR );
                                 }
                                 UnitsToSkip -= AktUnit - LastUnit;
                            }
                            else
                            {
                                 if ( ( BufferZeiger + TakeBuffer ) -> ExactVerPos > 0 &&
                                        NewUnit == PT_TRUE )
                                 {
                                      if ( ptSkipExact( ( BufferZeiger + TakeBuffer ) -> ExactVerPos )
                                           < 0 )
                                      {
                                           ptDefineUnit( OldUnit );
                                           return ( PT_PRINTERERROR );
                                      }
                                      UnitsToSkip -= ( BufferZeiger + TakeBuffer ) -> ExactVerPos;
                                 }
                            }

                            if ( ( BufferZeiger + TakeBuffer ) -> Ausrichtung == PT_PCX ||
                                 ( BufferZeiger + TakeBuffer ) -> Ausrichtung == PT_PCC )
                            {
                                 if ( ptPrintImageNow( PrinterDriver,
                                      ( BufferZeiger + TakeBuffer ) -> Text,
                                      ( BufferZeiger + TakeBuffer ) -> Right,
                                      ( BufferZeiger + TakeBuffer ) -> Left,
                                      (DOUBLE) ( BufferZeiger + TakeBuffer ) -> Font / (DOUBLE) 100 )
                                      < 0 )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return ( PT_PRINTERERROR );
                                 }

        /* Y-Auflösung ist vielfaches von 8 (9-Nadler) bzw. 24 (24-Nadler), daher muß */
        /* in einigen Fällen Aufgerundet werden, um die Positionsberechnung korrekt zu*/
        /* halten !           */

                                 YPixel = (WORD) ( (BufferZeiger + TakeBuffer ) -> Left * (DOUBLE) YResolution );

                                 if ( PrinterType == PT_ESC_9 )
                                 {
                                      if ( YPixel % 8 != 0 )
                                           YPixel -= 8 - YPixel % 8;
                                 }
                                 else
                                 if ( PrinterType == PT_ESC_24 )
                                 {
                                      if ( YPixel % 24 != 0 )
                                           YPixel += 24 - YPixel % 24;
                                 }

                                 AktUnit += (WORD) ( (LONG) VerticalMoveUnit * (LONG)
                                            YPixel / (LONG) YResolution );
                                 if ( ptSkipExact( VerUnitsPerLine - AktUnit % VerUnitsPerLine +
                                      VerUnitsPerLine) < 0 )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return( PT_PRINTERERROR );
                                 }

                                 if ( PrinterType == PT_HP )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return( AktUnit / VerUnitsPerLine );
                                 }
                                 else
                                 {
                                      ptDefineUnit( OldUnit );
                                      return( AktUnit / VerUnitsPerLine + 2 );
                                 }
                            }
                            else
                            {
                                 if ( ptSetSequenz(  ( BufferZeiger + TakeBuffer ) -> BoldPrint,
                                      ( BufferZeiger + TakeBuffer ) -> Underline,
                                      ( BufferZeiger + TakeBuffer ) -> Italic ) < 0 )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return ( PT_PRINTERERROR );
                                 }

                                 if ( ( BufferZeiger + TakeBuffer ) -> Font != SetFont )
                                 {
                                      if ( ptChooseFont( ( BufferZeiger + TakeBuffer ) -> Font ) < 0 )
                                      {
                                           ptDefineUnit( OldUnit );
                                           return ( PT_PRINTERERROR );
                                      }
                                 }

                                 if ( ptPrintAutoNow( ( BufferZeiger + TakeBuffer ) -> Text,
                                      ( BufferZeiger + TakeBuffer ) -> Left,
                                      ( BufferZeiger + TakeBuffer ) -> Right,
                                      ( BufferZeiger + TakeBuffer ) -> Ausrichtung )
                                      < 0 )
                                 {
                                      ptDefineUnit( OldUnit );
                                      return ( PT_PRINTERERROR );
                                 }
                            }
                            LastUnit = AktUnit;
                       }
                  }
                  AktBuffer++;
                  NewUnit = PT_FALSE;
            }
      }

      ptDefineUnit( OldUnit );

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
 *   Funktionsname: ptSendPCXBuffer [USER]                                  *
 *                                                                          *
 *   Parameter: CHAR*  : Name der Graphik-Druckertreiberdatei               *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion gibt alle im Speicher befindlichen Daten  *
 *                 ( soweit vorhanden ) INCL. PCX-BILDER an den Drucker aus.*
 *                 Die Standardfunktion "ptSendBuffer" gibt keine PCX-      *
 *                 Graphiken aus !                                          *
 ****************************************************************************/

SWORD ptSendPCXBuffer ( CHAR *PrinterDriver )
{

       WORD  AktZeile,
             AktBuffer;
       SWORD HVar;
       BOOL  NewLineDone,
             Old_ActiveSpecialChars;

       if ( AnzahlBufferEintrage == 0 )
       {
            return ( PT_NOBUFFERENTRIES );
       }

       Old_ActiveSpecialChars = ActiveSpecialChars;
       if ( ptSetSpecialChar( PT_OFF ) < 0 ) return ( PT_PRINTERERROR );

       if ( ptResetPrinter() < 0 ) return ( PT_PRINTERERROR );
       if ( ptSetSequenz( PT_FALSE, PT_FALSE, PT_FALSE ) < 0 ) return ( PT_PRINTERERROR );
       AktZeile = 1;

       if ( VerMoveKind == PT_HP )
       {
            if ( ptSkipExact( 1 ) < 0 ) return ( PT_PRINTERERROR );
            if ( ptSkipExact( VerUnitsPerLine - 1 ) < 0 ) return ( PT_PRINTERERROR );
       }

       while ( AktZeile <= LinesPerPage )
       {
             AktBuffer = 0;
             NewLineDone = PT_FALSE;
             while ( AktBuffer < AnzahlBufferEintrage && NewLineDone == PT_FALSE)
             {
                   if ( ( BufferZeiger + AktBuffer ) -> Line == AktZeile )
                   {
                        if ( ( HVar = ptPrintPCXBufferWithLine ( AktZeile, PrinterDriver ) ) < 0 )
                             return ( PT_PRINTERERROR );
                        if ( HVar > 0 )
                        {
                             AktZeile += HVar;
                             AktLine  += HVar;
                        }
                        NewLineDone = PT_TRUE;
                   }
                   AktBuffer++;
             }
             AktZeile++;
             if ( NewLineDone == PT_FALSE && AktZeile < LinesPerPage)
             {
                  if ( ptSkip ( 1 ) < 0 ) return ( PT_PRINTERERROR );
             }
       }

       if ( Old_ActiveSpecialChars == PT_TRUE )
       {
            if ( ptSetSpecialChar( PT_ON ) < 0 ) return ( PT_PRINTERERROR );
       }

       ptClearLoadedPCXData();

       return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptPrintImageBuffer  [USER]                              *
 *                                                                          *
 *   Parameter: WORD   : Zeile, ab welcher das Bild gedruckt werden soll.   *
 *              WORD   : Anzahl min. vert. Einheiten von dieser Zeile       *
 *                       (ptSkipExactStandard-Einheiten zu 1/48 Zoll)       *
 *              CHAR*  : Zeiger auf Bildname ( incl. PCX-Endung ! )         *
 *              BYTE   : Art des Bildes ( PT_PCX, PT_PCC )                  *
 *              DOUBLE : Horizontale Größe des Bildes in cm oder inch       *
 *              DOUBLE : Vertikale Größe des Bildes in cm oder inch         *
 *              DOUBLE : Horizontale Position des Bildes in cm oder inch    *
 *                       (je nach definierter Einheit)                      *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion legt die Position eines Bildes im Speicher*
 *                 ab. Ausgegeben wird alles mit "ptSendPCXBuffer"          *
 *                 Alle Daten werden dabei über die BUFFER-Struktur im      *
 *                 Speicher abgelegt (normalerweise von ptPrintAutoBuffer   *
 *                 verwaltet). Dabei werden die einzelnen BUFFER-Werte wie  *
 *                 folgt "mißbrauch":                                       *
 *                 Text[CHAR*]       :   Name des PCX-Files                 *
 *                 Font[WORD]        :   Horizontale Position in inch * 100 *
 *                 Line[WORD]        :   Ab welcher Zeile Bild darstellen   *
 *                 ExactVerPos[WORD] :   Zusätzliche Standard-Einheiten     *
 *                 Left[DOUBLE]      :   Vert. Größe in inch                *
 *                 Right[DOUBLE]     :   Hor. Größe in inch                 *
 ****************************************************************************/

SWORD ptPrintImageBuffer ( WORD Line, WORD VerUnits, CHAR *PicName,
                           DOUBLE XRes, DOUBLE YRes, DOUBLE HorPos )
{
      WORD  AktFontNr;
      SWORD HError;
      BYTE  FileType = PT_PCX;

      if ( AktMeasureUnit == PT_CM )
         HorPos = ptCm2Inch( HorPos );

      AktFontNr = AktFont;
      AktFont   = (WORD) ( HorPos * (DOUBLE) 100 );

      if ( ( HError = ptPrintAutoBuffer( Line, VerUnits, PicName, YRes,
           XRes, FileType ) ) < 0 )
      {
           AktFont = AktFontNr;
           return( HError );
      }

      AktFont = AktFontNr;
      return ( PT_OK );
}

/****************************************************************************
 *   Funktionsname: ptDefinePattern     [USER]                              *
 *                                                                          *
 *   Parameter: WORD   : Farbe (0 bis 15), deren Muster verändert werden    *
 *                       soll.                                              *
 *              8xBYTE : 8 Bytes für Muster (s. Handbuch !)                 *
 *                                                                          *
 *   Rückgabe : SWORD  : PT_OK oder Fehlermeldung                           *
 *                                                                          *
 *   Beschreibung: Diese Funktion gestattet dem Anwender, die Muster für    *
 *                 die einzelnen Farben selbst bestimmen zu können.         *
 ****************************************************************************/

SWORD ptDefinePattern( WORD Color, BYTE P1, BYTE P2, BYTE P3, BYTE P4, BYTE P5,
                       BYTE P6, BYTE P7, BYTE P8 )
{
      if ( Color > 15 ) return ( PT_WRONGPARAMETER );

      DruckMuster[Color][0] = P1;
      DruckMuster[Color][1] = P2;
      DruckMuster[Color][2] = P3;
      DruckMuster[Color][3] = P4;
      DruckMuster[Color][4] = P5;
      DruckMuster[Color][5] = P6;
      DruckMuster[Color][6] = P7;
      DruckMuster[Color][7] = P8;

      return( PT_OK );
}
