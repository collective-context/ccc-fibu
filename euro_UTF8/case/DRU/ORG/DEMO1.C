#include "PRTTOOLS.C"
char Ausgabe[6][3][13]={ { "Grand Plaza",  "0123/4567", "*****"},
                             { "Morning Star", "3210/7654", "****" },
                             { "Zum Trüffel",  "7654/3210", "***"  },
                             { "Haus Müller",  "1020/3040", "**"   },
                             { "Zum Stroh",    "0102/0304", "*"    },
                             { "Zum Wirt",     "1122/3344", "-"    }
                           };
int Count;
void main ( void )
{
  ptInitPrinter( "EPSON" );
  ptPrintAutoNow( "_Hotelliste_", 0, ptGetDoubleInfo( PT_HORLENGTH ),
                  PT_CENTER );

  ptSkip( 3 );

  for( Count = 0; Count < 6; Count++)
  {
       ptPrintLeftNow( Ausgabe[Count][0], 0 );
       ptPrintLeftNow( Ausgabe[Count][1], 8.5 );
       ptPrintLeftNow( Ausgabe[Count][2], 14 );
       ptSkip(1);
  }
  ptClosePrinter();
}
