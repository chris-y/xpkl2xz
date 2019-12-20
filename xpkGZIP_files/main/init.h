#ifndef INIT_H
#define INIT_H

#include <exec/memory.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <xpk/xpksub.h>
#include <proto/xpkGZIP.h>
#include <proto/lzma.h>

struct XpkSubBase
{
    struct Library libNode;
    BPTR segList;
    /* If you need more data fields, add them here */
	struct ExecIFace *IExec;
//	struct lzmaIFace *xz_Ilzma;
};

typedef struct XpkSubParams XPAR;

//#define Ilzma libBase->xz_Ilzma

#endif // INIT_H
