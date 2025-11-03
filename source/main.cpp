#include <iostream>

#include "rope.hpp"

int main(int argc, char** argv) {
    char* str = "abcd";

    Rope::Rope<char> rope(4, str);
    std::cout.write(rope.array(), rope.size());
}