from concord import datasets
import numpy as np
def test_erodos_renyi_graph():
    cov, prec = datasets.erodos_renyi_graph(10)

    assert np.sum(np.abs((np.linalg.inv(cov) - prec))) < 1e-8
    # diagonally dominant
    assert all(np.sum(np.abs(cov), axis=1) - 1 >= 0)
    assert all(np.diag(cov) == 1)
    assert all(np.diag(prec) >= 1)