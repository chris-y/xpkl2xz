#ifndef XPKGZIP_TOOLS_H
#define XPKGZIP_TOOLS_H

#include "init.h"
#include "zlib.h"

#define MemPool *((APTR      *)&xpar->xsp_Sub[1])
#define ZStream *((z_streamp *)&xpar->xsp_Sub[2])

/*
** utility functions for ZLIB
*/

extern void zcFree(voidpf,voidpf);
extern voidpf zcAlloc(voidpf,unsigned,unsigned);
extern z_streamp init_stream(struct XpkSubIFace *, XPAR *);


#endif // XPKGZIP_TOOLS_H

