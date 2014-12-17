def concord(x, penalty1, penalty2=0, x0=None, epstol=1e-5, maxitr=100, bb=0):

    from .ccista import ccista
    import scipy.sparse as sp
    from scipy.sparse import identity, coo_matrix

    n, p = x.shape

    if x0 is None:
        x0 = identity(p).tocoo()
    # else if type(x0) is not 'scipy.sparse.coo.coo_matrix':
    #     try:
    #         x0 = x0.tocoo()
    #     except:

    xf_i, xf_j, xf_data = ccista(
        x, x0.row, x0.col, x0.data, 
        penalty1, penalty2, epstol, maxitr, bb)

    return coo_matrix((xf_data, (xf_i, xf_j)),
                         shape=(p, p))


