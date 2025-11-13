// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:05 - Wed

//  Beispiel 2.3
//

#include <iostream.h>


void
main(void)
{
     for(int lineno=0; lineno<4; lineno++)
     {
     int temp=22;
     cout<<" Dies ist Zeilennummer " << lineno
	 <<" und temp hat den Wert " << temp++ <<"\n";
     }
     if(lineno==4) cout<<" Hoppla !  LineNo. 4 ?\n";
return;
}
