build:
	uv build
test:
	rm -rf .venv && uv venv && uv build --refresh && uv pip install . -v --refresh && uv run pytest tests --pdb
