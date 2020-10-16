import numpy as np
from math import comb
np.set_printoptions(suppress=True)
TOL = 1e-10


def erodos_renyi_graph(n_nodes, edge_fraction=0.1):
    """Return covariance and precision matrix from an Erdos-Renyi graph

    Args:
        n_nodes (int): Number of nodes
        edge_fraction (float, optional): Fraction of edges that have non-zero weighting,
        a number between 0 and 1.

    Returns:
        sigma: Covariance matrix
        omega: Precision matrix
    """
    n_possible_edges = comb(n_nodes, 2)

    edges = np.ones(n_possible_edges)
    edges[np.random.rand(n_possible_edges) > edge_fraction] = 0

    weights = np.random.rand(n_possible_edges) / 2 + 0.5
    negative_weights_index = np.random.rand(n_possible_edges) > 0.5
    weights[negative_weights_index] *= -1

    raw_omega = np.zeros((n_nodes, n_nodes))
    raw_omega[np.tril_indices(n_nodes, -1)] = edges * weights
    raw_omega[np.triu_indices(n_nodes, 1)] = edges * weights
    raw_omega[np.diag_indices(n_nodes)] = 1

    # Make diagonally dominant
    row_sum = np.sum(np.abs(raw_omega), axis=1)
    scaled_raw_omega = raw_omega / row_sum[:, np.newaxis] / 1.5
    sym_scaled_omega = (scaled_raw_omega + scaled_raw_omega.T) / 2
    sym_scaled_omega[np.diag_indices(n_nodes)] = 1

    A_inv = np.linalg.inv(sym_scaled_omega)
    A_inv_diag = (np.diagonal(A_inv)).reshape(n_nodes, 1)
    scale = np.sqrt(A_inv_diag * A_inv_diag.T)

    sigma = A_inv / scale
    omega = np.linalg.inv(sigma)
    omega[np.abs(omega) < TOL] = 0
    return sigma, omega
