// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Beep                         Datum: 28.10.88      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ruft die FUnktionen Ut_Ton()         ║
  ║                    und Ut_Pause() und das Makro Ut_Ton() auf,          ║
  ║                    um einen kurzen Warnton zu erzeugen.                ║
  ║                    Dieser Warnton wird bei allen Dialogfeldern         ║
  ║                    ausgegeben, wenn eine derzeit ungültige Aktion      ║
  ║                    ausgewählt wurde.                                   ║
  ║                                                                        ║
  ║                    Die Frequenz und Dauer des Tons werden den          ║
  ║                    globalen Variablen wBeepFreq_g und wBeepDauer_g     ║
  ║                    entnommen.                                          ║
  ║                                                                        ║
  ║                    Wenn Sie in Ihr Programm die Funktion Dl_Warnton()  ║
  ║                    aufnehmen, hat der Anwender die Möglichkeit,        ║
  ║                    die Dauer und die Frequenz einzustellen und         ║
  ║                    grundsätzlich festzulegen, ob ein Warnton er-       ║
  ║                    klingen soll oder nicht.                            ║
  ║                                                                        ║
  ║                    weitere Informationen finden Sie in der Dokumen-    ║
  ║                    tation zu - Dl_Warnton()                            ║
  ║                              - Dl_GetOptionen()                        ║
  ║                              - Dl_PutOptionen()                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wBeepFreq_g, wBeepDauer_g (R)                       ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT BOOL boBeepen_g;                 /* Warnton ein/aus                  */
IMPORT SWORD wBeepFreq_g;
IMPORT SWORD wBeepDauer_g;
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Beep(VOID)
{
if(boBeepen_g)
  {
  Ut_Ton(wBeepFreq_g);
  Ut_Pause(UT_TICKS, wBeepDauer_g);
  Ut_TonAus();
  return;
  }
}
