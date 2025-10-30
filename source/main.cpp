int main(int argc, char** argv) {
    Rope::Rope rope("Hello World");
    
    rope.insert(3, "what");
    rope.remove(3, 5);
    rope.split(6);
}