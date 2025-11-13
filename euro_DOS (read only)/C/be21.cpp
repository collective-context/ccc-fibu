// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:03 - Wed

// Beispiel 2.1
// Programm mit voreingestelltem Funktionsparameter

#include <iostream.h>

void show(int = 1, float = 2.3, long = 4);

void
main(void)
{
show(); 	       // standardwerte
show( 5 );	       // Argument 1
show( 5 , 7.8 );       // Argument 1 + 2
show( 5 , 9.9, 12L);   // Argument 1 + 2 + 3

return;
}

void show(int first, float sec, long third)
{
cout << " Erstens: " << first << " Zweitens " << sec << " Drittens: "
     << third << '\n';

}
