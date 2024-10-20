# pyconcord

[![Gitter][gitter-badge]][gitter-link]

|      CI              | status |
|----------------------|--------|
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |


[gitter-badge]:            https://badges.gitter.im/pybind/Lobby.svg
[gitter-link]:             https://gitter.im/pybind/Lobby
[actions-badge]:           https://github.com/pybind/pyconcord/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/pybind/pyconcord/actions?query=workflow%3AConda
[actions-conda-badge]:     https://github.com/pybind/pyconcord/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/pybind/pyconcord/actions?query=workflow%3APip
[actions-pip-badge]:       https://github.com/pybind/pyconcord/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/pybind/pyconcord/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/pybind/pyconcord/workflows/Wheels/badge.svg

## Installation

- Clone this repository
- `brew install eigen`
- `cd [this_repo]`
- `uv venv .venv --python 3.9 && source .venv/bin/activate`
- `uv pip install .[test]`

## To run tests:
```
pytest -s
```

## To install from pypi:
`uv pip install pyconcord --no-sources`

## Test call

```python
import numpy as np
from scipy.sparse import identity
import pyconcord as pc
x = np.random.randn(13, 9)

n, p = x.shape
x0 = identity(p).tocoo()
omega = pc.concord(x, x0=x0, lambda1=0.3)
cov = np.round(omega.todense(), 2)
assert ~np.isinf(cov[0,0])
assert cov[0, 1] == cov[1, 0]
```

## Files

This example has several files that are a good idea, but aren't strictly
necessary. The necessary files are:

* `pyproject.toml`: The Python project file
* `CMakeLists.txt`: The CMake configuration file
* `src/main.cpp`: The source file for the C++ build
* `src/pyconcord/__init__.py`: The Python portion of the module. The root of the module needs to be `<package_name>`, `src/<package_name>`, or `python/<package_name>` to be auto-discovered.

These files are also expected and highly recommended:

* `.gitignore`: Git's ignore list, also used by `scikit-build-core` to select files for the SDist
* `README.md`: The source for the PyPI description
* `LICENSE`: The license file

There are also several completely optional directories:

* `.github`: configuration for [Dependabot][] and [GitHub Actions][]
* `conda.recipe`: Example recipe. Normally you should submit projects to conda-forge instead of building them yourself, but this is useful for testing the example.
* `docs/`: Documentation
* `tests/`: Tests go here

And some optional files:

* `.pre-commit-config.yaml`: Configuration for the fantastic static-check runner [pre-commit][].

This is a simplified version of the recommendations in the [Scientific-Python
Development Guide][], which is a _highly_ recommended read for anyone
interested in Python package development (Scientific or not). The guide also
has a cookiecutter that includes scikit-build-core and pybind11 as a backend
choice.

### CI Examples

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [cibuildwheel][].

## License

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

[cibuildwheel]: https://cibuildwheel.readthedocs.io
[scientific-python development guide]: https://learn.scientific-python.org/development
[dependabot]: https://docs.github.com/en/code-security/dependabot
[github actions]: https://docs.github.com/en/actions
[pre-commit]: https://pre-commit.com
[nox]: https://nox.thea.codes
[pybind11]: https://pybind11.readthedocs.io
[scikit-build-core]: https://scikit-build-core.readthedocs.io
[scikit-build (classic)]: https://scikit-build.readthedocs.io
