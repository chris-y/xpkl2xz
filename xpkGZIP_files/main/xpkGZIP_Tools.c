#include <exec/memory.h>
#include "init.h"
#include <zlib.h>

/*
** `global data'
*/


#define MemPool *((APTR      *)&xpar->xsp_Sub[1])
#define ZStream *((z_streamp *)&xpar->xsp_Sub[2])

/*
** utility functions for ZLIB
*/

struct ExecIFace *IExec;


__attribute__((used)) static void zcFree(voidpf opaque, voidpf ptr)
{

	XPAR *xpar = (XPAR *)opaque;
	ULONG *p = (ULONG *)ptr;

	if (p) IExec->FreePooled(MemPool, p-1, *(p-1));

}

__attribute__((used)) static voidpf zcAlloc(voidpf opaque, unsigned items, unsigned itemsize)
{
	
	XPAR *xpar = (XPAR *)opaque;
	ULONG size, *mem;

	if ((mem=(ULONG *)IExec->AllocPooled(MemPool,size=sizeof(*mem)+items*itemsize)))
	{
		*mem++ = size;
	}

	return mem;

}

/*
** initialize a z_stream (either for compression or decompression)
*/

z_streamp init_stream(struct XpkSubIFace *Self, XPAR *xpar)
{
	z_streamp stream = ZStream;
	struct XpkSubBase *libBase = (struct XpkSubBase *)Self->Data.LibBase;

	IExec = libBase->IExec;

	if (!stream)
	{
		if ((MemPool=libBase->IExec->CreatePool(MEMF_CLEAR,32768,32768)) != NULL)
		{
			if ((ZStream=stream=zcAlloc(xpar,1,sizeof(*stream))) != NULL)
			{
				stream->zalloc    = zcAlloc;
				stream->zfree     = zcFree;
				stream->opaque    = xpar;
				stream->next_in   = (Bytef *)xpar->xsp_InBuf;
				stream->avail_in  = (uInt   )xpar->xsp_InLen;
				stream->next_out  = (Bytef *)xpar->xsp_OutBuf;
				stream->avail_out = (uInt   )xpar->xsp_OutBufLen;
			}
		}
	}
	else
	{
		stream->next_in   = (Bytef *)xpar->xsp_InBuf;
		stream->avail_in  = (uInt   )xpar->xsp_InLen;
		stream->next_out  = (Bytef *)xpar->xsp_OutBuf;
		stream->avail_out = (uInt   )xpar->xsp_OutBufLen;
	}

	return stream;
}


