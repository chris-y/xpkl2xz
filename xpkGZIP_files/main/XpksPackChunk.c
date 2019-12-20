/*
 *  $VER: XpksPackChunk.c $Revision$ (21-Nov-2004)
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

#include <stdbool.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <xpk/xpksub.h>
#include <proto/xpkGZIP.h>
#include <stdarg.h>
//#include "xpkGZIP_Tools.h"
#include "init.h"
#include <lzma.h>

/****** xpkGZIP/main/XpksPackChunk ******************************************
*
*   NAME
*      XpksPackChunk -- Description
*
*   SYNOPSIS
*      LONG XpksPackChunk(struct XpkSubParams * xpar);
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

LONG _XpkSub_XpksPackChunk(struct XpkSubIFace *Self,
       struct XpkSubParams * xpar)
{

	uint32_t level, extreme = 0;
	LONG err = XPKERR_OK;
	lzma_ret ret_xz;
	lzma_check check = LZMA_CHECK_CRC64;
	lzma_stream *strm; // = LZMA_STREAM_INIT;
	bool in_finished = false;
	bool out_finished = false;
	lzma_action action;
	bool ppc_filter = false;
	uint32_t delta = 0;

	struct XpkSubBase *libBase = (struct XpkSubBase *)Self->Data.LibBase;
	struct ExecIFace *IExec = libBase->IExec;

	if(xpar->xsp_Sub[0]) {
		strm = (lzma_stream *)xpar->xsp_Sub[0];
	} else {
		strm = IExec->AllocVecTags(sizeof(lzma_stream),
				AVT_ClearWithValue, 0,
				TAG_DONE);

		xpar->xsp_Sub[0] = strm;

		if ((level=xpar->xsp_Mode/10) != 10) /* level 0-9 */ {
			if (level < 0 || level > 9)
				level = LZMA_PRESET_DEFAULT;
		} else {
			extreme = true;
			level = 9;
		}

#ifdef DISABLE_OVER_80
		/* crash prevention, higher modes use too much memory */
		if(level > 7) level = 7;
#endif
		if(extreme) level |= LZMA_PRESET_EXTREME;

		switch((xpar->xsp_Mode - (level * 10))) {
			case 1:
				delta = 1;
			break;
			case 2:
				delta = 2;
			break;
			case 3:
				delta = 3;
			break;
			case 4:
				delta = 4;
			break;

			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				ppc_filter = true;
			break;

			case 0:
			default:
			break;
		}

		if(ppc_filter == true) {
			lzma_options_lzma opt_lzma2;
			Ilzma->lzma_lzma_preset(&opt_lzma2, level); /* should never fail */

			lzma_filter filters[] = {
				{ .id = LZMA_FILTER_POWERPC, .options = NULL },
				{ .id = LZMA_FILTER_LZMA2, .options = &opt_lzma2 },
				{ .id = LZMA_VLI_UNKNOWN, .options = NULL },
			};	

			ret_xz = Ilzma->lzma_stream_encoder(strm, filters, LZMA_CHECK_CRC64);
			if (ret_xz != LZMA_OK) return XPKERR_NOMEM;
		} else if(delta > 0) {
			lzma_options_lzma opt_lzma2;
			lzma_options_delta opt_delta;

			Ilzma->lzma_lzma_preset(&opt_lzma2, level); /* should never fail */

			opt_delta.type = LZMA_DELTA_TYPE_BYTE;
			opt_delta.dist = delta;

			lzma_filter filters[] = {
				{ .id = LZMA_FILTER_DELTA, .options = &opt_delta },
				{ .id = LZMA_FILTER_LZMA2, .options = &opt_lzma2 },
				{ .id = LZMA_VLI_UNKNOWN, .options = NULL },
			};	

			ret_xz = Ilzma->lzma_stream_encoder(strm, filters, LZMA_CHECK_CRC64);
			if (ret_xz != LZMA_OK) return XPKERR_NOMEM;
		} else {
			ret_xz = Ilzma->lzma_easy_encoder(strm, level, check);
			if (ret_xz != LZMA_OK) return XPKERR_NOMEM;
		}
	}

	strm->next_in = xpar->xsp_InBuf;
	strm->avail_in = xpar->xsp_InLen;
	strm->next_out = xpar->xsp_OutBuf;
	strm->avail_out = xpar->xsp_OutBufLen;

	action = LZMA_RUN;

	do {
		ret_xz = Ilzma->lzma_code (strm, LZMA_RUN);
		if ((ret_xz != LZMA_OK) && (ret_xz != LZMA_STREAM_END)) err = XPKERR_NOMEM;
	} while (strm->avail_in > 0);

	ret_xz = Ilzma->lzma_code (strm, LZMA_FINISH);
	if ((ret_xz != LZMA_OK) && (ret_xz != LZMA_STREAM_END)) err = XPKERR_NOMEM;

	xpar->xsp_OutLen = strm->total_out;

	return err;

}

