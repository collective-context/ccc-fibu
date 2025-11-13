;************************************************************
;*   Druckertreiber fÅr Epson FX-80
;*   29.3.88   16:35
;*   Programmautor: Heinrich Gîbl
;*   Aktivieren durch Einbinden in CONFIG.SYS und Booten
;*   Eintrag in CONFIG.SYS:      DEVICE=FX-80DRV.SYS
;*   Assemblieren: MASM FX-80DRV;
;*   Linken:       LINK FX-80DRV;
;*   BinÑrfile:    EXE2BIN FX-80DRV FX-80DRV.SYS
;************************************************************

drvseg segment

       assume cs:drvseg, ds:nothing, es:nothing, ss:nothing
       org    0000h

;------------------------------------------------------------
; Treiberkopf
;------------------------------------------------------------

       dw    -1             ;Offset des nÑchsten Treibers
       dw    -1             ;Segment des nÑchsten Treibers
       dw    8000h          ;Attibut fÅr Zeichentreiber
       dw    offset stratr  ;Einsprungadr. der Strategie-R.
       dw    offset interr  ;Einsprungadr. der Interrupt-R.
       db    "FX80    "     ;Name des Treibers


;-------------------------------------------------------------
; Tabelle der Sprungbefehle -- bei Bedarf "dummy" einsetzen
;-------------------------------------------------------------

beftab dw    init           ;0  Initialisierung
       dw    dummy          ;1  Nur bei BLOCK-Treibern
       dw    dummy          ;2  Nur bei BLOCK-Treibern
       dw    error          ;3  Direktes Lesen Åber IOCTL
       dw    error          ;4  Lesen
       dw    error          ;5  Lesen und im Puffer lassen
       dw    dummy          ;6  Status beim Lesen
       dw    dummy          ;7  Leert Lesepuffer
       dw    output         ;8  Schreibt
       dw    output         ;9  Schreibt mit VERIFY
       dw    outstat        ;10 Status beim Schreiben
       dw    dummy          ;11 Leert Schreibpuffer
       dw    dummy          ;12 Direktes Schreiben Åber IOCTL
       dw    dummy          ;13 ôffnen des GerÑtes
       dw    dummy          ;14 Schlie·en des GerÑtes
       dw    dummy          ;15 Nur bei BLOCK-Treibern


;------------------------------------------------------------
; Definition einiger Daten und Konstanten
;------------------------------------------------------------


befehl =     2       ;Nummer des Befehls
stat   =     3       ;Statuswort
endoff =     14      ;Letztes residentes Byte (Offset)
endseg =     16      ;(Segment)
zahl   =     18      ;Zahl der zu druckenden Zeichen
LPT    =     0       ;LPT1
ESC    =     27      ;Drucker Escape-Zeichen

befoff dw    (?)     ;Offset des Befehlsblock
befseg dw    (?)     ;Segment
status dw    8003h   ;Statuswort

umlaut db    "ÑîÅéôö·",0         ;Umsatztabelle mit 0 als Ende
cvttab db    "{|}[\]~"           ;Ziel

deutab db    ESC,"R",2,"$"  ;Sequenz zur Umstellung auf
usatab db    ESC,"R",0,"$"  ;deu/usa Zeichensatz
;********** "$" wird hier zur Erkennung des Endes verwendet!


;------------------------------------------------------------
; Code: Strategie- und Interrupt-Routine
;------------------------------------------------------------

;===================    Strategieroutine     ================

stratr proc  far
       mov   cs:befoff,bx      ;ES:BX retten
       mov   cs:befseg,es      ;und zurÅck zum Aufrufer
       ret
stratr endp

;===================    Interruptroutine     ================

interr proc  far
       push  ax                ;SÑmtliche Register auf
       push  bx                ;von DOS hoffentlich
       push  cx                ;zur VerfÅgung gestellten
       push  dx                ;Stack
       push  si
       push  di
       push  bp
       push  ds
       push  es
       pushf
       mov   ax,cs:[befseg]
       mov   es,ax             ;es = befseg
       mov   bx,cs:[befoff]    ;bx = befoff
       mov   al,es:[bx+befehl] ;al = Befehlsnummer
       cmp   al,15             ;befehl > 15 ?
       ja    zwischen          ;Falscher Befehl
       xor   ah,ah             ;ah = 0
       shl   ax,1              ;befehl = befehl * 2
       mov   si,offset beftab  ;si = Adresse von beftab
       add   si,ax             ;si = ptr -> Befehlsroutine
       jmp   [si]              ;Sprung zum Befehl
zwischen:
       jmp   error

;------------------------------------------------------------
; Befehlsverarbeitung
;------------------------------------------------------------

;****** Dummy-Routine setzt nur das Fertig-Bit **************

dummy:                         ;Routine fÅr nicht unter-
       mov   ax,0100h          ;stÅtzte Funktionen
       jmp   allret            ;Fertig-Bit setzen

;****** Output erledigt Funktionen 8 und 9 ******************

output:                        ;8+9  Schreibt Zeichen
       mov   cx,es:[bx+zahl]   ;Zahl der Zeichen
       jcxz  jumpret           ;Falls 0 => zurÅck
       mov   si,es:[bx+endoff] ;Offset des Puffers
       mov   ax,es:[bx+endseg] ;Segment des Puffers
       mov   ds,ax             ;in DS

; cx = Zahl der zu druckenden Zeichen
; ds:si = Adresse der Zeichen

loop1:
       mov   al,byte ptr [si]  ;al = zu druckendes Zeichen
       cmp   al,80h            ;Falls Zeichen kleiner 128
       jb    ready             ;==> normale Ausgabe
       push  si                ;sonst sichere Register
       push  bx
       mov   si,offset umlaut  ;si --> Umlauttabelle
       mov   di,offset cvttab  ;di --> öbersetzung
       xor   bx,bx
vgl:
       cmp   al,byte ptr cs:[si+bx]  ;Zeichen
       je    found                   ;gefunden?
       cmp   byte ptr cs:[si+bx],0   ;Tabellenende
       je    space                   ;erreicht?
       inc   bx                      ;nein! - nÑchstes
       jmp   vgl                     ;Tabellenelement
space:
       mov   al," "            ;FÅr Zeichen grî·er als 127
       pop   bx
       pop   si
       jmp   ready             ;wird ein " " gedruckt
found:
       mov   si,offset deutab
loop2:
       mov   al,byte ptr cs:[si]  ;*  Umschalten
       cmp   al,"$"               ;*  auf
       je    endloop2             ;*  deutschen
       call  pc                   ;*  Zeichensatz
       jnz   fehler1              ;*  27d,82d,2d
       inc   si
       jmp   loop2                ;*  1Bh,52h,02h
endloop2:
       mov   al,byte ptr cs:[di+bx]  ;Zeichen laden
       call  pc                      ;und drucken
       jnz   fehler1
       mov   si,offset usatab
loop3:
       mov   al,byte ptr cs:[si]
       cmp   al,"$"            ;*  Umschalten
       je    endloop3          ;*  auf
       call  pc                ;*  amerikanischen
       jnz   fehler1           ;*  Zeichensatz
       inc   si
       jmp   loop3             ;*  27d,82d,0d
endloop3:                      ;*  1Bh,52h,00h
       pop   bx                ;Register vom
       pop   si                ;Stack holen
       jmp   nextc
ready:
       call  pc                ;Normales Zeichen drucken
       jnz   fehler
nextc:
       inc   si                ;nÑchstes Zeichen
       dec   cx                ;ZÑhler verkleinern
       jcxz  return            ;Alle Zeichen gedruckt ?
       jmp   loop1             ;Nein => nÑchstes Zeichen
fehler1:
       pop   bx
       pop   si
fehler:
       mov   byte ptr cs:[status],ah
       jmp   error

jumpret:
       jmp   return

;****** Outstat gibt dem Aufrufer den Status des Druckers ***

outstat:                       ;10 - Status beim Schreiben
       mov   ah,2              ;Druckerstatus
       call  pstat
       jz    statret
       mov   byte ptr cs:[status],ah
       jmp   error
statret:
       xor   ax,ax
       jmp   return

;****** Error setzt Fehlercode auf "Funktion unbekannt" *****

error:
       mov   ax,cs:status      ;Fehler: unbekannter Befehl

return:
       or    ax,0100h          ;Fertig-Bit setzen
allret:
       mov   word ptr es:[bx+stat],ax
       popf
       pop   es
       pop   ds
       pop   bp
       pop   di
       pop   si
       pop   dx
       pop   cx
       pop   bx
       pop   ax
       ret            ;Far Return
interr endp

;------------------------------------------------------------
; pc -- erwartet zu druckendes Zeichen in AL
; erledigt Fehlerbehandlung. Beim Auftauchen eines Fehlers
; wird das Zeroflag (ZF) gesetzt.
; Ergebnis: Statuswort in AX
;------------------------------------------------------------

pstat  proc
       jmp   statin
pc     proc
       xor   ah,ah
statin:
       mov   dx,LPT
       int   17h               ;Druckerinterrupt
       mov   al,2              ;DOS-Error: Not Ready
       test  ah,01h            ;Timeout?
       jne   pcret
       mov   al,10             ;DOS-Error: Schreibfehler
       test  ah,08h            ;öbertragungsfehler?
       je    pcret
       test  ah,20h            ;Papier fehlt
       je    papok             ;oder nicht ?
       mov   al,9              ;DOS-Error: Paper out
papok:
       or    al,al
pcret:
       ret
pc     endp
pstat  endp

;------------------------------------------------------------
;  Bis hier geht der residente Teil - mit "init" beginnt der
;  Teil, der nach dem Ablauf nicht geschÅtzt im Speicher
;  steht. Bei der Initialisierung wird nicht abgefragt, ob
;  der Drucker empfangen hat, da er beim Kaltstart des Com-
;  puters mîglicherweise noch nicht eingeschaltet ist.
;------------------------------------------------------------

init:                          ;0  Initialisierung
       push  cs
       pop   ds                ;DS = CS
       mov   dx,offset msg     ;Startmeldung
       mov   ah,09h            ;Åber Interrupt 21h,
       int   21h               ;Funktion 9 ausgeben
       mov   dx,LPT            ;Ausgabe auf LPT
       mov   ah,1              ;Initialisierung
       int   17h
       xor   ah,ah             ;amerikanischen
       mov   al,ESC            ;Zeichensatz
       int   17h               ;einschalten
       xor   ah,ah             ;Falls eines der
       mov   al,"R"            ;Zeichen []{}\|~
       int   17h               ;vor einem
       xor   ax,ax             ;deutschen Umlaut
       int   17h               ;kommt

;folgende zwei Instruktionen teilen DOS mit, bis zu welchem
;Byte dieses Treibers es Speicher reservieren mu·

       mov   word ptr es:[bx+endoff],offset init
       mov   word ptr es:[bx+endseg],cs
       mov   beftab,offset dummy  ;alte Adresse "init" wird
       xor   ax,ax                ;auf "dummy" gesetzt
       jmp   return

msg    db    13,10,13,10,13,10
       db    "-----------------------------------------",13,10
       db    "   Druckertreiber fÅr Epson FX-80",13,10
       db    "   Heinrich Gîbl          27/3/88",13,10
       db    "-----------------------------------------",13,10,13,10,"$"

drvseg ends
       end
