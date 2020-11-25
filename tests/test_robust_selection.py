import numpy as np
from concord import robust_selection, concord, datasets
from sklearn.covariance import graphical_lasso
from sklearn.metrics import matthews_corrcoef
import time


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

    # Test Concord with Glasso as comparison
    # We estimate the number of edges in an erdos renyi graph and
    # compare with the true number of edges through the mathews correlation coefficient.
    # The results are printed and can be seen when running tests with
    # `pytest -s`
    def test_with_true_cov_known(self):
        n = 1000
        d = 100  # number of nodes
        n_experimens = 1
        mcc = np.zeros(n_experimens)
        mcc_concord = np.zeros(n_experimens)
        sigma, omega = datasets.erodos_renyi_graph(d, edge_fraction=0.1)
        for i in np.arange(n_experimens):
            if not (i % 10):
                print(i)

            data = np.random.multivariate_normal(np.zeros(d), sigma, n)
            data -= data.mean(axis=0)
            data /= data.std(axis=0)
            emp_cov = np.dot(data.T, data) / n
            optimal_lambda = robust_selection(data)
            t_start = time.time()
            omega_concord = concord(data, optimal_lambda)
            t_end = time.time()
            print(f"Concord time: {t_end - t_start}")
            t_start = time.time()
            sigma_lasso, omega_lasso = graphical_lasso(emp_cov, optimal_lambda)
            t_end = time.time()
            print(f"Lasso time: {t_end - t_start}")

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

        assert True


if __name__ == "__main__":
    TestCVConcord().generate_test_data()
