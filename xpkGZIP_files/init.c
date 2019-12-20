/*
 *  $VER: init.c $Revision$ (21-Nov-2004)
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


#include <dos/dos.h>
#include <xpk/xpksub.h>
#include <stdarg.h>
#include "main/init.h"

/* Version Tag */
#include "xpkL2XZ.library_rev.h"
static volatile char 
#ifdef __GNUC__
__attribute__((unused))
#endif
verstag[] = VERSTAG;

struct Library *NewlibBase;
struct Interface *INewlib;

struct Library *lzmaBase;
struct lzmaIFace *Ilzma;


/*
 * The system (and compiler) rely on a symbol named _start which marks
 * the beginning of execution of an ELF file. To prevent others from 
 * executing this library, and to keep the compiler/linker happy, we
 * define an empty _start symbol here.
 *
 * On the classic system (pre-AmigaOS4) this was usually done by
 * moveq #0,d0
 * rts
 *
 */
void _start(void)
{
    /* If you feel like it, open DOS and print something to the user */
}


/* Open the library */
struct Library *libOpen(struct LibraryManagerInterface *Self, ULONG version)
{
    struct XpkSubBase *libBase = (struct XpkSubBase *)Self->Data.LibBase; 

    /* Add any specific open code here 
       Return 0 before incrementing OpenCnt to fail opening */

    /* Add up the open count */
    libBase->libNode.lib_OpenCnt++;
    return (struct Library *)libBase;

}


/* Close the library */
APTR libClose(struct LibraryManagerInterface *Self)
{
    struct XpkSubBase *libBase = (struct XpkSubBase *)Self->Data.LibBase;
    /* Make sure to undo what open did */

    /* Make the close count */
    ((struct Library *)libBase)->lib_OpenCnt--;

    return 0;
}


/* Expunge the library */
APTR libExpunge(struct LibraryManagerInterface *Self)
{
    /* If your library cannot be expunged, return 0 */
    struct ExecIFace *IExec
        = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
    APTR result = (APTR)0;
    struct XpkSubBase *libBase = (struct XpkSubBase *)Self->Data.LibBase;
    if (libBase->libNode.lib_OpenCnt == 0)
    {
	     result = (APTR)libBase->segList;
        /* Undo what the init code did */

		if(Ilzma) {
			IExec->DropInterface((struct Interface *)Ilzma);
			IExec->CloseLibrary(lzmaBase);
		}

		if(INewlib) {
			IExec->DropInterface((struct Interface *)INewlib);
			IExec->CloseLibrary(NewlibBase);
		}

        IExec->Remove((struct Node *)libBase);
        IExec->DeleteLibrary((struct Library *)libBase);
    }
    else
    {
        result = (APTR)0;
        libBase->libNode.lib_Flags |= LIBF_DELEXP;
    }
    return result;
}

/* The ROMTAG Init Function */
struct Library *libInit(struct Library *LibraryBase, APTR seglist, struct Interface *exec)
{
    struct XpkSubBase *libBase = (struct XpkSubBase *)LibraryBase;
    struct ExecIFace *IExec
#ifdef __GNUC__
        __attribute__((unused))
#endif
        = (struct ExecIFace *)exec;

    libBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    libBase->libNode.lib_Node.ln_Pri  = 0;
    libBase->libNode.lib_Node.ln_Name = "xpkL2XZ.library";
    libBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    libBase->libNode.lib_Version      = VERSION;
    libBase->libNode.lib_Revision     = REVISION;
    libBase->libNode.lib_IdString     = VSTRING;

    libBase->segList = (BPTR)seglist;

    /* Add additional init code here if you need it. For example, to open additional
       Libraries:
       libBase->UtilityBase = IExec->OpenLibrary("utility.library", 50L);
       if (libBase->UtilityBase)
       {
           libBase->IUtility = (struct UtilityIFace *)IExec->GetInterface(ElfBase->UtilityBase, 
              "main", 1, NULL);
           if (!libBase->IUtility)
               return NULL;
       } else return NULL; */


	if(NewlibBase = IExec->OpenLibrary("newlib.library",51)) {
		INewlib = (struct NewlibIFace *)IExec->GetInterface((struct Library *)NewlibBase,"main",1,NULL);
	} else return NULL;

	if(lzmaBase = IExec->OpenLibrary("lzma.library", 0L)) {
		Ilzma = (struct lzmaIFace *)IExec->GetInterface(lzmaBase,"main",1,NULL);
	} else return NULL;

	libBase->IExec = IExec;

	return (struct Library *)libBase;
}

/* ------------------- Manager Interface ------------------------ */
/* These are generic. Replace if you need more fancy stuff */
static LONG _manager_Obtain(struct LibraryManagerInterface *Self)
{
    return Self->Data.RefCount++;
}

static ULONG _manager_Release(struct LibraryManagerInterface *Self)
{
    return Self->Data.RefCount--;
}

/* Manager interface vectors */
static void *lib_manager_vectors[] =
{
    (void *)_manager_Obtain,
    (void *)_manager_Release,
    (void *)0,
    (void *)0,
    (void *)libOpen,
    (void *)libClose,
    (void *)libExpunge,
    (void *)0,
    (void *)-1,
};

/* "__library" interface tag list */
static struct TagItem lib_managerTags[] =
{
    {MIT_Name,             (ULONG)"__library"},
    {MIT_VectorTable,      (ULONG)lib_manager_vectors},
    {MIT_Version,          1},
    {TAG_DONE,             0}
};

/* ------------------- Library Interface(s) ------------------------ */

#include "xpkGZIP_vectors.c"

/* Uncomment this line (and see below) if your library has a 68k jump table */
extern ULONG VecTable68K;

static struct TagItem mainTags[] =
{
    {MIT_Name,              (uint32)"main"},
    {MIT_VectorTable,       (uint32)main_vectors},
    {MIT_Version,           1},
    {TAG_DONE,              0}
};

static uint32 libInterfaces[] =
{
    (uint32)lib_managerTags,
    (uint32)mainTags,
    (uint32)0
};

struct TagItem libCreateTags[] =
{
    {CLT_DataSize,         (uint32)(sizeof(struct XpkSubBase))},
    {CLT_InitFunc,         (uint32)libInit},
    {CLT_Interfaces,       (uint32)libInterfaces},
    /* Uncomment the following line if you have a 68k jump table */
	{CLT_Vector68K,        (uint32)&VecTable68K},
    {TAG_DONE,             0}
};


/* ------------------- ROM Tag ------------------------ */
__attribute__((used)) static struct Resident lib_res =
{
    RTC_MATCHWORD,
    &lib_res,
    &lib_res+1,
    RTF_NATIVE|RTF_AUTOINIT, /* Add RTF_COLDSTART if you want to be resident */
    VERSION,
    NT_LIBRARY, /* Make this NT_DEVICE if needed */
    0, /* PRI, usually not needed unless you're resident */
    "xpkL2XZ.library",
    VSTRING,
    libCreateTags
};

