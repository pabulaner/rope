test: source/rope.hpp
	mkdir -p output
	g++ -std=c++20 -fsanitize=address -g test/test.cpp -o output/test