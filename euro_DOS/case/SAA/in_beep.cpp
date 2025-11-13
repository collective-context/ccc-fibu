// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Beep                         Datum: 28.10.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ruft die FUnktionen Ut_Ton()         บ
  บ                    und Ut_Pause() und das Makro Ut_Ton() auf,          บ
  บ                    um einen kurzen Warnton zu erzeugen.                บ
  บ                    Dieser Warnton wird bei allen Dialogfeldern         บ
  บ                    ausgegeben, wenn eine derzeit ungltige Aktion      บ
  บ                    ausgewhlt wurde.                                   บ
  บ                                                                        บ
  บ                    Die Frequenz und Dauer des Tons werden den          บ
  บ                    globalen Variablen wBeepFreq_g und wBeepDauer_g     บ
  บ                    entnommen.                                          บ
  บ                                                                        บ
  บ                    Wenn Sie in Ihr Programm die Funktion Dl_Warnton()  บ
  บ                    aufnehmen, hat der Anwender die M”glichkeit,        บ
  บ                    die Dauer und die Frequenz einzustellen und         บ
  บ                    grundstzlich festzulegen, ob ein Warnton er-       บ
  บ                    klingen soll oder nicht.                            บ
  บ                                                                        บ
  บ                    weitere Informationen finden Sie in der Dokumen-    บ
  บ                    tation zu - Dl_Warnton()                            บ
  บ                              - Dl_GetOptionen()                        บ
  บ                              - Dl_PutOptionen()                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wBeepFreq_g, wBeepDauer_g (R)                       บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT BOOL boBeepen_g;                 /* Warnton ein/aus                  */
IMPORT SWORD wBeepFreq_g;
IMPORT SWORD wBeepDauer_g;
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
