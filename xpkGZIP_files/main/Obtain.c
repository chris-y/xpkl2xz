/*
 *  $VER: Obtain.c $Revision$ (21-Nov-2004)
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

/****** xpkGZIP/main/Obtain ******************************************
*
*   NAME
*      Obtain -- Description
*
*   SYNOPSIS
*      ULONG Obtain(void)
*
*   FUNCTION
*
*   INPUTS
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

ULONG _XpkSub_Obtain(struct XpkSubIFace *Self)
{

	return(Self->Data.RefCount++);

}

