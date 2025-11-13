// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <ctype.h>

int             stoi(instr)
register char   **instr;
{
	/*  Konvertiere String nach Integer und aktualisiere *instr so,
	 *  daá er hinter die Zahl zeigt. Liefer den Integer-Wert, den
	 *  der String repr„sentiert.
         */

        register int    num  = 0 ;
        register char   *str     ;
        int             sign = 1 ;

        str = *instr;

        if( *str == '-' )
        {
                sign = -1 ;
                str++;
        }

        while( '0' <= *str  &&  *str <= '9' )
                num = (num * 10) + (*str++ - '0') ;

        *instr = str;
        return( num * sign );
}
