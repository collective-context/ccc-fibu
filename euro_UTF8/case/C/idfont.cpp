// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#include <stdio.h>
#include <tools.h>

PSSTR apstrText[] = {
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
