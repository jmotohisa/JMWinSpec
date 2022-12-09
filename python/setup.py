#!/usr/bin/env python
# -*- coding: utf-8 -*-

# System imports
from distutils.core import setup, Extension

# Third-party modules - we depend on numpy for everything
import numpy
# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

# alloc extension module
ext_speutils = Extension('_speutils',
                         sources=['speutils.i', 'pyspeconvert.c', 'pyglue2.c',
                                  'glue.c', 'readspe.c'],
                         include_dirs=[numpy_include],
                         extra_compile_args=['--verbose'],
                         )

if __name__ == '__main__':
    setup(
        name='speutils',
        description='JM python binding for SPE utilites',
        author='Junichi Motohisa',
        version='0.0.0',
        ext_modules=[ext_speutils],
        install_requires=['numpy', 'matplotlib', 'pandas'],
        py_modules=['speutils'],
        packages=['speutils'],
    )
