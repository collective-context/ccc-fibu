// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:08 - Wed

//  Beispiel 2.6
//		 Qualifizierer const

#include <iostream.h>


void
main(void)
{
const int  SIZE = 5;
      char ca[SIZE];

cout << " Die Größe von ca: " << sizeof ca;

return;
}
