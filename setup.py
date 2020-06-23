#! /usr/bin/env python
# Create Whl: python setup.py build_ext install sdist bdist_wheel
# Local installation: python -m pip install dist/[name-of-whl]
# Push to pip: python -m twine upload dist/*

# System imports
# from distutils.core import *
from setuptools import setup, Extension
from os import environ

# Third-party modules - we depend on numpy for everything
import numpy

from distutils.sysconfig import get_config_vars
(opt,) = get_config_vars('OPT')
environ['OPT'] = ' '.join(
    flag for flag in opt.split() if flag != '-Wstrict-prototypes'
)

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

# concord extension module
_ccista = Extension('_ccista',
                    ['concord/ccista.i', 'concord/ccista.cpp'],
                    include_dirs=[numpy_include, "eigen-3.3.7"],
                    swig_opts=['-c++'])

setup(name='pyconcord',
      description='Python implemetation of the concord algorithm for pseudo-likelihood graphical model selection',
      author='Sang-Yun Oh, Aydin Buluc, Onkar Dalal, Kshitij Khare, Bala Rajaratnam',
      author_email='syoh@lbl.gov',
      version='0.2.3',
      url='http://web.stanford.edu/~sangoh',
      packages=['concord'],
      install_requires=['numpy>=1.19.0'],
      ext_modules=[_ccista])
