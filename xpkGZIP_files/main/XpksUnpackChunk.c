/*
 *  $VER: XpksUnpackChunk.c $Revision$ (21-Nov-2004)
 *
 *  This file is part of xpkGZIP.
 *
 *  (C) Copyright 2004 Hyperion Entertainment
 *      All rights reserved
 *
 * $Id$
 *
 * $Log$
 *
 *
 */


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <xpk/xpksub.h>
#include <proto/xpkGZIP.h>
#include <stdarg.h>
#include "xpkGZIP_Tools.h"

/****** xpkGZIP/main/XpksUnpackChunk ******************************************
*
*   NAME
*      XpksUnpackChunk -- Description
*
*   SYNOPSIS
*      LONG XpksUnpackChunk(struct XpkSubParams * xpar);
*
*   FUNCTION
*
*   INPUTS
*       xpar - 
*
*   RESULT
*       The result ...
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/

LONG _XpkSub_XpksUnpackChunk(struct XpkSubIFace *Self,
       struct XpkSubParams * xpar)
{	
	struct XpkSubBase *libBase = (struct xpkSubBase*)Self->Data.LibBase;
	struct ExecIFace *IExec = libBase->IExec;
	LONG err = XPKERR_OK;
	lzma_ret ret;
	lzma_stream *strm; // = LZMA_STREAM_INIT;
	lzma_action action = LZMA_RUN;

	if(xpar->xsp_Sub[0]) {
		strm = (lzma_stream *)xpar->xsp_Sub[0];
	} else {
		strm = IExec->AllocVecTags(sizeof(lzma_stream),
				AVT_ClearWithValue, 0,
				TAG_DONE);

		xpar->xsp_Sub[0] = (ULONG)strm;

		ret = Ilzma->lzma_stream_decoder(strm, UINT64_MAX, 0);
		if(ret != LZMA_OK) {
			switch(ret) {
				case LZMA_MEM_ERROR:
					return XPKERR_NOMEM;
				break;

				case LZMA_OPTIONS_ERROR:
				case LZMA_PROG_ERROR:
				default:
					return XPKERR_UNKNOWN;
				break;
			}
		}
	}

	strm->next_in = xpar->xsp_InBuf;
	strm->avail_in = xpar->xsp_InLen;
	strm->next_out = xpar->xsp_OutBuf;
	strm->avail_out = xpar->xsp_OutBufLen;

	do {
		ret = Ilzma->lzma_code(strm, action);
		if((ret != LZMA_OK) && (ret != LZMA_STREAM_END)) {
			err = XPKERR_NOMEM;
			break;
		}
	} while (ret != LZMA_STREAM_END);

	xpar->xsp_OutLen = strm->total_out;
//	Ilzma->lzma_end(strm);

	return err;

}

