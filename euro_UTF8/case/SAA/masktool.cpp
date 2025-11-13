// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#include <stdio.h>
#include <eur_tool.h>

/*------ Maskentool ------------------------------------------------------------*/


/*------ Block_Defaults () -----------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Block_Defaults ()
{

} /* end Block_Defaults () */

/*------ Block_Input () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Block_Input ()
{
} /* end Block_Input () */


/*------ Change_Mask () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Change_Mask ()
{
} /* end Change_Mask () */


/*------ Cear_All_Blocks () ----------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Cear_All_Blocks ()
{
} /* end Cear_All_Blocks () */



/*------ Close_Printer () ------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Close_Printer ()
{

} /* end Close_Printer () */



/*------ Close_Window () -------------------------------------------------------*/
/* Diese Funktion schließt das zuletzt eröffnete Window.                        */
/*------------------------------------------------------------------------------*/
Close_Window ()
{
int err;

return (err);
} /*end Close_Window ()*/




/*------ Delete_Block () -------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Delete_Block ()
{

} /* end Delete_Block () */


/*------ Does_Fit () -----------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Does_Fit ()
{
} /* end Does_Fit () */


/*------ Edit_Field () ---------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Edit_Field ()
{
} /* end Edit_Field () */



/*------ Finish_Memo () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Finish_Memo ()
{

} /* end Finish_Memo () */

/*------ Get_Next_Memo_Line () -------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Get_Next_Memo_Line ()
{

} /* end Get_Next_Memo_Line () */


/*------ Insert_Block ----------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Insert_Block ()
{

} /* end () */



/*------ Key_Input () ----------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Key_Input ()
{

} /* end () */


/*------ Kill_Memo ( memofile_name, memo_key ) ---------------------------------*/
/* Diese Funktion ist ein Bestandteil des Memo-Systems. Mit ihr kann man ein    */
/* komplettes Memo aus einer Memo-Bibliothek löschen. Ist der angegebene        */
/* memoschlüssel nicht in der Bibliothek verzeichnet, so geschieht gar nichts.  */
/* Versorgung:                                                                  */
/*    Eingang:                                                                  */
/*    char *memofile_name;                                                      */
/*       Name der Memo-Bibliothek. Ist die Memobibliothek nicht vorhanden, so   */
/*       wird sie erzeugt, auch wenn mann in einer leeren Bibliothek nichts     */
/*       löschen kann.                                                          */
/*    char *memo_key;                                                           */
/*       Schlüssel des Memos, das gelöscht werden soll. Wird der Schlüssel in   */
/*       der Bibliothek nicht gefunden, wird sofort in den aufrufenden          */
/*       Programmteil zurückgesprungen.                                         */
/*------------------------------------------------------------------------------*/
Kill_Memo ( memofile_name, memo_key )
char *memofile_name, *memo_key;
{
int err;
return (err);
} /*end Kill_Memo ()*/


/*------ Make_Choise () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Make_Choise ()
{

} /* end Make_Choise () */



/*------ Make_Window () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Make_Window ()
{

} /* end Make_Window () */





/*------ Memo ( memofile_name, memo_key, right_margin, usage, message_str, -----*/
/*              question, changable, window_palette, frame_palette,             */
/*              default_upper_line, default_upper_col, default_width,           */
/*              default_hight )                                                 */
/* Diese Funktion aktiviert das Memo-System. Der Memo-Editor ist ein in sich    */
/* abgeschlossenes Datei- und Textverwaltungssystem. Über den Editor lassen     */
/* sich verschiedene Sonderfunktionen aufrufen, mit denen die Texte verwaltet   */
/* werden können. Der Rücksprung zum aufrufenden Programmteil erfolgt erst,     */
/* wenn der Anwender das Memo explizit durch ESC verlassen hat.                 */
/* Versorgung:                                                                  */
/*    Eingang:                                                                  */
/*    char *memofile_name;                                                      */
/*       Gibt den Dateinamen der Memo-Bibliothek an, die angesprochen werden    */
/*       soll. Der Dateiname muß ein DOS-konformer Dateiname (evtl. mit         */
/*       Pfadangaben) sein. Die Memo-Bibliothek wird erzeugt, falls sie noch    */
/*       nicht exisitiert.                                                      */
/*    char *memo_key;                                                           */
/*       Gibt den Schlüssel des memos in der Memo-Bibliothek an. Ist das Memo   */
/*       dort noch nicht verzeichnet, kann das Memo auf Wunsch des Anwenders    */
/*       dort eingetragen werden. In diesem Fall wird das Default-Memo ange-    */
/*       zeigt, welches leer ist und an der bezeichneten Stelle am Bildschirm   */
/*       erscheint. Der Memo-Schlüssel wird nur auf 35 Stellen ausgewertet.     */
/*    int right_margin;                                                         */
/*       Legt den rechten Rand fest, bis zu dem der Benutzer eine Zeile einge-  */
/*       ben kann, ehe sie automatisch umgebrochen wird. Der rechte Rand ist    */
/*       fast beliebig wählbar, sollte jedoch auf sinnvolle Werte beschränkt    */
/*       werden. Um den rechten Rand praktisch freizugeben kann z.B. ein Wert   */
/*       von 9999 angegeben werden.                                             */
/*    char *usage;                                                              */
/*       Gibt die Zeichenkette an, die oben im Rahmen des Memos eingeblendet    */
/*       werden soll, um auf die Funktion des Memos hinzuweisen. Ist das        */
/*       Fenster schmäler als die angegebene Zeichenkette, wird diese ent-      */
/*       sprechend rechts abgeschnitten.                                        */
/*    char *message_str;                                                        */
/*       Gibt die Nachricht an, die ausgegeben werden soll, wenn der angege-    */
/*       bene Memo-Schlüssel nicht in der Memo-Bibliothek vorhanden ist.        */
/*    char *question;                                                           */
/*       Gibt die Frage an, die gestellt werden soll, damit der Anwender ent-   */
/*       scheiden kann, ob ein noch nicht existierendes Memo angelegt werden    */
/*       soll. Die Frage wird allerdings nur dann gestellt, wenn der Anwender   */
/*       auch berechtigt ist, das Memo zu verändern (s.u.).                     */
/*    int changable;                                                            */
/*       Gibt an, ob der Anwender das Memo verändern (editieren) oder anlegen   */
/*       kann, falls es noch nicht vorhanden ist. Ansonsten wird der Memoinhalt */
/*       nur angezeigt. Soll der Anwender das Memo nicht verändern können und   */
/*       exisitert das bezeichnete Memo noch nicht, so erscheint dem Anwender   */
/*       nur die in message angegebene Meldung. Der Parameter question ist in   */
/*       diesem Fall ohne Bedeutung.                                            */
/*       Der Wert 0 für changable bedeutet, daß das memo nicht verändert werden */
/*       kann (nur angezeigt wird).                                             */
/*       Der Wert -1 bedeutet, daß das Memo editiert werden kann.               */
/*       Der Wert  bedeutet, daß das Memo nicht verändert werden kann und daß   */
/*       bei Nichtexistenz des Memos keinerlei Anzeige erfolgt, sonder direkt   */
/*       zum aufrufenden Programmteil zurückgesprungen wird. Andere Werte sind  */
/*       nicht zulässig.                                                        */
/*    int window_palette;                                                       */
/*       Ist die Nummer der Palette für die Farbattributen des Fensters         */
/*    int frame_palette;                                                        */
/*       Ist die Nummer der Palette für die Farbattributen des Fensterrahmens   */
/*    int default_upper_line;                                                   */
/*       Gibt die Zeilenkoordinate der linken oberen Ecke an, an der das        */
/*       Fenster eröffnet werden soll, falls das Memo noch nicht existiert.     */
/*    int default_upper_col;                                                    */
/*       Gibt die Spaltenkoordinate der linken oberen Ecke an, an der das       */
/*       Fenster eröffnet werden soll, falls das Memo noch nicht existiert.     */
/*       Die linke obere Bildschirmecke hat die Koordinaten 1/1.                */
/*    int default_width;                                                        */
/*       Gibt an, wie breit das Fenster eröffnet werden soll.                   */
/*    int default_hight;                                                        */
/*       Gibt an, wie hoch das Fenster eröffnet werden soll.                    */
/*------------------------------------------------------------------------------*/
Memo ( memofile_name, memo_key, right_margin, usage, message_str,
       question, changable, window_palette, frame_palette,
       default_upper_line, default_upper_col, default_width,
       default_hight )
int right_margin, changable, window_palette, frame_palette,
       default_upper_line, default_upper_col, default_width, default_hight;
char *memofile_name, *memo_key, *usage, *message_str, *question;
{

} /* end Memo () */


/*------ Memo_Create () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Memo_Create ()
{

} /* end Memo_Create () */


/*------ Memo_Match () ---------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Memo_Match()
{

} /* end Memo_Match () */


/*------ Memo_Open () ----------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Memo_Open ()
{

} /* end () */



/*------ Open_Printer () -------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Open_Printer ()
{

} /* end Open_Printer () */


/*------ Open_Window () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Open_Window ()
{

} /* end Open_Window () */


/*------ Output_Blocks () ------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Output_Blocks ()
{
} /* end Output_Block () */




/*------ Print_Line () ---------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Print_Line ()
{

} /* end Print_Line () */



/*------ Print_String () -------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Print_String ()
{

} /* end Print_String () */



/*------ Put_Next_Memo_Line () -------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Put_Next_Memo_Line ()
{

} /* end Put_Next_Memo_Line () */


/*------ Switch_Back () --------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Switch_Back ()
{

} /* end Switch_Back () */



/*------ Waiting () ------------------------------------------------------------*/
/* Diese Funktion ist eine interne Funktion des Masken-Interpreters, deren      */
/* Nutzung aber u.U. auch außerhalb des Masken-Interpreters interessant sein    */
/* kann.                                                                        */
/* Diese Funktion wartet darauf, daß der Anwender eine der Tasten Enter,        */
/* ESC oder Leerschritt.                                                        */
/*------------------------------------------------------------------------------*/
Waiting ()
{

} /* end () */



/*------ Toolbox - Datadictionary ----------------------------------------------*/


/*------ DD_Complet_Info () ----------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
DD_Complet_Info ()
{

} /* end DD_Complet_Info () */


/*------ DD_File_Info () -------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
DD_File_Info ()
{

} /* end DD_File_Info () */


/*------ DD_Index () -----------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
DD_Index ()
{

} /* end DD_Index () */


/*------ DD_Open () ------------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
DD_Open ()
{

} /* end DD_Open () */


/*------ Toolbox - Message -----------------------------------------------------*/


/*------ Chain_Message () ------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Chain_Message ()
{

} /* end Chain_Message () */





/*------ Locked_Retry_Message () -----------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Locked_Retry_Message ()
{

} /* end Locked_Retry_Message () */


/*------ Ok_Message () ---------------------------------------------------------*/
/*                                                                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
Ok_Message ()
{

} /* end Ok_Message () */




/*------ Toolbox - BuissnesGrafik ----------------------------------------------*/
