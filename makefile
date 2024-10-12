build:
	uv build
test:
	rm -rf .venv .pytest_cache dist && uv cache clean && uv venv .venv && uv build --no-cache --wheel && uv pip install dist/pybind11_numpy_example-1.0.1-cp39-cp39-macosx_14_0_arm64.whl --no-cache && 	
