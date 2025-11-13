// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:00 - Wed

// Beispiel 1.1

#include <iostream.h>

void hello(void);
void name (void);
void peter(void);


void
main(void)
{

hello();

name ();

peter();

int zahl = 123;
cout << "Irgendeine Zahl ist zum Beispiel: " << zahl;

cout <<' '<< dec << zahl <<' '<< oct << zahl <<' '<< hex << zahl <<'\n';

return;
}


void
hello(void)
{
cout << "Hello world\n";

return;
}

void
name (void)
{
char name[20];					       // String-Declaration
cout << "Geben Sie bitte Ihren Namen ein: ";	  // Eingabeaufforderung
cin  >>  name;					       // Namenseingabe
cout << "       Der eingegebene Name ist: "<< name <<'\n'; // Kontrollausgabe

return;
}


void
peter(void)
{
cout << " Geben Sie eine Zahl ein: ";
int n;
cin >> n;
cout << "Die eingegebene Zahl ist: " << n <<'\n';

return;
}
