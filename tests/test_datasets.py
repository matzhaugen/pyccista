from concord import datasets
import numpy as np
def test_erodos_renyi_graph():
    cov, prec = datasets.erodos_renyi_graph(10)

    # diagonally dominant
    assert all(np.sum(np.abs(cov), axis=1) - 1 >= 0)
    assert all(np.diag(cov) == 1)
    assert all(np.diag(prec) >= 1)