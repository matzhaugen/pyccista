def concord(x, penalty, x0=None, epstol=1e-5, maxitr=100, bb=0):

    from .ccista import ccista
    import scipy.sparse as sp

    n, p = x.shape

    if x0 is None:
        
        x0 = sp.identity(p).tocoo()

    xf_i, xf_j, xf_data = ccista(
        x, x0.row, x0.col, x0.data, 
        penalty, epstol, maxitr, bb)

    return sp.coo_matrix((xf_data, (xf_i, xf_j)),
                         shape=(p, p))

