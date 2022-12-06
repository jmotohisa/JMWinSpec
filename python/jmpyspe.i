%module(docstring="JM pyspe package") jmpyspe

%{
  #define SWIG_FILE_WITH_INIT
  #include "pyspeconvert.h"
  #include "pyglue2.h"
%}

%include "typemaps.i"
%include "numpy.i"
%init %{
  import_array();
%}

%feature("autodoc","1");

%apply (double *INPLACE_ARRAY1, int DIM1) {(double *spectrum, int n_spectrum)}
%apply (double *INPLACE_ARRAY1, int DIM1) {(double *coef, int n_coef)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *wl_dest, int n_dest)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *spectrum_dest,int n_spectrum_dest)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *flg, int n_flg)}
%apply int *OUTPUT {(int *xdim),(int *ydim),(int *NumFrames),(int *flag_wlcen),(int *ierror)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *spectrum1_dest, int n_spectrum1_dest)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *flg1_dest, int n_flg1_dest)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *spectrum2_dest, int n_spectrum2_dest)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *flg2_dest, int n_flg2_dest)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *spectrum_2glued,int n_spectrum_2glued)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *flg_2glued, int n_flg_2glued)}

%include "pyspeconvert.h"
%include "pyglue2.h"

%exception {
    errno = 0;
    $action
    if (errno != 0)
    {
        switch(errno)
        {
            case ENOMEM:
                PyErr_Format(PyExc_MemoryError, "Failed malloc()");
                break;
            default:
                PyErr_Format(PyExc_Exception, "Unknown exception");
                }
                SWIG_fail;
        }
 }

%pythoncode %{
from readspe import (
		     readspe,
		     checkspecalib,
		     readspecomments,
		     printspespan,
		     writespectrum_csv,
		     )
from glueprocs import (
		       glue2spe,
		       gluespe1,
		       gluemultiplespe,
		       )
  %}

