# pyccista

This package provides the most basic concord-ista algorithm for
computing the concord estimate in python3. The algorithm is based on the paper by [Oh, Dalal, Khare and Rajaratnam](https://github.com/matzhaugen/pyccista.git).

Prerequisite for installing this package is the Eigen linear algebra
library in addition to numpy and scipy. In order to install this
package, issue the following command:

```python  
pip install pyconcord
```

To test if everything is fine, start python interpreter and execute
something like the following:

```python
from concord import concord
import numpy as np

x = np.random.randn(13, 9)
omega = concord(x, 0.3)

print(np.round(omega.todense(),2))
```

Should work on linux and mac. Not compiled on Windows platforms.

# Developing
If you want to run the tests or develop, run:
```
make develop
pytest -s
```

### __** PRs Welcome **__
