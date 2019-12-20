#ifndef PROTO_XPKGZIP_H
#define PROTO_XPKGZIP_H

/*
**	$Id$
**	Includes Release 50.1
**
**	Prototype/inline/pragma header file combo
**
**	(C) Copyright 2003-2004 Amiga, Inc.
**	    All Rights Reserved
*/

#ifndef XPK_XPKSUB_H
#include <xpk/xpksub.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #ifndef __USE_BASETYPE__
  extern struct Library * XpkSubBase;
 #else
  extern struct XpkSubBase * XpkSubBase;
 #endif /* __USE_BASETYPE__ */
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/xpkGZIP.h>
 #ifdef __USE_INLINE__
  #include <inline4/xpkGZIP.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_XPKGZIP_PROTOS_H
  #define CLIB_XPKGZIP_PROTOS_H 1
 #endif /* CLIB_XPKGZIP_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  extern struct XpkSubIFace *IXpkSub;
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_XPKGZIP_PROTOS_H
  #include <clib/xpkGZIP_protos.h>
 #endif /* CLIB_XPKGZIP_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/xpkGZIP.h>
  #else
   #include <ppcinline/xpkGZIP.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/xpkGZIP_protos.h>
  #endif /* __PPC__ */
 #else
  #include <pragmas/xpkGZIP_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_XPKGZIP_H */
