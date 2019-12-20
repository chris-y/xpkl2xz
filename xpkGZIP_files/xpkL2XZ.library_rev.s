VERSION = 1
REVISION = 1

.macro DATE
.ascii "12.9.2014"
.endm

.macro VERS
.ascii "xpkL2XZ.library 1.1"
.endm

.macro VSTRING
.ascii "xpkL2XZ.library 1.1 (12.9.2014)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: xpkL2XZ.library 1.1 (12.9.2014)"
.byte 0
.endm
