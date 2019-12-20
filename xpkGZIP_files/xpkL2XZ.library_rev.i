VERSION		EQU	1
REVISION	EQU	1

DATE	MACRO
		dc.b '12.9.2014'
		ENDM

VERS	MACRO
		dc.b 'xpkL2XZ.library 1.1'
		ENDM

VSTRING	MACRO
		dc.b 'xpkL2XZ.library 1.1 (12.9.2014)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: xpkL2XZ.library 1.1 (12.9.2014)',0
		ENDM
