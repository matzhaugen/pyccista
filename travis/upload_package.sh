#!/bin/bash
python -m twine upload --repository-url https://test.pypi.org/legacy/ /wheelhouse/* -u mahaugen -p ${TWINE_PASSWORD}