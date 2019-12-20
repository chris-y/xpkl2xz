/*
 *  $VER: XpksPackerInfo.c $Revision$ (21-Nov-2004)
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
#include "../xpkL2XZ.library_rev.h"
#include "stdio.h"
#include <libraries/iffparse.h>
#include <lzma.h>

/****** xpkGZIP/main/XpksPackerInfo ******************************************
*
*   NAME
*      XpksPackerInfo -- Description
*
*   SYNOPSIS
*      struct XpkInfo * XpksPackerInfo(void)
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

#define GZIPMODE(x) (struct XpkMode *)(GZIPModes+x)

STATIC const struct XpkMode GZIPModes[] = {
  { GZIPMODE(1),   0, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   841, 0, "0"     },
  { GZIPMODE(2),   1, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   776, 0, "0+delta1" },
  { GZIPMODE(3),   2, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   709, 0, "0+delta2" },
  { GZIPMODE(4),   3, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   735, 0, "0+delta3" },
  { GZIPMODE(5),   4, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   748, 0, "0+delta4" },
  { GZIPMODE(6),   9, /*XPKMF_A3000SPEED*/ 0, 0, 0, 45901, 9180,   846, 0, "0+ppc"   },
  { GZIPMODE(7),  10, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 847, 0, "1"   },
  { GZIPMODE(8),  11, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 780, 0, "1+delta1" },
  { GZIPMODE(9),  12, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 712, 0, "1+delta2" },
  { GZIPMODE(10),  13, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 738, 0, "1+delta3" },
  { GZIPMODE(11),  14, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 748, 0, "1+delta4" },
  { GZIPMODE(12),  19, /*XPKMF_A3000SPEED*/ 0, 0, 0, 11475, 7650, 852, 0, "1+ppc"   },
  { GZIPMODE(13),  20, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 849, 0, "2"    },
  { GZIPMODE(14),  21, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 781, 0, "2+delta1" },
  { GZIPMODE(15),  22, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 714, 0, "2+delta2" },
  { GZIPMODE(16),  23, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 739, 0, "2+delta3" },
  { GZIPMODE(17),  24, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 749, 0, "2+delta4" },
  { GZIPMODE(18),  29, /*XPKMF_A3000SPEED*/ 0, 0, 0, 7650, 7650, 853, 0, "2+ppc"    },
  { GZIPMODE(19),  30, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 849, 0, "3"      },
  { GZIPMODE(20),  31, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 782, 0, "3+delta1"  },
  { GZIPMODE(21),  32, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 714, 0, "3+delta2"  },
  { GZIPMODE(22),  33, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 739, 0, "3+delta3"  },
  { GZIPMODE(23),  34, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 749, 0, "3+delta4"  },
  { GZIPMODE(24),  39, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 11475, 854, 0, "3+ppc"   },
  { GZIPMODE(25),  40, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 863, 0, "4"      },
  { GZIPMODE(26),  41, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 799, 0, "4+delta1"  },
  { GZIPMODE(27),  42, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 756, 0, "4+delta2"  },
  { GZIPMODE(28),  43, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 763, 0, "4+delta3"  },
  { GZIPMODE(29),  44, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 775, 0, "4+delta4"  },
  { GZIPMODE(30), 49, /*XPKMF_A3000SPEED*/ 0, 0, 0,  433, 9180, 869, 0, "4+ppc"      },
  { GZIPMODE(31), 50, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 865, 0, "5"   },
  { GZIPMODE(32), 51, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 794, 0, "5+delta1" },
  { GZIPMODE(33), 52, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 759, 0, "5+delta2" },
  { GZIPMODE(34), 53, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 766, 0, "5+delta3" },
  { GZIPMODE(35), 54, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 777, 0, "5+delta4" },
  { GZIPMODE(36), 59, /*XPKMF_A3000SPEED*/ 0, 0, 0,  227, 7650, 870, 0, "5+ppc"   },
  { GZIPMODE(37), 60, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 866, 0, "6"   },
  { GZIPMODE(38), 61, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 801, 0, "6+delta1" },
  { GZIPMODE(39), 62, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 761, 0, "6+delta2" },
  { GZIPMODE(40), 63, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 769, 0, "6+delta3" },
  { GZIPMODE(41), 64, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 782, 0, "6+delta4" },
  { GZIPMODE(42), 69, /*XPKMF_A3000SPEED*/ 0, 0, 0,  223, 7650, 871, 0, "6+ppc"   },
  { GZIPMODE(43), 70, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 866, 0, "7"    },
  { GZIPMODE(44), 71, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 801, 0, "7+delta1" },
  { GZIPMODE(45), 72, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 761, 0, "7+delta2" },
  { GZIPMODE(46), 73, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 769, 0, "7+delta3" },
  { GZIPMODE(47), 74, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 782, 0, "7+delta4" },
  { GZIPMODE(48), 79, /*XPKMF_A3000SPEED*/ 0, 0, 0,  151, 9180, 871, 0, "7+ppc"    },
#ifndef DISABLE_OVER_80
/* These modes use too much memory and crash my SAM440! */
  { GZIPMODE(49), 80, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8"  },
  { GZIPMODE(50), 81, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8+delta1" },
  { GZIPMODE(51), 82, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8+delta2" },
  { GZIPMODE(52), 83, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8+delta3" },
  { GZIPMODE(53), 84, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8+delta4" },
  { GZIPMODE(54), 89, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "8+ppc"  },
  { GZIPMODE(55), 90, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9"  },
  { GZIPMODE(56), 91, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9+delta1" },
  { GZIPMODE(57), 92, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9+delta2" },
  { GZIPMODE(58), 93, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9+delta3" },
  { GZIPMODE(59), 94, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9+delta4" },
  { GZIPMODE(60), 99, /*XPKMF_A3000SPEED*/ 0, 0, 0,   62, 1525, 0, 0, "9+ppc"  },
  { NULL,        100, /*XPKMF_A3000SPEED*/ 0, 0, 0,   52, 1525, 0, 0, "9+extreme" }
#else
  { NULL,        100, /*XPKMF_A3000SPEED*/ 0, 0, 0,   113, 9180, 867, 0, "7+extreme" }
#endif
};

STATIC struct XpkInfo GZIPInfo = {
  1,				/* info version */
  VERSION,			/* lib  version */
  2,				/* master vers  */
  0,				/* pad          */
  "L2XZ",			/* short name   */
  "LZMA2 XZ",	/* long name    */
  "Lempel-Ziv-Markov (LZMA) 2 compressor with delta and PPC filters", /* description  */
//	"LZ77 class packer with hashing and dynamic Huffman coding", /* description  */
  MAKE_ID('L','2','X','Z'),		/* 4 letter ID  */
  XPKIF_PK_CHUNK |	/* flags        */
  XPKIF_UP_CHUNK | XPKIF_MODES | XPKIF_CHECKING,
  UINT32_MAX,		/* max in chunk */
  0,				/* min in chunk */
  UINT32_MAX,			/* def in chunk */
  "xz-packing",		/* pk message   */
  "xz-unpacking",		/* up message   */
  "xz-packed",		/* pk past msg  */
  "xz-unpacked",		/* up past msg  */
  LZMA_PRESET_DEFAULT * 10,				/* def mode     */
  0,				/* pad          */
  GZIPMODE(0),		/* modes        */
  {0,}				/* reserved     */
};

struct XpkInfo * _XpkSub_XpksPackerInfo(struct XpkSubIFace *Self)
{

	return &GZIPInfo;

}
