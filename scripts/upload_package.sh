#!/bin/bash
python -m twine upload --skip-existing --repository-url ${REPOSITORY_URL} wheelhouse/*