# Makefile.am
build: speutils

speutils: 
	python setup.py build_ext --inplace

install-local:
	python setup.py install

all-local: speutils

clean-local:
	rm -rf *.so __pycache__ build

