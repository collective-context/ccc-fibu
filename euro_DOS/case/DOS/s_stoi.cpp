

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: S_STOI.C                                               บ
  บ  Function      : euroSOFT String-Tools                                  บ
  บ                  Standard-Stringfunktionen fuer EUR_M51x.LIB            บ
  บ                                                                         บ
  บ                                                                         บ
  บ  Date          : 04.11.1991, Graz           Update: 04.11.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/* #define TEST 1 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>           /* ANSI-C Standard fr va_start(), va_end()   */
#include <eur_tool.h>

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                            Standard-Strings                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*.ta stoi()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ      ins() - Ein Zeichen in einen String einfgen.                           บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_str.h> oder #include <eur_type.h>
PSSTR ins (cZeichen,  pstr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
CHAR   cZeichen    Zeichen das eingefgt werden soll.
PSSTR	pstr	    Zeiger auf Einfgeposition im Ausgangsstring.

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion erzeugt einen String, der lnger als der Ausgangsstring ist.
Diese Funktion geht davon aus, daแ fr den zu manipilierenden String aus-
reichend Platz reserviert ist.

Das Zeichen <cZeichen> wird an der bergebenen Adresse in den String <pstr>
eingefgt. Resultierende Lnge: strlen(pstr) + 1.

 STOI.C       Bessere Version von atoi.

  Konvertiert einen String nach Integer. Falls der String mit 0x
  beginnt, wird er als Hex-Zahl interpretiert, falls mit einer
  0 als Octalzahl, ansonsten als Dezimalzahl. Die Konversion endet
  bei dem ersten Zeichen, das keine Ziffer im angewhlten System
  darstellt. *instr wird hinter das Ende der Zahl gesetzt.



Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion gibt die Anfangsadresse des Ausgangsstrings zurck.

.de \euro\demo\ins.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

printf("Bettigen Sie ^C zum Beenden.\n");

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
   
