import numpy as np


def robust_selection(data, B=1000, alpha=0.9):
    n, p = data.shape
    A = np.cov(data.T)
    indeces = np.random.choice(np.arange(n), (n, B))
    R_star = np.zeros(B)
    for b in np.arange(B):

        data_star = data[indeces[:, b], :]
        A_star = np.cov(data_star.T)
        R_star[b] = np.amax(A - A_star)

    optimal_lambda = np.quantile(R_star, q=(1 - alpha))
    return optimal_lambda
