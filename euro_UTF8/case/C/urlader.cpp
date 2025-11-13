// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#include <dos.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
void main(void)
{
union REGS regs;

int86(0x19, &regs, &regs);                           /* Interrupt auslösen   */
}
