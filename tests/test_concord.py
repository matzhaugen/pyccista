from concord import concord
import numpy as np


def test_basic():
    x = np.random.randn(13, 9)
    omega = concord(x, 0.3)

    cov = np.round(omega.todense(), 2)
    assert cov[0, 1] == cov[1, 0]
