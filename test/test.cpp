#include <cassert>
#include <sstream>

#include "../source/rope.hpp"

#define BEGIN_TEST(name) void test##name() { \
    std::cout << "Running test '" << #name << "'" << std::endl;
#define END_TEST }

std::string toString(Util::Rope<char>& rope) {
    std::ostringstream oss;
    oss << rope;

    return oss.str();
}

void testEmpty() {
    Util::Rope<char> rope = Util::Rope<char>::empty();

    assert(rope.size() == 0);
    assert(toString(rope) == "");
}

void testCopy() {

}

void testMove() {

}

int main(int argc, char** argv) {
    testEmpty();
    return 0;
}