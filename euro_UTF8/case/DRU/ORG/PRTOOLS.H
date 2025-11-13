/*----------------------------------------------------------------------------
                 Tools.h wird von der ptToolbox benîtigt
----------------------------------------------------------------------------*/

  typedef  signed   int           SWORD;
  typedef  signed char            CHAR;
  typedef  unsigned char          BYTE;
  typedef  signed   char          SBYTE;
  typedef  unsigned int           WORD;
  typedef  unsigned long          LONG;
  typedef  unsigned char          BOOL;
  typedef  float                  FLOAT;
  typedef  double                 DOUBLE;
  typedef  unsigned long int      LONG;
  typedef  void                   VOID;

  typedef  struct ColumnFormat
  {
           WORD   CNumber;                /* Nummer der Spalte                          */
           DOUBLE Left;                   /* in inch !                                  */
           DOUBLE Right;                  /* in inch !                                  */
  }        COLUMNS;

  typedef  struct TRAPstruct
  {
           WORD   Line;                   /* Zeilennummer                               */
           BOOL   Activated;              /* Trap aktiviert ?                           */
           VOID   ( *ptFunc )( );         /* Zeiger auf Funktion                        */
  }        TRAP;

  typedef  struct BUFFERstruct
  {
           CHAR   *Text;                  /* Zeiger auf Speicherplatz, der fÅr Text reserviert ist */
           BOOL   BoldPrint;              /* TRUE: Fettdruck an, FALSE: aus             */
           BOOL   Underline;              /* TRUE: Unterstreichen an, FALSE: aus        */
           BOOL   Italic;                 /* TRUE: Kursiv an, FALSE: aus                */
           WORD   Font;                   /* Nummer des Fonts, Standard: 0              */
           BYTE   Ausrichtung;            /* PT_LEFT, PT_CENTER, ..                     */
           WORD   Line;                   /* Line gibt die Zeilennummer an, ExactVerPos die Anzahl */
           SWORD  ExactVerPos;            /* minimaler Bewegungsschritte von Line nach unten, ExactVerPos < VerUnitsPerLine !!!! */
           DOUBLE Left;                   /* in inch !                                  */
           DOUBLE Right;                  /* in inch !                                  */
  }        BUFFER;

  typedef  struct FontFormat
  {
           CHAR FontName[10];             /* Name des Fonts, z.B. Courier_12            */
           BYTE Chars_per_inch;           /* Anzahl Zeichen pro inch, meist 10 oder 12  */
           CHAR *InitCode;                /* Zeiger auf Initialisierungscode            */
  }        FONTS;

  #define  PT_TRUE                  0     /* Schalter: Wahr                             */
  #define  PT_FALSE                 1     /* Schalter: Unwahr                           */
  #define  PT_OK                    0     /* Antwort: Auftrag erfolgreich beendet       */
  #define  PT_ERROR                 1     /* Antwort: Fehler aufgetreten                */
  #define  PT_CR                    13    /* CARRIGE RETURN: ZurÅck zum linken Blattrand*/
  #define  PT_LF                    10    /* LINE FEED: Neue Zeile                      */
  #define  PT_SPACE                 32    /* ASCII-Code fÅr Leerzeichen                 */
  #define  PT_DELETE                8     /* ASCII-Code fÅr Backspace                   */
  #define  PT_NOEND                 32000 /* Endlospapier hat (hier) 32000 Zeilen       */
  #define  PT_PCXKENNUNG            10    /* Kennungsbyte einer PCX-Datei               */
  #define  PT_EOS                   0     /* END OF STRING: Endbyte eines Strings       */
  #define  PT_HP                    5     /* HP-Laserdrucker                            */
  #define  PT_ESC_9                 6     /* Epson 9-Nadler                             */
  #define  PT_ESC_24                7     /* Epson 24-Nadler                            */
  #define  PT_ESC                   6     /* Epson-9 oder 24-Nadler                     */
  #define  PT_PCX                   8     /* PCX-File                                   */
  #define  PT_PCC                   9     /* PCC-File                                   */
  #define  PT_CM                    1     /* Einheit: cm                                */
  #define  PT_INCH                  2     /* Einheit: inch = 2,54 cm                    */
  #define  PT_RIGHT                 1     /* Ausrichtung: RechtsbÅndig                  */
  #define  PT_LEFT                  3     /* Ausrichtung: LinksbÅndig                   */
  #define  PT_CENTER                2     /* Ausrichtung: Zentriert                     */
  #define  PT_JUSTIFY               4     /* Ausrichtung: Blocksatz                     */
  #define  PT_ON                    1     /* Schalter: Aktivieren                       */
  #define  PT_OFF                   0     /* Schalter: Deaktivieren                     */
  #define  PT_BOLD                  1     /* Schriftextras: Fettdruck                   */
  #define  PT_UNDERLINE             2     /* Schriftextras: Unterstreichen              */
  #define  PT_ITALIC                3     /* Schriftextras: Kursivdruck                 */
  #define  PT_LINE                  7     /* Aktuelle Zeile                             */
  #define  PT_MEASUREUNIT           8     /* Aktuelle Ma·einheit (PT_CM, PT_INCH)       */
  #define  PT_NUMBEROFCOLUMNS       9     /* Anzahl Spalten                             */
  #define  PT_BUFFERENTRIES         10    /* Buffer-EintrÑge                            */
  #define  PT_COLUMN                11    /* Aktuelle Spalte                            */
  #define  PT_NUMBEROFTRAPS         12    /* Anzahl vorhandener TRAPS                   */
  #define  PT_VERUNITS              13    /* Aktuelle Zahl minimaler Bewegungseinheiten */
  #define  PT_PAGESPRINTED          14    /* Anzahl gedruckter Seiten                   */
  #define  PT_LINESPERPAGE          15    /* Anzahl Zeilen pro Seite                    */
  #define  PT_SPECIALCHARS          16    /* "^_" an/aus ?                              */
  #define  PT_VERUNIT               17    /* Kleinstmîgliche vertikale Bewegungseinheit */
  #define  PT_VERUNITSPERLINE       18    /* Notwendige min. Bew.einheiten fÅr 1 Zeile  */
  #define  PT_PRINTERTYPE           19    /* PT_HP oder PT_ESC                          */
  #define  PT_AKTFONT               20    /* Nummer des aktuellen Fonts                 */
  #define  PT_NUMBEROFFONTS         21    /* Anzahl vorhandener Schriften               */
  #define  PT_CHARSPERINCH          22    /* Zeichen pro Inch des aktuellen Fonts       */
  #define  PT_PRINTER               23    /* Name des Druckers                          */
  #define  PT_PORT                  24    /* Druckerport                                */
  #define  PT_FONTNAME              25    /* Name des aktuellen Fonts                   */
  #define  PT_SMALLESTFONT          26    /* Kleinster Font                             */
  #define  PT_BIGGESTFONT           27    /* Grî·ter Font                               */
  #define  PT_CHARSPERLINE          28    /* Zeichen pro Zeile                          */
  #define  PT_HORLENGTH             29    /* LÑnge einer Zeile                          */
  #define  PT_NOUNITCHOSEN          -1    /* Keine Einheit gewÑhlt                      */

  /* Fehlermeldungen */

  #define  PT_FILEERROR             -1    /* Datei konnte nicht geîffnet werden          */
  #define  PT_NOPCXFILE             -2    /* Es handelt sich nicht um eine PCX-Datei     */
  #define  PT_PCXFORMATNOTSUPPORTED -3    /* Dieses PCX-Format wird nicht unterstÅtzt    */
  #define  PT_PRINTERERROR          -4    /* Drucker lÑ·t sich nicht ansprechen          */
  #define  PT_NOTENOUGHSPACE        -5    /* Nicht genug Platz vorhanden                 */
  #define  PT_UNITNOTKNOWN          -6    /* Angegebene Einheit existiert nicht          */
  #define  PT_ALREADYDEFINED        -7    /* TRAP o.Ñ. schon definiert                   */
  #define  PT_NOTENOUGHMEMORY       -8    /* Nicht genug Speicher vorhanden              */
  #define  PT_COLUMNNOTDEFINED      -9    /* Spalte mit dieser Nummer existiert nicht    */
  #define  PT_WRONGMARGINS          -10   /* SpaltenÅberschneid. (li/re vertauscht o.Ñ.) */
  #define  PT_WRONGMETHOD           -11   /* Falsche Methode (falsche Ausrichtung, o.Ñ.) */
  #define  PT_WRONGCOLUMN           -12   /* Falsche Spalte                              */
  #define  PT_WRONGPARAMETER        -13   /* Falsche Parameter Åbergeben                 */
  #define  PT_TRAPNOTDEFINED        -14   /* TRAP nicht definiert                        */
  #define  PT_NOBUFFERENTRIES       -15   /* Keine BuffereintrÑge vorhanden              */
