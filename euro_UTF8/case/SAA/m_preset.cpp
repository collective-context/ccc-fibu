// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <stdio.h>
#include <string.h>
#include <eur_tool.h>


/*------ Preset_Records ( record[] ) -------------------------------------------*/
/* Diese Funktion ist eine Hilfs-funktion des Maskentools. Die Funktion muß     */
/* aufgerufen werden, wenn die angegebenen Datensätze mit den Preset-Werten     */
/* aus der Masken-Definition belegt werden sollen ( siehe Preset-Anweisung      */
/* im Masken-Compiler ).                                                        */
/* Versorgung:                                                                  */
/*    Ausgang:                                                                  */
/*    char *record[];                                                           */
/*       Gibt den vorbelegten Datensatz an.                                     */
/*       Ist der angegebene Datensatz zu kurz, so wird er entsprechend ver-     */
/*       längert und der Zwischenraum mit Low-Values (ASCII 0) aufgefüllt.      */
/*------------------------------------------------------------------------------*/
GLOBAL
SWORD M_PresetRecords (PSSTR apstrRecord[], SWORD wRecordNumber)
{
SWORD wRetCode = 0;

/*     strcpy (apstrRecord[wRecordNumber], apstrRecord[wRecordNumber]); */

apstrRecord[0]=apstrRecord[0];
wRecordNumber=wRecordNumber;

return (wRetCode);
} /* end Preset_Records () */
