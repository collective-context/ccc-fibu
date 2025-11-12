// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: HP_FCOPY						    บ
  บ  Funktion	  : euroSOFT-FONTcopy					    บ
  บ		    Kopiert Fonts fr HP-Laser-Jet			    บ
  บ                                                                         บ
  บ  Datum	  : 07.08.1990, Graz	       Update:	 .  .	 , Graz     บ
  บ  Author       :                            Author:                      บ
  บ  Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <types.h>
#include <stat.h>
#include <eur_type.h>

#define ID_START "\x1B*c"
#define ID_END	 "D"

SWORD main (SWORD argc, PSSTR argv[])
{
FILE *pfOutDevice;
CHAR strOut[128];

PSSTR pstrIdNummer=argv[1];
PSSTR pstrFontDatei=argv[2];
PSSTR pstrOutDevice=argv[3];

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      PROGRAMMAUFRUF BERPRFEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
if(argc != 4)
  {
  printf ("\a\n!!! Fehler:"
	  "\neuroSOFT-FONTcopy Rel. 1.00\n"
	  "\nSyntax:   HP_FCOPY  <ID-Nummer>  <[Pfad]Font-Datei>  <OUT-DEVICE>\n"
	  "\nBeispiel: HP_FCOPY  80  \\euro\\fnt\\PC08PN  LPT1\n\n");
  exit (ERROR);
  }

if(!(pfOutDevice=fopen(pstrOutDevice, "w")))   /* Out-Device ”ffnen */
  {
  printf ("\nOut-Device <%s> kann nicht ge”ffnet werden", pstrOutDevice);
  exit (ERROR);
  }
fprintf(pfOutDevice, "%s%s%s", ID_START, pstrIdNummer, ID_END);

fflush(pfOutDevice);
if(fclose(pfOutDevice)) 			 /* Eingabe - Datei schlieแen */
  {printf ("\nOut-Device <%s> kann nicht "
     "geschlossen werden", pstrOutDevice);
  exit (ERROR);}

printf("\nAuf ID-Nr: ฏ%sฎ wird FONT ฏ%sฎ kopiert ...\n", pstrIdNummer, pstrFontDatei);
sprintf(strOut, "copy %s /b %s", pstrFontDatei, pstrOutDevice);
system(strOut);

return(OK);
}


/*
SWORD wFontDatei;
CHAR strInput[1600];
PSSTR pstr=strInput;

if( (wFontDatei=open(pstrFontDatei, O_RDONLY | O_BINARY)) <= NULL)
  {
  printf ("\nFont-Datei <%s> kann nicht ge”ffnet werden", pstrFontDatei);
  exit (ERROR);
  }

sprintf(strOut, "%s%s%s", ID_START, pstrIdNummer, ID_END);
write(wOutDevice, strOut, strlen(strOut));

printf("\nAuf ID-Nr: ฏ%sฎ wird FONT ฏ%sฎ kopiert ", pstrIdNummer, pstrFontDatei);
while( !eof(wFontDatei) )
  {
  pstr=strInput;
  for(i=0; i<1500 && !eof(wFontDatei); i++)
     read(wFontDatei, pstr++, 1);

  write(wOutDevice, strInput, i);
  printf(".");
  }
printf(" OK!\n");

if ( ! close(wFontDatei) )
   {
   printf ("\nFont-Datei <%s> kann nicht geschlossen werden !!\n", pstrFontDatei);
   exit (ERROR);
   }

if ( ! fclose(wOutDevice) )
   {
   printf ("\nAusgabe-Device <%s> kann nicht geschlossen werden !!\n", pstrOutDevice);
   exit (ERROR);
   }

*/
