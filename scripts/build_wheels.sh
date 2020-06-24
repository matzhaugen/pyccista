#!/bin/bash
set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat "$PLAT" -w /io/wheelhouse/
    fi
}

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/requirements.txt
    "${PYBIN}/python3" setup.py build_ext -I eigen-3.3.7 bdist_wheel
done

# Bundle external shared libraries into the wheels
for whl in dist/*.whl; do
    repair_wheel "$whl"
done

# Install packages and test
# for PYBIN in /opt/python/cp38-cp38/bin/; do
#     "${PYBIN}/pip" install pyconcord --no-index -f /io/wheelhouse
# done
