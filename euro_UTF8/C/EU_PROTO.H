/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
	VOID main		   (SWORD argc, PSSTR argv[]);

GLOBAL SWORD SetDatei(PSSTR[][2], SWORD[], PPSTR, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL PTEXTBOX SetTextBox(SWORD, PTEXTBOX);
GLOBAL SWORD SetFeldNummer(PSWORD, PPSTR, PTEXTBOX, SWORD, SWORD);

#define F_ pstrMaskFile_g
#define L_ wEingangsZeile_g+wKombi_g+1
#define SetFeldNr() SetFeldNummer(&wFeldNummer, apstrWort, pTB_g, wBox_g, wRefFieldNb_g)

