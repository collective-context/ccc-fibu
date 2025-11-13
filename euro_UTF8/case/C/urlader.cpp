// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <dos.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
void main(void)
{
union REGS regs;

int86(0x19, &regs, &regs);                           /* Interrupt ausl”sen   */
}
