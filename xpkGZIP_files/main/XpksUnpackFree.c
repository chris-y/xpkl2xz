/*
 *  $VER: XpksUnpackFree.c $Revision$ (21-Nov-2004)
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

/****** xpkGZIP/main/XpksUnpackFree ******************************************
*
*   NAME
*      XpksUnpackFree -- Description
*
*   SYNOPSIS
*      void XpksUnpackFree(struct XpkSubParams * xpar);
*
*   FUNCTION
*
*   INPUTS
*       xpar - 
*
*   RESULT
*       This function does not return a result
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
void _XpkSub_XpksUnpackFree(struct XpkSubIFace *Self,
       struct XpkSubParams * xpar)
{
	lzma_stream *strm;
	struct XpkSubBase *libBase = (struct xpkSubBase*)Self->Data.LibBase;
	struct ExecIFace *IExec = libBase->IExec;

	if(xpar->xsp_Sub[0]) {
		strm = (lzma_stream *)xpar->xsp_Sub[0];
		Ilzma->lzma_end(strm);
		IExec->FreeVec(strm);
		xpar->xsp_Sub[0] = NULL;
	}

}
