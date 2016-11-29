/* plplotsub.h */

#ifndef _PLPLOTSUB_H
#define _PLPLOTSUB_H

#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL
#else
#define	GLOBAL extern
#endif

//GLOBAL PLFLT *x_plplotsubs;
//GLOBAL PLFLT *y11_plplotsubs;
//GLOBAL PLFLT *y12_plplotsubs;
//GLOBAL PLFLT *y21_plplotsubs;
//GLOBAL PLFLT *y22_plplotsubs;

//GLOBAL void plot1_plplotsubs(int );
//GLOBAL void plot2_plplotsubs(int );
//GLOBAL void plot22_plplotsubs(int );

//GLOBAL void plot1_plplotcall(int , double  * ,  double  *);
//GLOBAL void plot2_plplotcall(int , double  * ,  double  * , double  *);
//GLOBAL void plot22_plplotcall(int , double  * ,  double  * , double  * , double  *, double  *);

GLOBAL void plot1_plplot(int , PLFLT * ,  PLFLT *, char *);
GLOBAL void plot1_plplot2(int , int , PLFLT * ,  PLFLT *, char *);
//GLOBAL void plot2_plplot(int , PLFLT * ,  PLFLT * , PLFLT *);
//GLOBAL void plot22_plplot(int , PLFLT * ,  PLFLT * , PLFLT * , PLFLT *, PLFLT *);

GLOBAL void plot2d_plshade_plplot(int , int , PLFLT *, PLFLT *, PLFLT *, char *);
GLOBAL void plot2d_plshade0_plplot(int , int , PLFLT *, PLFLT *, PLFLT **, char *);
GLOBAL void plot2d_plsurf3d_plplot(int , int , double *, char *);
GLOBAL void plot2d_plsurf3d0_plplot(int , int , PLFLT **, char *);

//GLOBAL void subst_linearray_2dGrid(double *, PLFLT **, int , int );
//GLOBAL void subst_linearray_diff_2dGrid(double *, double *, PLFLT **, int , int );
GLOBAL void minmax2d_plplot(PLFLT **, PLINT , PLINT , PLFLT *, PLFLT *);
GLOBAL void cmap1_init(int );

#undef GLOBAL_VALUE_DEFINE
#undef GLOBAL
#endif
