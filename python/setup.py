#!/usr/bin/env python
# -*- coding: utf-8 -*-

# System imports
from distutils.core import *
from distutils import sysconfig

# Third-party modules - we depend on numpy for everything
import numpy
# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

# alloc extension module
_glue = Extension("_glue",
                  sources=["glue.i", "pyspeconvert.c", "pyglue2.c"],
                  include_dirs=[numpy_include, '../speutils'],
                  libraries=['speutils'],
                  library_dirs=['../speutils'],
                  extra_compile_args=["--verbose"]
                  )

# NumyTypemapTests setup
setup(name="SPE Glue",
      description="Glue Spectra",
      author="Junichi Motohisa",
      version="1.0",
      ext_modules=[_glue]
      )
