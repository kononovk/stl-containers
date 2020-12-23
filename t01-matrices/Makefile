.PHONY: clean compilegcc all smoke

all: matrices smoke

matrices: main.cpp matrices.cpp matrices.hpp
	clang++ -g -Wall -Wextra -std=c++17 -o matrices main.cpp matrices.cpp

compilegcc: smoke
	rm -f matrices
	g++ -Wall -Wextra -std=c++17 -o matrices main.cpp matrices.cpp

clean:
	rm -f matrices
	rm -rf .idea cmake-build-debug

smoke: matrices
	cd smoke_test && ./smoke_test.sh ../matrices
