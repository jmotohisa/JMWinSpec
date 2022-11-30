
#ifdef HAS_PLPLOT
#include <plplot.h>
#include "plplotsubs0.h"
#else
#define PLFLT double
#endif

#define MAX(a,b) a>b ? a:b
#define MIN(a,b) a<b ? a:b

#ifdef HAS_PLPLOT
// 1d plot
void plot1_plplot(int n,PLFLT *x,PLFLT *y1,char *title)
{

  PLFLT xmin=1e30,xmax=-1e30,ymax=-1e30,ymin=1e30;
  int i;
  //  PLINT just=0,axis=0;
  //  PLBOOL mode=1,status;
  
  pladv(0);
  for(i=0;i<n;i++)
	{
	  xmin=MIN(xmin,*(x+i));
	  xmax=MAX(xmax,*(x+i));
	  ymin=MIN(ymin,*(y1+i));
	  ymax=MAX(ymax,*(y1+i));
	}
  plvpor(0.15, 0.85, 0.1, 0.9);
  plwind(xmin, xmax, ymin, ymax);
  plcol0(1);
  //  plbox("bcnst", 0.0, 0, "bnstv", 0.0, 0);
  plbox("bcfghnst", 0.0, 0, "bcghnstv", 0.0, 0);

  plcol0(2);
  plline(n,x,y1);
  pllab ("wavelenghth", "intensity" , title);
  plflush();
}

// 2d plot: shade
void plot2d_plshade_plplot(int nx, int ny, PLFLT *x, PLFLT *y, PLFLT *u, char *title)
{
  PLFLT **z;
  int i,j;
  //  plsdev("aqt");
  //		plsdev("xwin");
  //  plinit();
  plAlloc2dGrid(&z, (PLINT) nx, (PLINT) ny);
  for(i=0;i<nx;i++)
	for(j=0;j<ny;j++)
	  z[i][j] = *(u+i+j*nx);
  plot2d_plshade0_plplot(nx, ny, x, y, z, title);
  plFree2dGrid(z,nx,ny);
  //  plend();
}

void plot2d_plshade0_plplot(int nx, int ny, PLFLT *x, PLFLT *y,PLFLT **z, char *title)
{
  PLFLT xmin=1e30,xmax=-1e30,ymax=-1e30,ymin=1e30;
  int i,j;
  int ns=20;
  PLFLT *clevel,*shedge;
  PLFLT zmin,zmax;
  PLINT fill_width = 2, cont_color = 0, cont_width = 0;
  
  clevel = (PLFLT *) calloc(ns, sizeof(PLFLT));
  shedge = (PLFLT *) calloc(ns+1, sizeof(PLFLT));
  minmax2d_plplot(z,nx,ny, &zmin, &zmax);
  for(i=0;i<nx;i++)
	{
	  xmin=MIN(xmin,*(x+i));
	  xmax=MAX(xmax,*(x+i));
	}
  for(j=0;j<ny;j++)
	{
	  ymin=MIN(ymin,*(y+j));
	  ymax=MAX(ymax,*(y+j));
	}
	
  for (i = 0; i < ns; i++)
	clevel[i] = zmin + (zmax - zmin) * (i + 0.5) / (PLFLT) ns;
  
  for (i = 0; i < ns+1; i++)
	shedge[i] = zmin + (zmax - zmin) * (PLFLT) i / (PLFLT) ns;
  
  //  plsdev("aqt");
  //  plsdev("xwin");
  //  plinit();

  
/* Plot using identity transform */
/*  Both x and y are NULL */

  pladv(0);
  plvpor(0.1, 0.9, 0.1, 0.9);
  plwind((PLFLT) xmin,(PLFLT) xmax,(PLFLT) ymin,(PLFLT) ymax);
		 
  plcol0(1);
  plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);

  plcol0(2);
  plpsty(0);
  plshades(z, nx, ny, NULL, xmin,xmax,ymin,ymax,
		   shedge, ns+1, fill_width,
		   cont_color, cont_width,
		   plfill, 1, NULL, NULL);
/*
  plcont(w, nx, ny, 1, nx, 1, ny, clevel, ns, mypltr, NULL);
    */
  pllab("wavelength", "pixel", title);
  //  plend();
  free(clevel);
  free(shedge);
}

// 2d plot: surface plot

void plot2d_plsurf3d_plplot(int nx, int ny, double *u, char *title)
{
  PLFLT **z;
  int i,j;
  //  plsdev("aqt");
  //		plsdev("xwin");
  //  plinit();
  plAlloc2dGrid(&z, (PLINT) nx, (PLINT) ny);
  for(i=0;i<nx;i++)
	for(j=0;j<ny;j++)
	  z[i][j] = -*(u+i+j*nx);
  plot2d_plsurf3d0_plplot(nx, ny, z, title);
  plFree2dGrid(z,nx,ny);
  //  plend();
}

void plot2d_plsurf3d0_plplot(int nx, int ny, PLFLT **z, char *title)
{
  int i,j;
  int ns=10;
  PLFLT *x,*y;
  PLFLT *clevel;
  PLFLT zmin,zmax,step;
  PLINT fill_width = 2, cont_color = 0, cont_width = 0;

  x= (PLFLT *) calloc(nx,sizeof(PLFLT));
  y= (PLFLT *) calloc(ny,sizeof(PLFLT));
  for(i=0;i<nx;i++)
	*(x+i) = (double) (i - (nx / 2))/ ((double) (nx/2));
  for(i=0;i<ny;i++)
	*(y+i) = (double) (i - (ny / 2))/ ((double) (ny/2) );
  clevel = (PLFLT *) calloc(ns, sizeof(PLFLT));
  //  minmax2d_plplot(z,nx,ny, &zmin, &zmax);

  plMinMax2dGrid(z, nx, ny, &zmax, &zmin);
  step = (zmax-zmin)/(ns+1);
  for (i=0; i<ns; i++)
    clevel[i] = zmin + step + step*i;

  pllightsource(1.,1.,1.);

  pladv(0);
  plvpor(0.0, 1.0, 0.0, 0.9);
  plwind(-1.0, 1.0, -0.9, 1.1);
  plcol0(3);
  //  plmtex("t", 1.0, 0.5, 0.5, title[k]);
  plcol0(1);
  plw3d(1.0, 1.0, 1.0, -1., 1, -1., 1., zmin, zmax, 60, 30);
  
  plbox3("bnstu", "x axis", 0.0, 0,
	     "bnstu", "y axis", 0.0, 0,
	     "bcdmnstuv", "z axis", 0.0, 0);
  plcol0(2);
  
  cmap1_init(0);
  plsurf3d(x, y, z, nx, ny, MAG_COLOR | FACETED, NULL, 0);
  free(x);
  free(y);
  free(clevel);
}

void minmax2d_plplot(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmin, PLFLT *fmax)
{
    int i, j;

    *fmax = f[0][0];
    *fmin = *fmax;

    for (i = 0; i < nx; i++) {
	for (j = 0; j < ny; j++) {
            *fmax = MAX(*fmax, f[i][j]);
            *fmin = MIN(*fmin, f[i][j]);
	}
    }
}

void
cmap1_init(int gray)
{
  PLFLT i[2], h[2], l[2], s[2];

  i[0] = 0.0;		/* left boundary */
  i[1] = 1.0;		/* right boundary */

  if (gray) {
    h[0] = 0.0;		/* hue -- low: red (arbitrary if s=0) */
    h[1] = 0.0;		/* hue -- high: red (arbitrary if s=0) */

    l[0] = 0.5;		/* lightness -- low: half-dark */
    l[1] = 1.0;		/* lightness -- high: light */

    s[0] = 0.0;		/* minimum saturation */
    s[1] = 0.0;		/* minimum saturation */
  } else {
    h[0] = 240; /* blue -> green -> yellow -> */
    h[1] = 0;   /* -> red */

    l[0] = 0.6;
    l[1] = 0.6;

    s[0] = 0.8;
    s[1] = 0.8;
  }

  plscmap1n(256);
  c_plscmap1l(0, 2, i, h, l, s, NULL);
}

// 1d multiple plot
void plot1_plplot2(int n, int nplot,PLFLT *x,PLFLT *y1,char *title)
{
  PLFLT xmin=1e30,xmax=-1e30,ymax=-1e30,ymin=1e30;
  int i;
  //  PLINT just=0,axis=0;
  //  PLBOOL mode=1,status;
  
  pladv(0);
  for(i=0;i<n;i++)
	{
	  xmin=MIN(xmin,*(x+i));
	  xmax=MAX(xmax,*(x+i));
	}
  for(i=0;i<n*nplot;i++)
	{
	  ymin=MIN(ymin,*(y1+i));
	  ymax=MAX(ymax,*(y1+i));
	}
  plvpor(0.15, 0.85, 0.1, 0.9);
  plwind(xmin, xmax, ymin, ymax);
  plcol0(1);
  //  plbox("bcnst", 0.0, 0, "bnstv", 0.0, 0);
  plbox("bcfghnst", 0.0, 0, "bcghnstv", 0.0, 0);

  plcol0(2);
  for(i=0;i<nplot;i++) {
	plline(n,x,y1+i*n);
  }
  pllab ("wavelenghth", "intensity" , title);
  plflush();
}
#endif
