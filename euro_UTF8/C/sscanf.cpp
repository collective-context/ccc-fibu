// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:13 - Wed

#include <stdio.h>

main()
{
int eingabe, i;
char buffer[30];

strcpy (buffer, "  3  12  14  15  16");

for (i=0; i<20; i+=4)
   {
   sscanf (&buffer[i], "%3d", &eingabe);
   printf ("\n%d", eingabe+1);
   }
}
