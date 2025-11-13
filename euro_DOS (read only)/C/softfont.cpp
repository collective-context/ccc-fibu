// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:12 - Wed

#include <stdio.h>
#include <tools.h>

PSSTR  apstrText[] = {
     "First, ensure the printer is powered ON",
     "",
     "",
     "What font ID number would you like to assign",
     "to the downloaded font?",
     "(Enter a number between 0 and 32767, then press RETURN)",
     "",
     "Font ID # =",
     NULL}

SWORD wFontId = -1;

main()
{

while(*apstrText)
  printf("%s\n", apstrText++);

while(wFontId<0 || wFontId>32767)
  sscanf("%d", wFontId);

printf ("%s%s%d%s", "\x1B", "*c", wFontId, "D");

exit(OK);

/* OPEN "IDDATA" FOR OUTPUT AS #1
   PRINT #1,FONTID
   CLOSE #1 */

}



10  OPEN "iddata" FOR INPUT AS #2
20  INPUT #2,FONTID
30  PRINT
40  PRINT
50  PRINT "Would you like the downloaded font to be"
60  PRINT "permanent or temporary?"
70  PRINT "Please enter a P (permanent) or T (temporary) & press RETURN"
80  PRINT


90  INPUT "Permanent or Temporary?  ",STATUS$
100 PRINT
110 IF STATUS$ = "P" THEN GOTO 270
120 IF STATUS$ = "p" THEN GOTO 270

130 IF STATUS$ = "T" THEN GOTO 330
140 IF STATUS$ = "t" THEN GOTO 330 ELSE GOTO 70
 
150 CLOSE #2

160 PRINT
170 PRINT "Would you like a print sample of the downloaded font?"
180 PRINT "Please enter a Y (yes) or N (no) & press RETURN?"
190 PRINT
200 INPUT "Would you like a print sample?  ",SAMPLE$
210 PRINT

220 IF SAMPLE$ = "Y" THEN GOTO 390
230 IF SAMPLE$ = "y" THEN GOTO 390

240 IF SAMPLE$ = "N" THEN GOTO 260
250 IF SAMPLE$ = "n" THEN GOTO 260 ELSE GOTO 180

260 SYSTEM

270 REM *******  MAKE FONT PERMANENT  *******
280 LPRINT CHR$(27);"*c";FONTID;"D";
290 LPRINT CHR$(27);"*c5F";
300 PRINT
310 PRINT "*** The selected font is now permanent ***"
320 GOTO 150

330 REM *******  MAKE FONT TEMPORARY  *******
340 LPRINT CHR$(27);"*c";FONTID;"D";
350 LPRINT CHR$(27);"*c4F";
360 PRINT
370 PRINT "*** The selected font is now temporary ***"
380 GOTO 150

390 REM *****  ORIENTATION AND PRINT SAMPLE  *****
400 PRINT
410 PRINT "Is the orientation of this downloaded font PORTRAIT or LANDSCAPE?"
420 PRINT "Please enter a P (Portrait) or L (Landscape) & press RETURN"
430 PRINT

440 INPUT "Font orientation?  ",ORIENT$
450 PRINT

460 IF (ORIENT$ = "p") OR (ORIENT$ = "P") THEN 700

470 IF (ORIENT$ = "l") OR (ORIENT$ = "L") THEN 680

480 PRINT
490 PRINT "Invalid orientation."
500 PRINT "please enter P (Portrait) or an L (Landscape)."
510 GOTO 430

520 LPRINT CHR$(27);"(";FONTID;"X";
530 LPRINT "This is a sample of the downloaded font."
540 LPRINT " "

550 FOR I=0 TO 7
560 FOR J=0 TO 31
570 IF ((I*32)+J) = 12 THEN 590
580 LPRINT CHR$((I * 32) + J);
590 NEXT J
600 LPRINT
610 LPRINT
620 LPRINT
630 NEXT I

640 LPRINT
650 LPRINT "The font ID for this font is ";FONTID;"."
660 LPRINT CHR$(13);CHR$(12);
670 GOTO 260

680 LPRINT CHR$(27);"&l1O";
690 GOTO 520

700 LPRINT CHR$(27);"&l0O";
710 GOTO 520



