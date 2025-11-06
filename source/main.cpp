#include <iostream>

#include "rope.hpp"

int main(int argc, char** argv) {
    char* str1 = new char[3] {'a', 'b', 'c'};
    char* str2 = new char[2] {'d', 'e'};

    Util::Rope<char> rope1(3, str1);
    Util::Rope<char> rope2(2, str2);

    std::cout << rope1 << std::endl;
    std::cout << rope2 << std::endl;

    rope1.append(std::move(rope2));

    std::cout << rope1 << std::endl;
    std::cout << rope2 << std::endl;
}