/*
**	$Id$
**	Generated by IDLTool 51.3
**	Do not edit
**
**	AutoInit stub for xpkGZIP
**
**	(C) Copyright 2003-2004 Amiga, Inc.
**	    All Rights Reserved
*/

#include <xpk/xpksub.h>

#include <interfaces/xpkGZIP.h>
#include <proto/exec.h>
#include <assert.h>

__attribute__((weak)) struct XpkSubIFace * IXpkSub = NULL;
/****************************************************************************/

extern struct XpkSubBase * XpkSubBase;

void __init_xpkGZIP_main(void) __attribute__((constructor));
void __exit_xpkGZIP_main(void) __attribute__((destructor));

/****************************************************************************/

void __init_xpkGZIP_main(void)
{
    struct XpkSubBase * LibBase;
    if (XpkSubBase == NULL) /* Library base is NULL, we need to open it */
    {
        LibBase = IExec->OpenLibrary("xpkGZIP.library", 0L);
        assert(LibBase != NULL);
    }
    else
    {
        LibBase = XpkSubBase;
    }

    IXpkSub = (struct XpkSubIFace *)IExec->GetInterface((struct Library *)LibBase, "main", 1, NULL);
    assert(IXpkSub != NULL);
}

/****************************************************************************/

void __exit_xpkGZIP_main(void)
{
    if (IXpkSub) IExec->DropInterface ((struct Interface *)IXpkSub);
}

/****************************************************************************/

