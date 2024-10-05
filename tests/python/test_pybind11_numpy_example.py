import pybind11_numpy_example as pne
import numpy as np
import pytest


n_values = [0, 1, 2, 17]

def test_basic_ccista():
    x = np.random.randn(13, 9)

    from scipy.sparse import identity, csr_matrix
    from numpy import vstack

    n, p = x.shape
    
    # TODO: allow passing initial guess
    # x0 = identity(p).tocoo()
    # i, j, data = pne.ccista(
    #     x, x0.row, x0.col, x0.data, penalty1=0.3, penalty2=0,
    # )
    # i, j, data = pne.ccista(
    #     x, lambda1=0.3
    # )
    # omega = csr_matrix((data, vstack((i, j))), shape=(p, p))
    omega = pne.ccista(x, lambda1=0.3)

    
    cov = np.round(omega.todense(), 2)
    assert cov[0, 1] == cov[1, 0]

# def test_basic():
#     x = np.random.randn(13, 9)
#     omega = pne.concord(x, 0.3)

#     cov = np.round(omega.todense(), 2)
#     assert cov[0, 1] == cov[1, 0]


@pytest.mark.parametrize("list_func", [pne.pure_python_list, pne.vector_as_list])
@pytest.mark.parametrize("n", n_values)
def test_pybind11_numpy_example_list(list_func, n):
    l = list_func(n)
    assert isinstance(l, list)
    assert len(l) == n
    for i in range(n):
        assert l[i] == i


@pytest.mark.parametrize(
    "ndarray_func", [pne.vector_as_array, pne.vector_as_array_nocopy]
)
@pytest.mark.parametrize("n", n_values)
def test_pybind11_numpy_example_ndarray(ndarray_func, n):
    a = ndarray_func(n)
    assert isinstance(a, np.ndarray)
    assert len(a) == n
    assert a.dtype == np.int16
    for i in range(n):
        assert a[i] == i
