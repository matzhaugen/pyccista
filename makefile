SHELL := /bin/bash

# Run outside make
setup: uv venv .venv && source ./.venv/bin/activate
install:
	uv pip install .
test:
	pytest -s
.PHONY: test source setup