/*
 *  readspe.h
 *  ViewSPE_cocoa
 *
 *  Created by Junichi Motohisa on 11/09/25.
 *  Copyright 2011 Junichi Motohisa. All rights reserved.
 *
 */

#ifndef _READSPE_H
#define _READSPE_H
#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

GLOBAL int read_spe_header(char *, WINXHEADER_STRUCT *);

#ifdef HAVE_PLPLOT
#include <plplot/plplot.h>
GLOBAL int read_spe_data(char *, PLFLT *, WINXHEADER_STRUCT );
GLOBAL int poly(int , PLFLT *, int , double *);
#else
GLOBAL int read_spe_data(char *, double *, WINXHEADER_STRUCT );
GLOBAL int poly(int , double *, int , double *);
#endif

#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL
#endif

