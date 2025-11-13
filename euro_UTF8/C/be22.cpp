// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:04 - Wed

//
//
#include <stdio.h>

void
main(void)
{
printf("%s %c", "Der String", 'A');

}



#include <iostream.h>

void peter(void);


void
main(void)
{
peter();

return;
}


void
peter(void)
{
cout << "Geben Sie eine Zahl ein: ";
int n;
cin >> n;
cout << "Die eingegebene Zahl ist: " << n;

return;
}
