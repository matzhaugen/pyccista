#! /usr/bin/env python

# System imports
from distutils.core import *
from distutils      import sysconfig

# Third-party modules - we depend on numpy for everything
import numpy

from distutils.sysconfig import get_config_vars

(opt,) = get_config_vars('OPT')
os.environ['OPT'] = ' '.join(
    flag for flag in opt.split() if flag != '-Wstrict-prototypes'
)

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

# concord extension module
_ccista = Extension('_ccista',
                  ['concord/ccista.i','concord/ccista.cpp'],
                   include_dirs = [numpy_include],
                   swig_opts = ['-c++'])

setup(name        = 'concord',
      description = 'Python implemetation of the concord algorithm for pseudo-likelihood graphical model selection',
      author      = 'Sang-Yun Oh, Aydin Buluc, Onkar Dalal, Kshitij Khare, Bala Rajaratnam',
      author_email= 'syoh@lbl.gov',
      version     = '0.1b',
      url         = 'http://web.stanford.edu/~sangoh',
      packages    = ['concord'],
      ext_modules = [_ccista])