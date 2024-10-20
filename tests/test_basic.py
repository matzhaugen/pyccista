from __future__ import annotations

import pyconcord as pc


# import pyconcord as pc
import numpy as np


n_values = [0, 1, 2, 17]

def test_basic_concord():
    x = np.random.randn(13, 9)

    n, p = x.shape
    
    # TODO: allow passing initial guess
    # x0 = identity(p).tocoo()
    # i, j, data = pc.concord(
    #     x, x0.row, x0.col, x0.data, penalty1=0.3, penalty2=0,
    # )
    # i, j, data = pc.concord(x, lambda1=0.3)
    # omega = csr_matrix((data, vstack((i, j))), shape=(p, p))
    print(pc.concord.__doc__)
    omega = pc.concord(x, lambda1=0.3)
    # omega = pc.concord(Y=x, lambda1=0.3)
    
    cov = np.round(omega.todense(), 2)
    assert ~np.isinf(cov[0,0])
    assert cov[0, 1] == cov[1, 0]

def test_concord_w_guess():
    from scipy.sparse import identity

    x = np.random.randn(13, 9)

    n, p = x.shape
    
    # TODO: allow passing initial guess
    x0 = identity(p).tocoo()
    # i, j, data = pc.concord(
    #     x, x0.row, x0.col, x0.data, penalty1=0.3, penalty2=0,
    # )
    # i, j, data = pc.concord(x, lambda1=0.3)
    # omega = csr_matrix((data, vstack((i, j))), shape=(p, p))
    print(pc.concord.__doc__)
    
    omega = pc.concord(x, x0=x0, lambda1=0.3)
    cov = np.round(omega.todense(), 2)
    assert ~np.isinf(cov[0,0])
    assert cov[0, 1] == cov[1, 0]

def test_speed():
    # Smaller lambda takes longer time
    lambdas = np.exp(np.linspace(np.log(5e-2), np.log(5e-1), 10))

    data_large = np.random.randn(200, 200)
    import time
    for lam in lambdas:
        start = time.time()
        sparse_arr = pc.concord(data_large, lambda1=lam)
        np.asarray(sparse_arr.todense())
        end = time.time()
        print(f"Elapsed for concord at lambda {lam:.3f} NZ={sparse_arr.nnz / (sparse_arr.shape[0]*(sparse_arr.shape[0]-1))*100:.2f}% time= {(end - start):.3f}s")

