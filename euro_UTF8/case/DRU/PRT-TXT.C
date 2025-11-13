/*---------------------------------------------------------------------------

           Programm für die Definition eigener Druckertreiber

                      (c) 1992/93 by Markus Mück

-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

typedef  signed   int       SWORD;
typedef  unsigned char      CHAR;
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

#define  MM_HP              5
#define  MM_ESC             6

#define  MM_NOTENOUGHMEMORY -1
#define  MM_ERROR           -2


CHAR     PrinterName[8];          /* Name des Druckers = Filename               */
CHAR     Port[8];                 /* Druckerport, z.B. Lpt1                     */
WORD     VerticalMoveUnit = 0;    /* Kleinstmögliche vertikale Bewegungseinheit */
WORD     VerUnitsPerLine  = 0;    /* Vertikale Bewegungseinheit für 1 Zeile     */
BYTE     VerMoveKind   = 0;       /* Art des Bewegungscodes: MM_HP oder MM_ESC  */
CHAR     *PreVerCode   = NULL;    /* Code vor vertikaler Bewegung               */
CHAR     *AftVerCode   = NULL;    /* Code nach vertikaler Bewegung              */
CHAR     *Reset        = NULL;    /* Zeiger auf String für Druckerreset         */
CHAR     *BoldOn       = NULL;    /* Zeiger auf String für Fettdruck ein        */
CHAR     *BoldOff      = NULL;    /* Zeiger auf String für Fettdruck aus        */
CHAR     *UnderlineOn  = NULL;    /* Zeiger auf String für Unterstreichen an    */
CHAR     *UnderlineOff = NULL;    /* Zeiger auf String für Unterstreichen aus   */
CHAR     *ItalicOn     = NULL;    /* Zeiger auf String für Kursiv ein           */
CHAR     *ItalicOff    = NULL;    /* Zeiger auf String für Kursiv aus           */
CHAR     *BackSpace    = NULL;    /* Zeiger auf String für "1 Zeichen zurück"   */
CHAR     *NewPage      = NULL;	  /* Zeiger auf String für "neue Seite"         */

FILE     *Datei;

WORD     AktFont;
WORD     Fonts         = 0;       /* Anzahl verfügbarer Fonts                   */

typedef  struct FontFormat
{
         CHAR FontName[10];       /* Name des Fonts, z.B. Courier_12            */
         BYTE Chars_per_inch;     /* Anzahl Zeichen pro inch, meist 10 oder 12  */
         CHAR *InitCode;          /* Zeiger auf Initialisierungscode            */
}     FONTS;

FONTS *FontZeiger;


VOID HeadLine ( VOID )
{
     clrscr();
     printf("--------------------------------------------------------------------------------");
     printf("        Mück Softwareentwicklung C - Druckertoolbox - Druckerdefinition\n\r");
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
      CHAR Dummy[4] = "0x  ";
      SCHAR *Error;
      WORD ZahlNr = 0;
      long lZahl;

      if ( Anzahl <= 0 ) return ( MM_ERROR );

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
     CHAR Hex[80];
     WORD ZahlNr = 0, Zahl, Stand;
     BYTE HexDigit = 1;
     CHAR Charac;
     CHAR Ziel[100];

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
     while ( HexDigit == 2 || Charac != CR || ZahlNr == 0 );

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
     printf("                            Werte übernehmen (J/N)?");
     return ( JaNein() );
}

CHAR MoreFonts( VOID )
{
     printf("\n\r\n\r");
     printf("                            Weitere Schriften (J/N)?");
     return ( JaNein() );
}


SWORD SaveParameters( VOID )
{
     BYTE Zahl = 0, Zahl2;

     Datei = fopen ( PrinterName, "wb" );

     if ( Datei == NULL ) return ( MM_ERROR );

     fputs( Port, Datei );
     fputc( (BYTE) 0, Datei );

     fputc( (BYTE) ( VerticalMoveUnit / 256 ), Datei );
     fputc( (BYTE) ( VerticalMoveUnit % 256 ), Datei );

     fputc( (BYTE) ( VerUnitsPerLine / 256 ), Datei );
     fputc( (BYTE) ( VerUnitsPerLine % 256 ), Datei );

     fputc( (BYTE) VerMoveKind, Datei );
     fputc( (BYTE) *(BackSpace + 1), Datei );
     fputc( (BYTE) *(NewPage + 1), Datei );

     fputc( (BYTE) *PreVerCode, Datei );
     for( Zahl=1; Zahl<=*PreVerCode; Zahl++) fputc( *(PreVerCode+Zahl), Datei );
     fputc( (BYTE) *AftVerCode, Datei );
     for( Zahl=1; Zahl<=*AftVerCode; Zahl++) fputc( *(AftVerCode+Zahl), Datei );
     fputc( (BYTE) *Reset, Datei );
     for( Zahl=1; Zahl<=*Reset; Zahl++) fputc( *(Reset+Zahl), Datei );
     fputc( (BYTE) *BoldOn, Datei );
     for( Zahl=1; Zahl<=*BoldOn; Zahl++) fputc( *(BoldOn+Zahl), Datei );
     fputc( (BYTE) *BoldOff, Datei );
     for( Zahl=1; Zahl<=*BoldOff; Zahl++) fputc( *(BoldOff+Zahl), Datei );
     fputc( (BYTE) *UnderlineOn, Datei );
     for( Zahl=1; Zahl<=*UnderlineOn; Zahl++) fputc( *(UnderlineOn+Zahl), Datei );
     fputc( (BYTE) *UnderlineOff, Datei );
     for( Zahl=1; Zahl<=*UnderlineOff; Zahl++) fputc( *(UnderlineOff+Zahl), Datei );
     fputc( (BYTE) *ItalicOn, Datei );
     for( Zahl=1; Zahl<=*ItalicOn; Zahl++) fputc( *(ItalicOn+Zahl), Datei );
     fputc( (BYTE) *ItalicOff, Datei );
     for( Zahl=1; Zahl<=*ItalicOff; Zahl++) fputc( *(ItalicOff+Zahl), Datei );

     fputc( (BYTE) Fonts, Datei );

     for( Zahl=1; Zahl<=Fonts; Zahl++)
     {
	  fputs( ( FontZeiger + Zahl - 1 ) -> FontName, Datei );
	  fputc( 0, Datei );
	  fputc( (BYTE) *( (FontZeiger + Zahl - 1 ) -> InitCode ), Datei );
	  for( Zahl2=1; Zahl2<=*( (FontZeiger + Zahl - 1 ) -> InitCode ); Zahl2++)
	       fputc( *( (FontZeiger + Zahl - 1 ) -> InitCode + Zahl2 ), Datei );
	  fputc( (BYTE) ( FontZeiger + Zahl - 1 ) -> Chars_per_inch, Datei );
     }

     if ( fclose( Datei ) != 0 ) return ( MM_ERROR );

     return ( OK );
}

SWORD GetMemory( CHAR **Ziel, WORD Menge )
{
      *Ziel = malloc( Menge );
      if (*Ziel == NULL)
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
     BYTE Zahl, Zahl2;
     CHAR Chr;

     Datei = fopen ( DName, "rb" );

     if ( Datei == NULL ) return ( MM_ERROR );

     LoadString( Port );

     VerticalMoveUnit = fgetc( Datei ) * 256 + fgetc( Datei );
     VerUnitsPerLine  = fgetc( Datei ) * 256 + fgetc( Datei );

     VerMoveKind = fgetc( Datei );

     GetMemory( &BackSpace, 2 );
     *BackSpace       = 1;
     *(BackSpace + 1) = fgetc( Datei );

     GetMemory( &NewPage, 2 );
     *NewPage        = 1;
     *(NewPage + 1 ) = fgetc( Datei );

     LoadCodes( &PreVerCode );
     LoadCodes( &AftVerCode );
     LoadCodes( &Reset );
     LoadCodes( &BoldOn );
     LoadCodes( &BoldOff );
     LoadCodes( &UnderlineOn );
     LoadCodes( &UnderlineOff );
     LoadCodes( &ItalicOn );
     LoadCodes( &ItalicOff );

     Fonts = fgetc( Datei );

     FontZeiger = calloc( Fonts, sizeof( FONTS ) );

     if ( FontZeiger == NULL )
     {
	  printf("\n\r\n\r  NOT ENOUGH MEMORY !!\n\r\n\r");
	  exit(0);
     }

     for( Zahl=1; Zahl<=Fonts; Zahl++)
     {
	  LoadString( ( FontZeiger + Zahl - 1 ) -> FontName );
	  LoadCodes( &((FontZeiger + Zahl - 1 ) -> InitCode) );
	  ( FontZeiger + Zahl - 1 ) -> Chars_per_inch = fgetc( Datei );
     }

     if ( fclose( Datei ) != 0 ) return ( MM_ERROR );

     return ( OK );
}


VOID main ( VOID )
{
     CHAR  Z[100], Charac;
     SCHAR *Zeiger;
     WORD  Zahl;

     Z[0] = 0;

     do
     {
	  HeadLine();
	  printf("Name des Druckers: ");
	  Input( PrinterName, 8 );

	  LoadParameters( PrinterName );

	  printf("\n\r\n\r");

	  printf("Port oder Dateiname (meist lpt1): ");
	  Input( Port, 8 );

	  printf("\n\r\n\r");
	  printf("Reset Codes (Hex): ");
	  EnterHexCode( &Reset, 19 );

	  printf("\n\r\n\r");
	  printf("Fettdruck an - Codes (Hex): ");
	  EnterHexCode( &BoldOn, 19 );

	  printf("\n\r\n\r");
	  printf("Fettdruck aus - Codes (Hex): ");
	  EnterHexCode( &BoldOff, 19 );

	  printf("\n\r\n\r");
	  printf("Unterstreichen an - Codes (Hex): ");
	  EnterHexCode( &UnderlineOn, 19 );

	  printf("\n\r\n\r");
	  printf("Unterstreichen aus - Codes (Hex): ");
	  EnterHexCode( &UnderlineOff, 19 );

	  printf("\n\r\n\r");
	  printf("Kursiv an - Codes (Hex): ");
	  EnterHexCode( &ItalicOn, 19 );

	  printf("\n\r\n\r");
	  printf("Kursiv aus - Codes (Hex): ");
	  EnterHexCode( &ItalicOff, 19 );
     }
     while ( YesNo() == 'N' );

     do
     {
	  HeadLine();
	  printf("Code für `1 Zeichen zurück` (Hex, meist 08): ");
	  EnterHexCode( &BackSpace, 1 );

	  printf("\n\r\n\r");
	  printf("Code für `neue Seite` (Hex, meist 0C): ");
	  EnterHexCode( &NewPage, 1 );

	  printf("\n\r\n\r");
	  printf("Minimale vertikale Bewegungseinheit in 1/n Zoll (dezimal): ");
	  do
	  {
	       if ( VerticalMoveUnit != 0 )
	       {
		    sprintf( Z, "%d", VerticalMoveUnit );
	       }
	       else
	       {
		    Z[0] = 0;
	       }
	       Input( Z, 3 );
	       VerticalMoveUnit = (WORD) strtol( Z, &Zeiger, 10 );
	       if ( (*Zeiger != 0 || VerticalMoveUnit == 0) && strlen(Z)>0 )
	       {
		    for (Zahl=0;Zahl<strlen(Z); Zahl++) printf("%c", DELETE );
		    printf("   %c%c%c", DELETE, DELETE, DELETE );
	       }
	  }
	  while ( *Zeiger != 0 || VerticalMoveUnit == 0 );

	  printf("\n\r\n\r");
	  printf("Zeilenabstand in minimalen vertikale Bewegungseinheiten (dezimal): ");
	  do
	  {
	       if ( VerUnitsPerLine != 0 )
	       {
		    sprintf( Z, "%d", VerUnitsPerLine );
	       }
	       else
	       {
		    Z[0] = 0;
	       }
	       Input( Z, 3 );
	       VerUnitsPerLine= (WORD) strtol( Z, &Zeiger, 10 );
	       if ( (*Zeiger != 0 || VerUnitsPerLine == 0) && strlen(Z)>0 )
	       {
		    for (Zahl=0;Zahl<strlen(Z); Zahl++) printf("%c", DELETE );
		    printf("   %c%c%c", DELETE, DELETE, DELETE );
	       }
	  }
	  while ( *Zeiger != 0 || VerUnitsPerLine == 0 );

	  Z[0] = 0;

	  printf("\n\r\n\r");
	  printf("Code vor vertikaler Bewegung: (Hex): ");
	  EnterHexCode( &PreVerCode, 19 );

	  printf("\n\r\n\r");
	  printf("Code nach vertikaler Bewegung (Hex): ");
	  EnterHexCode( &AftVerCode, 19 );

	  printf("\n\r\n\r");
	  printf("Zählerübergabe auf (H)P-Laser oder (E)SC-Nadel Art: ");
	  if ( VerMoveKind == MM_HP )
	  {
	       printf("HP%c%c", DELETE, DELETE);
	  }
	  else
	  if ( VerMoveKind == MM_ESC )
	  {
	       printf("ESC%c%c%c", DELETE, DELETE, DELETE );
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
	  while ( Charac != 'e' && Charac != 'E' && Charac != 'h' && Charac != 'H' );
	  if ( Charac == 'e' || Charac == 'E' )
	  {
	       VerMoveKind = MM_ESC;
	       printf("ESC");
	  }
	  else
	  {
	       VerMoveKind = MM_HP;
	       printf("HP ");
	  }
     }
     while ( YesNo() == 'N' );

     AktFont = 1;
     do
     {
	   if ( Fonts < AktFont )
	   {
		if ( Fonts == 0 )
		{
		     FontZeiger = malloc ( sizeof( FONTS) );
		}
		else
		{
		     FontZeiger = realloc( FontZeiger, ( Fonts + 1 ) * sizeof( FONTS ) );
		}
		( FontZeiger + Fonts ) -> Chars_per_inch = 0;
		( FontZeiger + Fonts ) -> FontName[0]    = 0;
		( FontZeiger + Fonts ) -> InitCode       = NULL;
	   }

	   if ( FontZeiger == NULL )
	   {
		printf("\n\r\n\r NOT ENOUGH MEMORY !! \n\r\n\r");
		exit(0);
	   }

	   HeadLine();
	   printf("Bitte editieren Sie Font %d von %d verfügbaren Fonts. Font 1 ist Standardfont", AktFont, Fonts );
	   printf("\n\r\n\r\n\r");
	   printf("Name von Font %d: ", AktFont );
	   Input( ( FontZeiger + AktFont - 1 ) -> FontName, 9 );

	   printf("\n\r\n\r");
	   printf("Zeichen pro inch (dezimal): ");
	   do
	   {
                Z[0] = 0;
                if ( ( FontZeiger + AktFont - 1 ) -> Chars_per_inch )
                   sprintf( Z, "%d", ( FontZeiger + AktFont - 1 ) -> Chars_per_inch );

		Input( Z, 3 );
		Zahl = (WORD) strtol( Z, &Zeiger, 10 );
		if ( (*Zeiger != 0 || Zahl == 0) && strlen(Z)>0 )
		{
		     for (Zahl=0;Zahl<strlen(Z); Zahl++) printf("%c", DELETE );
		     printf("   %c%c%c", DELETE, DELETE, DELETE );
		}
	   }
	   while ( *Zeiger != 0 || Zahl == 0 );
	   ( FontZeiger + AktFont - 1 ) -> Chars_per_inch = (BYTE) Zahl;

	  printf("\n\r\n\r");
	  printf("Initialisierungscode (Hex): ");
	  EnterHexCode( &( ( FontZeiger + AktFont - 1 ) -> InitCode ), 15 );

	  if ( AktFont > Fonts ) Fonts = AktFont;
	  AktFont++;
     }
     while ( MoreFonts() == 'J' && Fonts < 256 );

     Fonts = AktFont-1;

     printf("\n\r\n\r");
     printf("                            Daten speichern (J/N) ? ");
     Z[0] = JaNein();
     printf( "%c", Z[0] );
     if ( Z[0] == 'J' ) SaveParameters();
}