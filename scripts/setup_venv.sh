python -m venv venv
source venv/bin/activate
pip install -r requirements_tox.txt
python -m build
python -m pip install dist/pyconcord-0.3.2-cp310-cp310-macosx_14_0_arm64.whl