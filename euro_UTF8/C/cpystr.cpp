// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:25 - Wed

/* CPYSTR.C illustrates memory and string copy functions including:
 *      _memccpy        memcpy          memmove
 *      strncpy         strcpy          _strdup          strlen
 */

#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

char string1[60] = "The quick brown dog jumps over the lazy fox";
char string2[60] = "The quick brown fox jumps over the lazy dog";
/*                           1         2         3         4         5 *
 *                  12345678901234567890123456789012345678901234567890 */
void main()
{
    char buffer[61];
    char *pdest, *newstring;
    int  pos;

    printf( "Function:\t_memccpy 60 characters or to character 's'\n" );
    printf( "Source:\t\t%s\n", string1 );
    pdest = _memccpy( buffer, string1, 's', 60 );
    *pdest = '\0';
    printf( "Result:\t\t%s\n", buffer );
    printf( "Length:\t\t%d characters\n\n", strlen( buffer ) );

    pos = pdest - buffer;
    printf( "Function:\tstrcpy\n" );
    printf( "Source:\t\t%s\n", string2 + pos );
    pdest = strcpy( buffer + pos, string2 + pos );
    printf( "Result:\t\t%s\n", buffer );
    printf( "Length:\t\t%d characters\n\n", strlen( buffer ) );

    printf( "Function:\tmemcpy 20 characters\n" );
    printf( "Source:\t\t%s\n", string2 );
    memcpy( buffer, string2, 20 );
    printf( "Result:\t\t%s\n", buffer );
    printf( "Length:\t\t%d characters\n\n", strlen( buffer ) );

    printf( "Function:\tstrncpy 30 characters\n" );
    printf( "Source:\t\t%s\n", string1 + 20 );
    pdest = strncpy( buffer + 20, string1 + 20, 30 );
    printf( "Result:\t\t%s\n", buffer );
    printf( "Length:\t\t%d characters\n\n", strlen( buffer ) );

    _getch();

    printf( "Function:\t_strdup\n" );
    printf( "Source:\t\t%s\n", buffer );
    newstring = _strdup( buffer );
    printf( "Result:\t\t%s\n", newstring );
    printf( "Length:\t\t%d characters\n\n", strlen( newstring ) );

    /* Illustrate overlapping copy: memmove handles it correctly,
     * memcpy does not.
     */
    printf( "Function:\tmemcpy with overlap\n" );
    printf( "Source:\t\t%s\n", string1 + 4 );
    printf( "Destination:\t%s\n", string1 + 10 );
    memcpy( string1 + 10, string1 + 4, 40 );
    printf( "Result:\t\t%s\n", string1 );
    printf( "Length:\t\t%d characters\n\n", strlen( string1 ) );

    printf( "Function:\tmemmove with overlap\n" );
    printf( "Source:\t\t%s\n", string2 + 4 );
    printf( "Destination:\t%s\n", string2 + 10 );
    memmove( string2 + 10, string2 + 4, 40 );
    printf( "Result:\t\t%s\n", string2 );
    printf( "Length:\t\t%d characters\n\n", strlen( string2 ) );
}
