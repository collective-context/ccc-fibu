 #include "PRTTOOLS.C"
 #include "PCXTOOLS.C"

 void main ( void )
 {
      ptInitPrinter( "EPSON" );
      ptDefinePattern( 15, 170, 85, 170, 85, 170, 85, 170, 85 );
      ptPrintImageNow( "ESC9x120", "PT.PCX", 4, 4, 1 );
      ptClosePrinter();
 }
