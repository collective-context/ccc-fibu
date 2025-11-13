

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: S_STOI.C                                               ║
  ║  Function      : FiCore String-Tools                                  ║
  ║                  Standard-Stringfunktionen fuer EUR_M51x.LIB            ║
  ║                                                                         ║
  ║                                                                         ║
  ║  Date          : 04.11.1991, Graz           Update: 04.11.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/* #define TEST 1 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>           /* ANSI-C Standard für va_start(), va_end()   */
#include <eur_tool.h>

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                            Standard-Strings                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/

/*.ta stoi()
╔══════════════════════════════════════════════════════════════════════════════╗
║      ins() - Ein Zeichen in einen String einfügen.                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_str.h> oder #include <eur_type.h>
PSSTR ins (cZeichen,  pstr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
CHAR   cZeichen    Zeichen das eingefügt werden soll.
PSSTR	pstr	    Zeiger auf Einfügeposition im Ausgangsstring.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erzeugt einen String, der länger als der Ausgangsstring ist.
Diese Funktion geht davon aus, daß für den zu manipilierenden String aus-
reichend Platz reserviert ist.

Das Zeichen <cZeichen> wird an der übergebenen Adresse in den String <pstr>
eingefügt. Resultierende Länge: strlen(pstr) + 1.

 STOI.C       Bessere Version von atoi.

  Konvertiert einen String nach Integer. Falls der String mit 0x
  beginnt, wird er als Hex-Zahl interpretiert, falls mit einer
  0 als Octalzahl, ansonsten als Dezimalzahl. Die Konversion endet
  bei dem ersten Zeichen, das keine Ziffer im angewählten System
  darstellt. *instr wird hinter das Ende der Zahl gesetzt.



Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Die Funktion gibt die Anfangsadresse des Ausgangsstrings zurück.

.de \euro\demo\ins.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD stoi(register char **instr)
{
register SWORD	num  = 0 ;
register CHAR   *str     ;
SWORD		sign = 1 ;

str=*instr;
while(*str == ' '  ||  *str == '\t'  ||  *str == '\n' )
  str++ ;

if(*str == '-')
  {sign= -1 ; str++;}

if(*str == '0')
  {
  ++str;
  if(*str == 'x'  ||  *str == 'X')
    {
    str++;
    while( ('0'<= *str && *str <= '9') ||
      ('a'<= *str && *str <= 'f') ||
      ('A'<= *str && *str <= 'F')  )
      {
      num *= 16;
      num += ('0'<= *str && *str <= '9')
        ? *str - '0'
        : toupper(*str) - 'A' + 10;
      str++;
      }
    }
  else
    {
    while( '0' <= *str  &&  *str <= '7' )
      {
      num *= 8;
      num += *str++ - '0' ;
      }
    }
  }
else
  {
  while( '0' <= *str  &&  *str <= '9' )
    {
    num *= 10;
    num += *str++ - '0' ;
    }
  }

*instr = str;
return(num * sign);
}


#ifdef TEST
main()
{
SWORD wNum;
CHAR strNumBuf[80];
PSSTR pstr;

printf("Betätigen Sie ^C zum Beenden.\n");

while(1)
  {
  printf("string ( 0x.. 0.. ... ): ");
  gets(pstr=strNumBuf);
  wNum=stoi( &pstr )  ;

  printf("<%s> = %d = 0x%x = %o Oktal   ", strNumBuf,wNum,wNum,wNum);
  printf("Stringende = <%s>\n", pstr );
  }

return(OK);
}
#endif
   
