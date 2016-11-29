// readspe.h

#ifndef _READSPE_H
#define _READSPE_H
#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

GLOBAL int read_spe_header(char *, WINXHEADER_STRUCT *);

#ifdef HAS_PLPLOT
#include <plplot.h>
GLOBAL int read_spe_data(char *, PLFLT *, WINXHEADER_STRUCT );
GLOBAL int poly(int , PLFLT *, int , double *);
#else
GLOBAL int read_spe_data(char *, double *, WINXHEADER_STRUCT );
GLOBAL int poly(int , double *, int , double *);
#endif

#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL
#endif
