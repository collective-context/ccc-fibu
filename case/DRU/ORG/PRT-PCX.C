/*----------------------------------------------------------------------------

              Programm fÅr die Definition eigener Druckertreiber

                      fÅr die Ausgabe von PCX-Graphiken

                          (c) 1992/93 by Markus MÅck

-----------------------------------------------------------------------------*/

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>
  #include <conio.h>

  typedef  signed   int       SWORD;
  typedef  signed   char      CHAR;
  typedef  signed   char      SCHAR;
  typedef  unsigned char      BYTE;
  typedef  signed   char      SBYTE;
  typedef  unsigned int       WORD;
  typedef  unsigned long      LONG;
  typedef  unsigned int       BOOL;
  typedef  float              FLOAT;
  typedef  double             DOUBLE;
  typedef  void               VOID;

  #define  TRUE               0
  #define  FALSE              1
  #define  OK                 0
  #define  CR                 13
  #define  LF                 10
  #define  SPACE              32
  #define  DELETE             8
  #define  EOS                0

  #define  MM_HP              5     /* HP-Laserdrucker                            */
  #define  MM_ESC_9           6     /* Epson 9-Nadler                             */
  #define  MM_ESC_24          7     /* Epson 24-Nadler                            */

  #define  MM_NOTENOUGHMEMORY -1
  #define  MM_ERROR           -2


  CHAR     PrinterName[8+3];        /* Name des Druckers = Filename  + Endung 'PX'*/
  BYTE     PrinterType   = 0;       /* Art des Bewegungscodes: MM_HP oder MM_ESC..*/

  CHAR     *NoLineSpace  = NULL;    /* [EPSON] Code fÅr Zeilenabstand 7/72 ''     */
  CHAR     *NewLine      = NULL;    /* [EPSON] Code fÅr neue Zeile                */
  CHAR     *GraphicOn    = NULL;    /* [EPSON,HP] Code fÅr 'Graphikmodus ein'     */
  CHAR     *LineSpace    = NULL;    /* [EPSON] Code fÅr normalen Zeilenabstand    */
  WORD     Resolution    = 0;       /* [EPSON,HP] Auflîsung der Graphik           */

  CHAR     *PreYRes      = NULL;    /* [HP] Code VOR Y-Auflîsung ( ESC & a )      */
  CHAR     *AftYRes      = NULL;    /* [HP] Code NACH Y-Auflîsung ( H )           */
  CHAR     *PreDPIRes    = NULL;    /* [HP] Code VOR Druckauflîsung in DPI        */
  CHAR     *AftDPIRes    = NULL;    /* [HP] Code NACH Druckauflîsung in DPI       */
  CHAR     *PreBytes     = NULL;    /* [HP] Code VOR Anzahl Graphicbytes          */
  CHAR     *AftBytes     = NULL;    /* [HP] Code NACH Anzahl Graphicbytes         */
  CHAR     *CodeEnd      = NULL;    /* [HP] Code fÅr Graphic-Ende                 */

  CHAR     Z[4];                    /* Hilfsvariable                              */
  SWORD    *Zeiger;                 /* Hilfsvariable                              */
  WORD     Zahl;                    /* Hilfsvariable                              */

  FILE     *Datei;

  VOID HeadLine ( VOID )
  {
       clrscr();  /* Mu· bei MS-Compilern entfallen ! */
       printf("--------------------------------------------------------------------------------");
       printf("                   MÅck Softwareentwicklung C - Druckertoolbox\n\r" );
       printf("                              PCX-Druckerdefinition\n\r");
       printf("--------------------------------------------------------------------------------\n\r");
  }

  VOID Input ( CHAR *Ziel, BYTE MaxLen )
  {
       BYTE Chars = 0;
       CHAR Charac = 0;

       if ( *Ziel != 0 )
       {
            printf("%s", Ziel );
            Chars = strlen( Ziel );
       }

       do
       {
            Charac = getch();
            if ( Charac == 3 )
            {
                 printf("\n\r*** ^C - Programm beendet ***\n\r");
                 exit(0);
            }
            if ( isalnum( Charac ) != 0 && Chars < MaxLen )
            {
                 printf( "%c", Charac );
                 *( Ziel + Chars ) = Charac;
                 Chars++;
            }
            else
            if ( Charac == DELETE && Chars > 0 )
            {
                 printf( "%c %c", DELETE, DELETE );
                 Chars--;
            }
       }
       while ( Charac != CR );

       *( Ziel + Chars ) = EOS;
  }

  SWORD AsciiHex2Numbers( SWORD Anzahl, CHAR **Zielspeicher, CHAR *HexCodes )
  {
        CHAR  Dummy[4] = "0x  ";
        SCHAR *Error;
        WORD  ZahlNr = 0;

        if ( Anzahl < 0 ) return ( MM_ERROR );

        *Zielspeicher = malloc ( Anzahl + 1 );

        if ( Zielspeicher == NULL )
        {
             printf("\n\r\n\r NOT ENOUGH MEMORY !!\n\r\n\r ");
             exit ( 0 );
        }

        *( *( Zielspeicher ) )= Anzahl;

        while ( ZahlNr < Anzahl )
        {
              strncpy( &Dummy[2], &( * ( HexCodes + ZahlNr * 2 ) ), 2 );
              *( *( Zielspeicher ) + ZahlNr + 1 ) = (BYTE) strtol( Dummy, &Error, 0 );
              ZahlNr++;
        }
        return ( OK );
  }


  VOID EnterHexCode ( CHAR **Zielspeicher, WORD MaxAnzahl )
  {
       WORD ZahlNr = 0,
            Zahl,
            Stand;
       BYTE HexDigit = 1;
       CHAR Charac,
            Ziel[100];

       if ( *Zielspeicher != NULL )
       {
            ZahlNr = Zahl = **Zielspeicher;

            for ( Stand=0; Stand < Zahl; Stand++)
            {
                  if ( *(*Zielspeicher+Stand+1) > 0xf )
                  {
                       sprintf( &Ziel[Stand*2], "%X", *(*Zielspeicher+Stand+1) );
                  }
                  else
                  {
                       Ziel[Stand*2] = '0';
                       sprintf( &Ziel[Stand*2+1], "%X", *(*Zielspeicher+Stand+1) );
                  }

                  printf("%c%c ", Ziel[Stand*2], Ziel[Stand*2+1]);
            }
       }

       do
       {
            Charac = getch();
            if ( Charac == 3 )
            {
                 printf("\n\r*** ^C - Programm beendet ***\n\r");
                 exit(0);
            }

            if ( isxdigit( Charac ) != 0 && ZahlNr < MaxAnzahl )
            {
                 if ( Charac>='a' && Charac<='z' ) Charac -= 32;
                 printf("%c", Charac );
                 *( Ziel + ZahlNr * 2 + HexDigit - 1 ) = Charac;
                 if ( HexDigit == 2 )
                 {
                      printf(" ");
                      HexDigit = 1;
                      ZahlNr++;
                 }
                 else
                 {
                      HexDigit++;
                 }
            }

            if ( Charac == DELETE && ZahlNr > 0 && HexDigit == 1 )
            {
                 printf("%c%c %c", DELETE, DELETE, DELETE );
                 ZahlNr--;
                 HexDigit++;
            }
            else
            if ( Charac == DELETE && HexDigit == 2 )
            {
                  printf("%c %c", DELETE, DELETE );
                  HexDigit--;
            }
       }
       while ( HexDigit == 2 || Charac != CR );

       *( Ziel + ZahlNr*2 ) = 0;

       AsciiHex2Numbers( ZahlNr, Zielspeicher, Ziel );
  }

  CHAR JaNein( VOID )
  {
       CHAR ch;

       do
       {
           ch = getch();
           if ( ch == 3 )
           {
                printf("\n\r*** ^C - Programm beendet ***\n\r");
                exit(0);
           }
           if ( ch == 'j' ) ch ='J';
           if ( ch == 'n' ) ch ='N';
       }
       while ( ch != 'J' && ch != 'N');

       return ( ch );
  }

  CHAR YesNo( VOID )
  {
       printf("\n\r\n\r");
       printf("                            Werte Åbernehmen (J/N)?");
       return ( JaNein() );
  }

  SWORD SaveParameters( VOID )
  {
       BYTE Zahl = 0;

       Datei = fopen ( PrinterName, "wb" );

       if ( Datei == NULL ) return ( MM_ERROR );

       fputc( (BYTE) ( PrinterType ), Datei );
       fputc( (BYTE) ( Resolution / 256 ), Datei );
       fputc( (BYTE) ( Resolution % 256 ), Datei );

       fputc( (BYTE) *GraphicOn, Datei );
       for( Zahl=1; Zahl<=*GraphicOn; Zahl++)
            fputc( *(GraphicOn+Zahl), Datei );

       if ( PrinterType == MM_ESC_9 || PrinterType == MM_ESC_24 )
       {
            fputc( (BYTE) *NoLineSpace, Datei );
            if ( *NoLineSpace > 0 )
                 for( Zahl=1; Zahl<=*NoLineSpace; Zahl++) fputc( *(NoLineSpace+Zahl), Datei );
            fputc( (BYTE) *NewLine, Datei );
            if ( NewLine > 0 )
                 for( Zahl=1; Zahl<=*NewLine; Zahl++) fputc( *(NewLine+Zahl), Datei );
            fputc( (BYTE) *LineSpace, Datei );
            if ( *LineSpace > 0 )
                 for( Zahl=1; Zahl<=*LineSpace; Zahl++) fputc( *(LineSpace+Zahl), Datei );
       }
       else
       {
            fputc( (BYTE) *PreDPIRes, Datei );
            if ( *PreDPIRes > 0 )
                 for( Zahl=1; Zahl<=*PreDPIRes; Zahl++) fputc( *(PreDPIRes+Zahl), Datei );
            fputc( (BYTE) *AftDPIRes, Datei );
            if ( *AftDPIRes > 0 )
                 for( Zahl=1; Zahl<=*AftDPIRes; Zahl++) fputc( *(AftDPIRes+Zahl), Datei );
            fputc( (BYTE) *PreBytes, Datei );
            if ( *PreBytes > 0 )
                 for( Zahl=1; Zahl<=*PreBytes; Zahl++) fputc( *(PreBytes+Zahl), Datei );
            fputc( (BYTE) *AftBytes, Datei );
            if ( *AftBytes > 0 )
                 for( Zahl=1; Zahl<=*AftBytes; Zahl++) fputc( *(AftBytes+Zahl), Datei );
            fputc( (BYTE) *CodeEnd, Datei );
            if ( *CodeEnd > 0 )
                 for( Zahl=1; Zahl<=*CodeEnd; Zahl++) fputc( *(CodeEnd+Zahl), Datei );
       }

       if ( fclose( Datei ) != 0 ) return ( MM_ERROR );

       return ( OK );
  }

  SWORD GetMemory( CHAR **Ziel, WORD Menge )
  {
        *Ziel = malloc( Menge );
        if ( *Ziel == NULL)
        {
             printf("\n\r\n\r NOT ENOUGH MEMORY !! \n\r\n\r");
             exit(0);
        }

        return ( OK );
  }

  VOID LoadCodes ( CHAR **Zeiger )
  {
       BYTE Zahl2, Zahl;

       Zahl2 = fgetc( Datei );
       GetMemory( Zeiger, Zahl2 + 1 );
       **Zeiger = Zahl2;
       if ( Zahl2 > 0 )
            for( Zahl=1; Zahl<=Zahl2; Zahl++) *(*Zeiger + Zahl) = fgetc( Datei );
  }

  VOID LoadString ( CHAR *Zeiger )
  {
       BYTE Zahl;
       CHAR Chr;

       Zahl = 0;
       do
       {
            Chr = fgetc( Datei );
            Zeiger[Zahl] = Chr;
            Zahl++;
       }
       while ( Chr != 0 );
  }


  SWORD LoadParameters( CHAR *DName )
  {

       Datei = fopen ( DName, "rb" );

       if ( Datei == NULL ) return ( MM_ERROR );

       PrinterType = fgetc( Datei );
       Resolution = fgetc( Datei ) * 256 + fgetc( Datei );

       LoadCodes ( &GraphicOn );

       if ( PrinterType == MM_ESC_9 || PrinterType == MM_ESC_24 )
       {
            LoadCodes( &NoLineSpace );
            LoadCodes( &NewLine     );
            LoadCodes( &LineSpace   );
       }
       else
       {
            LoadCodes( &PreDPIRes   );
            LoadCodes( &AftDPIRes   );
            LoadCodes( &PreBytes    );
            LoadCodes( &AftBytes    );
            LoadCodes( &CodeEnd     );
       }

       if ( fclose( Datei ) != 0 ) return ( MM_ERROR );

       return ( OK );
  }


  VOID main ( VOID )
  {
       CHAR  Z[100],
             Charac,
             PrName[8] = { "" };
       SCHAR *Zeiger;
       WORD  Zahl;

       Z[0] = 0;

       do
       {
            HeadLine();
            printf("Name des Druckers: ");
            Input( PrName, 8 );
            strcpy( PrinterName, PrName );
            strcat( PrinterName, ".PX" );

            LoadParameters( PrinterName );

            printf("\n\r\n\r");

            printf("Druckertyp ( (H)P-Laser, ESC-(9)-Nadler oder ESC-(2)4-Nadler ): ");
            if ( PrinterType == MM_HP )
            {
                 printf( "HP-Laser     " );
                 gotoxy( 65, 8 );
            }
            else
            if ( PrinterType == MM_ESC_9 )
            {
                 printf( "ESC-9-Nadler " );
                 gotoxy( 65, 8 );
            }
            else
            if ( PrinterType == MM_ESC_24 )
            {
                 printf( "ESC-24-Nadler" );
                 gotoxy( 65, 8 );
            }
            do
            {
                 Charac = getch();
                 if ( Charac == 3 )
                 {
                      printf("\n\r*** ^C - Programm beendet ***\n\r");
                      exit(0);
                 }

            }
            while ( Charac != 'h' && Charac != 'H' && Charac != '2' && Charac != '9' );
            if ( Charac == 'h' || Charac == 'H' )
            {
                 PrinterType = MM_HP;
                 printf( "HP-Laser     ");
            }
            else
            if ( Charac == '9' )
            {
                 PrinterType = MM_ESC_9;
                 printf( "ESC-9-Nadler ");
            }
            else
            {
                 PrinterType = MM_ESC_24;
                 printf( "ESC-24-Nadler");
            }

            if ( PrinterType == MM_HP )
            {

                printf("\n\r\n\r");
                printf("Code VOR Auflîsung in DPI (Hex): ");
                EnterHexCode( &PreDPIRes, 19 );

                printf("\n\r\n\r");
                printf("Code NACH Auflîsung in DPI (Hex): ");
                EnterHexCode( &AftDPIRes, 19 );

                printf("\n\r\n\r");
                printf("Code fÅr 'Graphikmodus an' (Hex): ");
                EnterHexCode( &GraphicOn, 19 );
            }
            else
            {
                printf("\n\r\n\r");
                printf("Code fÅr Zeilenabstand 7/72 '' (Hex): ");
                EnterHexCode( &NoLineSpace, 19 );

                printf("\n\r\n\r");
                printf("Code fÅr 'Neue Zeile' (Hex): ");
                EnterHexCode( &NewLine, 19 );

                printf("\n\r\n\r");
                printf("Code fÅr 'Graphikmodus an' (Hex): ");
                EnterHexCode( &GraphicOn, 19 );

                printf("\n\r\n\r");
                printf("Code fÅr normalen Zeilenabstand (Hex): ");
                EnterHexCode( &LineSpace, 19 );

                printf("\n\r\n\r");
                printf("Graphikauflîsung in DPI: ");
                if ( Resolution > 0 )
                {
                     itoa( Resolution, Z, 10 );
                }
                else
                {
                     Z[0] = 0;
                }
                do
                {
                     Input( Z, 3 );
                     Resolution = (WORD) strtol( Z, &Zeiger, 10 );
                     if ( (*Zeiger != 0 || Resolution == 0) && strlen(Z) > 0 )
                     {
                          for (Zahl=0; Zahl < strlen(Z); Zahl++) printf("%c", DELETE );
                          printf("   %c%c%c", DELETE, DELETE, DELETE );
                     }
                }
                while ( Resolution == 0 );
            }
       }
       while ( YesNo() == 'N' );

       if ( PrinterType == MM_HP )
       do
       {
            HeadLine();

            printf("\n\r\n\r");
            printf("Code VOR Anzahl GraphicBytes (Hex): ");
            EnterHexCode( &PreBytes, 19 );

            printf("\n\r\n\r");
            printf("Code NACH Anzahl GraphicBytes (Hex): ");
            EnterHexCode( &AftBytes, 19 );

            printf("\n\r\n\r");
            printf("Code fÅr Graphik-Ende (Hex): ");
            EnterHexCode( &CodeEnd, 19 );

            printf("\n\r\n\r");
            printf("Graphikauflîsung in DPI: ");
            if ( Resolution > 0 )
            {
                 itoa( Resolution, Z, 10 );
            }
            else
            {
                 Z[0] = 0;
            }
            do
            {
                 Input( Z, 3 );
                 Resolution = (WORD) strtol( Z, &Zeiger, 10 );
                 if ( (*Zeiger != 0 || Resolution == 0) && strlen(Z) > 0 )
                 {
                      for (Zahl=0; Zahl < strlen(Z); Zahl++) printf("%c", DELETE );
                      printf("   %c%c%c", DELETE, DELETE, DELETE );
                 }
            }
            while ( Resolution == 0 );
       }
       while ( YesNo() == 'N' );

       printf("\n\r\n\r");
       printf("                            Daten speichern (J/N) ? ");
       Z[0] = JaNein();
       printf( "%c", Z[0] );
       if ( Z[0] == 'J' ) printf(""); SaveParameters();
  }