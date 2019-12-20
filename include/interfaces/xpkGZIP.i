#ifndef XPKGZIP_INTERFACE_DEF_H
#define XPKGZIP_INTERFACE_DEF_H
/*
** This file is machine generated from idltool
** Do not edit
*/ 

#include <exec/types.i>
#include <exec/exec.i>
#include <exec/interfaces.i>

STRUCTURE XpkSubIFace, InterfaceData_SIZE
	    FPTR IXpkSub_Obtain
	    FPTR IXpkSub_Release
	    FPTR IXpkSub_Expunge
	    FPTR IXpkSub_Clone
	    FPTR IXpkSub_XpksPackerInfo
	    FPTR IXpkSub_XpksPackChunk
	    FPTR IXpkSub_XpksPackFree
	    FPTR IXpkSub_XpksPackReset
	    FPTR IXpkSub_XpksUnpackChunk
	    FPTR IXpkSub_XpksUnpackFree
	LABEL XpkSubIFace_SIZE

#endif
