#!/bin/bash
python -m twine upload --repository-url https://test.pypi.org/legacy/ io/wheelhouse/* -u mahaugen -p ${TWINE_PASSWORD}