%module ccista
%{
#include <errno.h>
#include "ccista.h"

#define SWIG_FILE_WITH_INIT
  %}

%include "numpy.i"

%init %{
  import_array();
  %}

%exception ccista
{
  errno = 0;
  $action

    if (errno != 0)
      {
        switch(errno)
	  {
	  case EPERM:
	    PyErr_Format(PyExc_IndexError, "Index error");
	    break;
	  case ENOMEM:
	    PyErr_Format(PyExc_MemoryError, "Not enough memory");
	    break;
	  default:
	    PyErr_Format(PyExc_Exception, "Unknown exception");
	  }
        SWIG_fail;
      }
}

%apply (double*  IN_ARRAY2, int DIM1, int DIM2) {(double  *x_i, int  x_i_dim1, int x_i_dim2)}

%apply (int*     IN_ARRAY1, int DIM1)           {(int     *i_i, int  i_i_dim)}
%apply (int*     IN_ARRAY1, int DIM1)           {(int     *j_i, int  j_i_dim)}
%apply (double*  IN_ARRAY1, int DIM1)           {(double  *v_i, int  v_i_dim)}

%apply (int**    ARGOUTVIEWM_ARRAY1, int* DIM1) {(int    **i_o, int *i_o_dim)}
%apply (int**    ARGOUTVIEWM_ARRAY1, int* DIM1) {(int    **j_o, int *j_o_dim)}
%apply (double** ARGOUTVIEWM_ARRAY1, int* DIM1) {(double **v_o, int *v_o_dim)}

%include "ccista.h"
