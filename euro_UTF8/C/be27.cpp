// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:09 - Wed

//  Beispiel 2.7
//		 enum Datentypen

#include <iostream.h>

enum ignition_parts
{
distributor=1, cap, points, done
};


void
main(void)
{
int choice;
ignition_parts ip;

do
{
   cout << "\n Eingabe einer Nummer: (1..3 / 4=Ende): ";
   cin	>>  choice;
   ip=	(ignition_parts)choice;

   switch(ip)
   {
      case distributor:
	   cout << "\n Distributor         \n";
	   break;
      case cap:
	   cout << "\n Distributor cap     \n";
	   break;
      case points:
	   cout << "\n Distributor points  \n";
	   break;
      case done:
	   cout << "\n     Ende.  baba !   gemma ham ....\n";
	   break;
      default:
	   cout << "\n" << choice << " ist eine ungültige Nummer !!!\n";
	   break;
   }
}while(ip!=done);
return;
}
