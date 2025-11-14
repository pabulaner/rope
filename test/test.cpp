#include <cassert>
#include <sstream>

#include "../source/rope.hpp"

#define ASSERT_SIZE(actual, expected) assert(actual.size() == expected)

#define ASSERT_DATA(actual, expected) { \
    std::ostringstream oss; \
    oss << actual; \
    assert(oss.str() == expected); \
}

char* heap(const char* str) {
    size_t size = strlen(str);
    char* res = new char[size];

    for (size_t i = 0; i < size; i++) {
        res[i] = str[i];
    }

    return res;
}

void testEmpty() {
    Util::Rope<char> rope = Util::Rope<char>::empty();

    ASSERT_SIZE(rope, 0);
    ASSERT_DATA(rope, "");
}

void testCopy() {
    char* str = heap("hello");
    auto rope = Util::Rope<char>::copy(5, str);

    ASSERT_SIZE(rope, 5);
    ASSERT_DATA(rope, "hello");
    
    str[1] = 'a';

    ASSERT_DATA(rope, "hello");
    delete[] str;
}

void testMove() {
    char* str = heap("hello");
    auto rope = Util::Rope<char>::move(5, str);

    ASSERT_SIZE(rope, 5);
    ASSERT_DATA(rope, "hello");
    
    str[1] = 'a';

    ASSERT_DATA(rope, "hallo");
}

void testAppend() {
    char* str1 = heap("hello");
    char* str2 = heap(" world");

    auto rope1 = Util::Rope<char>::move(5, str1);
    auto rope2 = Util::Rope<char>::move(6, str2);
    
    rope1.append(std::move(rope2));

    ASSERT_SIZE(rope1, 11);
    ASSERT_SIZE(rope2, 0);

    ASSERT_DATA(rope1, "hello world");
    ASSERT_DATA(rope2, "");
}

void testSplit() {
    char* str = heap("hello world");
    auto rope = Util::Rope<char>::move(11, str);

    auto [left, right] = rope.split(5);

    ASSERT_SIZE(rope, 0);
    ASSERT_SIZE(left, 5);
    ASSERT_SIZE(right, 6);

    ASSERT_DATA(rope, "");
    ASSERT_DATA(left, "hello");
    ASSERT_DATA(right, " world");
}

int main(int argc, char** argv) {
    testEmpty();
    testCopy();
    testMove();
    testAppend();
    testSplit();

    return 0;
}