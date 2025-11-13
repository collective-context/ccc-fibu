// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:07 - Wed

//  Beispiel 2.5
//		 Makro - inline

#include <iostream.h>

#define MAX(A,B) ( (A)>(B) ? (A) : (B) )

inline int max(int a, int b)
{
       if (a>b) return a;
       return b;
}


void
main(void)
{
int i, x=23, y=45;

i=MAX(x++, y++);    // Seiteneffekt
i=max(x++, y++);    // Ok !

return;
}
