#pragma once

#define SIZE_MASK (((size_t) -1) >> 1)

/// The rope namespace
namespace Rope {

/// The base class for inner and outer nodes
template <typename T>
struct Node {
    /// The size of the node
    /// The most significant bit indicates if it is an inner (1) or outer (0) node
    /// If it is a inner node the size is the size of all left children
    /// If it is a outer node the size is the size of the data
    size_t size;
};

/// A inner node of the rope
template <typename T>
struct Inner : public Node<T> {
    /// The left child
    Node<T>* left;

    /// The right child
    Node<T>* right;
};

// A outer node of the rope
template <typename T>
struct Outer : public Node<T> {
    /// The data
    T* data;
};

/// The rope data structure
template <typename T>
class Rope {
private:
    /// The size of the entire data
    size_t size;

    /// The root node
    Node<T>* root;

public:
    /// Constructs an empty rope
    Rope();

    /// Constructs a rope with the provided string
    Rope(size_t size, T* str);

    /// Destructor
    ~Rope();

    // Appends the provided rope to the end
    void append(Rope<T>&& other);

    // Inserts the provided rope at the provided index
    void insert(size_t index, Rope<T>&& other);

    // Erases the string between the provided begin and end
    void erase(size_t begin, size_t end);

    // Splits the rope at the provided index
    // This rope is emptied durring the process
    std::pair<Rope<T>> split(size_t index);

    // Returns the data at the provided index
    T& at(size_t index) const;

    // Returns the size of the rope
    size_t size() const;

private:
    // Creates an inner node
    static Inner<T>* createInner(Node<T>* left, Node<T>* right);

    // Creates an outer node
    static Outer<T>* createOuter(size_t size, T* data);

    // Returns the size of the provided node
    static size_t size(Node<T>* node);

    // Returns true if the node is an inner node
    static bool isInner(Node<T>* node);
}

} // namespace Rope