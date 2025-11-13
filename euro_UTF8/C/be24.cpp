// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:06 - Wed

//  Beispiel 2.4
//		 Bereichsauflösungsoperator

#include <iostream.h>

int zahl=123;			 // Globale Variable

void
main(void)
{
int zahl=456;			 // Lokale  Variable

cout << "\n Dies ist die globale 'zahl' : " << ::zahl;
cout << "\n Dies ist die  lokale 'zahl' : " <<	 zahl <<"\n";

return;
}
