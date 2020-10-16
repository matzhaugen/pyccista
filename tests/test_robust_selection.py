import numpy as np
from concord import robust_selection, concord
from sklearn.covariance import graphical_lasso
from sklearn.metrics import matthews_corrcoef
from math import comb

d = 200  # number of nodes
TOL = 1e-10


def generate_test_model(d):
    n_possible_edges = comb(d, 2)

    edges = np.ones(n_possible_edges)
    edges[np.random.rand(n_possible_edges) > 0.1] = 0

    weights = np.random.rand(n_possible_edges) / 2 + 0.5
    negative_weights_index = np.random.rand(n_possible_edges) > 0.5
    weights[negative_weights_index] *= -1

    omega = np.zeros((d, d))
    omega[np.tril_indices(d, -1)] = edges * weights
    omega[np.triu_indices(d, 1)] = edges * weights
    omega[np.diag_indices(d)] = 1

    # Make diagonally dominant
    row_sum = np.sum(np.abs(omega), axis=1)
    scaled_omega = omega / row_sum / 1.5
    sym_scaled_omega = (scaled_omega + scaled_omega.T) / 2
    sym_scaled_omega[np.diag_indices(d)] = 1

    A_inv = np.linalg.inv(sym_scaled_omega)
    A_inv_diag = (np.diagonal(A_inv)).reshape(d, 1)
    scale = np.sqrt(A_inv_diag * A_inv_diag.T)

    sigma = A_inv / scale
    new_omega = np.linalg.inv(sigma)
    new_omega[new_omega < TOL] = 0

    return sigma, new_omega


def mathews_correlation_coefficient(edges, edges_hat):
    TP = np.sum(edges_hat[edges == True] == edges[edges == True])
    TN = np.sum(edges_hat[edges == False] == edges[edges == False])
    FN = np.sum(np.logical_not(edges_hat[edges == True]) == edges[edges == True])
    FP = np.sum(np.logical_not(edges_hat[edges == False]) == edges[edges == False])
    num = TP * TN - FP * FN
    denom = np.sqrt((TP + FP) * (TP + FN) * (TN + FP) * (TN + FP))
    return num / denom


class TestCVConcord:
    def test_cv_concord(self):
        x = np.random.rand(100, 3)
        optimal_lambda = robust_selection(x)

        assert optimal_lambda > 0

    def test_with_true_cov_known(self):
        n = 1000
        n_experimens = 2
        mcc = np.zeros(n_experimens)
        mcc_concord = np.zeros(n_experimens)
        sigma, omega = generate_test_model(d)
        for i in np.arange(n_experimens):
            if not (i % 10):
                print(i)

            data = np.random.multivariate_normal(np.zeros(d), sigma, n)
            data -= data.mean(axis=0)
            data /= data.std(axis=0)
            emp_cov = np.dot(data.T, data) / n
            optimal_lambda = robust_selection(data)
            omega_concord = concord(data, optimal_lambda)
            sigma_lasso, omega_lasso = graphical_lasso(emp_cov, optimal_lambda)

            edges_lasso = np.triu(omega_lasso > 1e-8, 1)
            edges_concord = np.triu(omega_concord.todense() > 1e-8, 1)
            edges = np.triu(omega != 0, 1)
            print(f"Optimal lambda (RS): {optimal_lambda}")
            print(f"# of edges: {np.sum(edges)}")
            print(f"# of estimated edges (concord): {np.sum(edges_concord)}")
            print(f"# of estimated edges (lasso): {np.sum(edges_lasso)}")
            mcc[i] = mathews_correlation_coefficient(edges, edges_lasso)
            mcc_concord[i] = mathews_correlation_coefficient(edges, edges_concord)
            matthews_corrcoef(edges.flatten(), edges_concord.flatten())

        print(np.mean(mcc), np.std(mcc))
        print(np.mean(mcc_concord), np.std(mcc_concord))

        assert False


if __name__ == "__main__":
    TestCVConcord().generate_test_data()
