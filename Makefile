test: source/rope.hpp
	g++ -std=c++20 -fsanitize=address -g test/test.cpp -o output/test