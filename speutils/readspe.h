// readspe.h

#ifndef _READSPE_H
#define _READSPE_H
#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

GLOBAL int read_spe_header(char *, WINXHEADER_STRUCT *);
GLOBAL int read_spe_data(char *, double *, WINXHEADER_STRUCT );
GLOBAL int poly(int , double *, int , double *);
GLOBAL double poly0(double x, int norder, double *coef);
GLOBAL void dump_spectrum(char *s, int n, double *wl, double *spectrum);

#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL
#endif
