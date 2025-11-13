// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/* STOI.C       Bessere Version von atoi.
 *
 *	Copyright (C) 1985 by Allen  Holub.  All rights reserved. 
 */

#define islower(c)	( 'a' <= (c) && (c) <= 'z' )
#define toupper(c)	( islower(c) ? (c) - ('a' - 'A') : (c) )

int		stoi(instr)
register char	**instr;
{
	/*  Konvertiert einen String nach Integer. Falls der String mit 0x
	 *  beginnt, wird er als Hex-Zahl interpretiert, falls mit einer
	 *  0 als Octalzahl, ansonsten als Dezimalzahl. Die Konversion endet
	 *  bei dem ersten Zeichen, das keine Ziffer im angewählten System
	 *  darstellt. *instr wird hinter das Ende der Zahl gesetzt.
	 */

	register int	num  = 0 ;
	register char	*str	 ;
	int		sign = 1 ;

	str = *instr;

	while(*str == ' '  ||  *str == '\t'  ||  *str == '\n' )
		str++ ;

	if( *str == '-' )
	{
		sign = -1 ;
		str++;
	}

	if(*str == '0')
	{
		++str;
		if (*str == 'x'  ||  *str == 'X')
		{
			str++;
			while(  ('0'<= *str && *str <= '9') ||
				('a'<= *str && *str <= 'f') ||
				('A'<= *str && *str <= 'F')  )
			{
				num *= 16;
				num += ('0'<= *str && *str <= '9') ?
					*str - '0' 		   :
					toupper(*str) - 'A' + 10   ;
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
	return( num * sign );
}

#ifdef TEST


main()
{
	int	num;
	char	numbuf[80], *p;

	printf("Betätigen Sie ^C zum Beenden.\n");

	while( 1 )
	{
		printf("string ( 0x.. 0.. ... ): ");
		gets( p = numbuf );
		num = stoi( &p )  ;
		printf( "<%s> = %d = 0x%x = %o Oktal   ", numbuf,num,num,num);
		printf( "Stringende = <%s>\n", p );
	}
}

#endif
   
