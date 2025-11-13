// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: datetime.c 					    ║
  ║  Function	   : FiCore Datum-Tool				    ║
  ║		     Standard-Datumsfunktionen für DOS_M51x.LIB 	    ║
  ║                                                                         ║
  ║  Date	   : 01.07.1991, Graz		Update: 06.07.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <dos.h>

/*#include <eur_dos.h>
  #include <dos.h>
  #include <malloc.h>
  #include <stdio.h>*/

#define GET_DATE 0x2A
#define SET_DATE 0x2B
#define GET_TIME 0x2C
#define SET_TIME 0x2D

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ D_get_date()							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Function GET_DATE(2Ah) - Liest das Systemdatum ein. 		   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PD_DATE D_get_date (PD_DATE pDatum)
{
union REGS outregs;

if(pDatum==NULL)		 /* Falls Aufrufer NULL übergibt fordert die */
  Ut_Calloc(pDatum, 1, D_DATE);  /* Funktion den nötigen Speicher an	     */

outregs.h.ah=GET_DATE;
intdos(&outregs, &outregs);

pDatum->wJahr =outregs.x.cx;		/* Jahr (1980 - ????) */
pDatum->cMonat=outregs.h.dh;		      /* Monat (1-12) */
pDatum->cTag  =outregs.h.dl;			/* Tag (1-31) */
pDatum->cWTag =outregs.h.al;	 /* Tag der Woche (0=Sonntag) */

return(pDatum); 		 /* Gibt die Adresse der Struktur zurück */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ D_set_date()							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Function SET_DATE(2Bh) - Setzt das Systemdatum.			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD D_set_date (PD_DATE pDatum)
{
union REGS inregs;

inregs.h.ah=SET_DATE;			    /* Füllen der Register mit */
inregs.x.cx=pDatum->wJahr;		    /* dem gewünschten Datum   */
inregs.h.dh=pDatum->cMonat;
inregs.h.dl=pDatum->cTag;

intdos(&inregs, &inregs);
return(inregs.h.al);		     /* 00 = gültiges Datum, FFh = ungültig */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ D_get_time()							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Function GET_TIME(2Ch) - Liest die Systemzeit ein.			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PD_TIME D_get_time (PD_TIME pZeit)
{
union REGS outregs;

if(pZeit==NULL) 		 /* Falls Aufrufer NULL übergibt fordert die */
  Ut_Calloc(pZeit, 1, D_TIME);	 /* Funktion den nötigen Speicher an	     */

outregs.h.ah=GET_TIME;
intdos(&outregs, &outregs);

pZeit->cStd =outregs.h.ch;		       /* Stunde (0-23) */
pZeit->cMin =outregs.h.cl;		       /* Minute (0-59) */
pZeit->cSek =outregs.h.dh;		       /* Sekunde (0-59) */
pZeit->cHSek=outregs.h.dl;		       /* Hunderstel (0-99) */
return(pZeit);				       /* Pointer auf die Struktur */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ D_set_time()							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Function SET_TIME(2Dh) - Setzt die Systemzeit.			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD D_set_time (PD_TIME pZeit)
{
union REGS inregs;

inregs.h.ah=SET_TIME;			/* Füllen der Register mit */
inregs.h.ch=pZeit->cStd;		/* der gewünschten Zeit    */
inregs.h.cl=pZeit->cMin;
inregs.h.dh=pZeit->cSek;
inregs.h.dl=pZeit->cHSek;

intdos(&inregs, &inregs);
return(inregs.h.al);
}
