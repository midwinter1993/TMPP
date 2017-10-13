a.out: tmpp.cpp
	clang++ -std=c++14 $^ -o $@
