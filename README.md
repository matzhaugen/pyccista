This package provides the most basic concord-ista algorithm for
computing the concord estimate.

Prerequisite for installing this package is the Eigen linear algebra
library in addition to numpy and scipy. In order to install this
package, issue the following command:

```python    
python setup.py build_ext -I[path to Eigen library] install
```

To test if everything is fine, start python interpreter and execute
something like the following:

```python
from concord import concord
import numpy as np

x = np.random.randn(13, 9)
omega = concord(x, 0.3)

print np.round(omega.todense(),2)
```

