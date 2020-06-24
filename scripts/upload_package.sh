#!/bin/bash
python -m twine upload --skip-existing --repository-url ${REPOSITORY_URL} io/wheelhouse/* -u __token__ -p ${TWINE_TOKEN_TEST}