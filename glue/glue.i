%module(docstring="glue spectra") gluespectra

%{
  #define SWIG_FILE_WITH_INIT
  #include "../viewspe/readspe.h"
  #include "glue.h"
%}

%include "typemaps.i"
%include "numpy.i"
%init %{
  import_array();
%}

%feature("autodoc","1");

%apply (double* INPLACE_ARRAY1, int DIM1) {(double *T, int nt)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *N, int nn)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *S, int ns)}

%apply double *OUTPUT { (double *sint), (double *decay)}

%include "../viewspe/readspe.h"
%include "glue.h"

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

/* %pythoncode %{ */
/* from .define_params import ( */
/* 			    ParamSimulation, */
/* 			    ParamYB, */
/* 			    ParamO, */
/* 			    ParamHT, */
/* 			    init_vars, */
/* 			    runYB, */
/* 			    runYB1, */
/* 			    runO, */
/* 			    runHT, */
/* 			    ) */
/* from .rate_equation import ( */
/* 			   gaussianpulse, */
/* 			   func_YB_norm, */
/* 			   func_YB_norm_gaussian, */
/* 			   func_Oulton, */
/* 			   func_Oulton_gaussian, */
/* 			   func_S_from_n_Oulton_CW, */
/* 			   func_HT, */
/* 			   func_HT_gaussian, */
/* 			   func_for_findroot_ht_cw, */
/* 			   runYBcw0, */
/* 			   runOcw0, */
/* 			   runHTcw0, */
/* 			   runYBcw, */
/* 			   runOcw, */
/* 			   runHTcw, */
/* 			   ) */
/* %} */
  
