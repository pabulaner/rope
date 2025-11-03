#pragma once

#include <cstddef>
#include <utility>
#include <functional>

/// The rope namespace
namespace Rope {

/// The base class for inner and outer nodes
template <typename T>
struct Node {
    /// True if it is an inner node
    bool isInner;

    /// The size of the node
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
    /// The root node
    Node<T>* root;

public:
    /// Constructs an empty rope
    Rope();

    /// Constructs a rope with the provided string
    Rope(size_t size, T* str);

    /// Destructor
    ~Rope();

    /// Copy constructor
    Rope(const Rope<T>& other);

    /// Move constructor
    Rope(Rope<T>&& other);

    /// Copy assignment operator
    Rope<T>& operator=(const Rope<T>& other);

    /// Move assignment operator
    Rope<T>& operator=(Rope<T>&& other);

    // Appends the provided rope to the end
    void append(Rope<T>&& other);

    // Inserts the provided rope at the provided index
    void insert(size_t index, Rope<T>&& other);

    // Erases the string between the provided begin and end
    void erase(size_t begin, size_t end);

    // Clears the rope
    void clear();

    // Splits the rope at the provided index
    // This rope is emptied durring the process
    std::pair<Rope<T>, Rope<T>> split(size_t index);

    // Returns an array with the entire data of the rope
    T* array();

    // Returns the data at the provided index
    T& at(size_t index) const;

    // Returns the size of the rope
    size_t size() const;

private:
    // Creates an inner node
    static Inner<T>* createInner(const Node<T>* left, const Node<T>* right);

    // Creates an outer node
    static Outer<T>* createOuter(size_t size, const T* data);

    // Copies the provided node
    static Node<T>* copy(const Node<T>* node);

    // Clears the provided node
    static void clear(Node<T>* node);

    // Copies all data from the provided node into the provided pointer
    static void array(Node<T>* node, T* begin);

    // Returns the data at the provided index for the provided node
    static T& at(Node<T>* node, size_t index);

    // Returns the size of the provided node
    static size_t size(const Node<T>* node);
};

template <typename T>
Rope<T>::Rope() 
    : root(nullptr)
{
    // empty
}

template <typename T>
Rope<T>::Rope(size_t size, T* data) 
    : root(createOuter(size, data))
{
    // empty
}

template <typename T>
Rope<T>::~Rope() {
    clear();
}

template <typename T>
Rope<T>::Rope(const Rope<T>& other) 
    : root(copy(other.root))
{
    // empty
}

template <typename T>
Rope<T>::Rope(Rope<T>&& other)
    : root(other.root) 
{
    other.root = nullptr;
}

template <typename T>
Rope<T>& Rope<T>::operator=(const Rope<T>& other) {
    clear();
    root = copy(other.root);
}

template <typename T>
Rope<T>& Rope<T>::operator=(Rope<T>&& other) {
    clear();

    root = other.root;
    other.root = nullptr;
}

template <typename T>
void Rope<T>::append(Rope<T>&& other) {

}

template <typename T>
void Rope<T>::insert(size_t index, Rope<T>&& other) {
}

template <typename T>
void Rope<T>::erase(size_t begin, size_t end) {
}

template <typename T>
void Rope<T>::clear() {
    if (root) {
        clear(root);
        root = nullptr;
    }
}

template <typename T>
std::pair<Rope<T>, Rope<T>> Rope<T>::split(size_t index) {
    return {{}, {}};
}

template <typename T>
T* Rope<T>::array() {
    T* data = new T[size()];
    array(root, data);

    return data;
}

template <typename T>
T& Rope<T>::at(size_t index) const {
    
}

template <typename T>
size_t Rope<T>::size() const {
    return root 
        ? size(root) 
        : 0;
}

template <typename T>
Inner<T>* Rope<T>::createInner(const Node<T>* left, const Node<T>* right) {
    Inner<T>* inner = new Inner<T>();

    inner->isInner = true;
    inner->size = size(left);
    inner->left = left;
    inner->right = right;

    return inner;
}

template <typename T>
Outer<T>* Rope<T>::createOuter(size_t size, const T* data) {
    Outer<T>* outer = new Outer<T>();

    outer->isInner = false;
    outer->size = size;
    outer->data = data;

    return outer;
}

template <typename T>
Node<T>* Rope<T>::copy(const Node<T>* node)
{
    if (node->isInner) {
        Inner<T>* inner = (Inner<T>*) node;
        Node<T>* left = copy(inner->left);
        Node<T>* right = copy(inner->right);

        return createInner(left, right);
    } else {
        Outer<T>* outer = (Outer<T>*) node;
        T* data = new T[outer->size];

        std::copy(outer->data, outer->data + outer->size, data);
        return createOuter(outer->size, data);
    }
}

template <typename T>
void Rope<T>::clear(Node<T>* node) {
    if (node->isInner) {
        Inner<T>* inner = (Inner<T>*) node;

        clear(inner->left);
        clear(inner->right);
    } else {
        Outer<T>* outer = (Outer<T>*) node;
        delete outer->data;
    }
}

template <typename T>
void Rope<T>::array(Node<T>* node, T* begin) {
    if (node->isInner) {
        Inner<T>* inner = (Inner<T>*) node;
        
        array(node->left, begin);
        array(node->right, begin + inner->size);
    } else {
        Outer<T>* outer = (Outer<T>*) outer;
        std::copy(outer->data, outer->data + outer->size, begin);
    }
}

template <typename T>
T& Rope<T>::at(Node<T>* node, size_t index) {
    if (node->isInner) {
        Inner<T>* inner = (Inner<T>*) node;

        return index < inner->size
            ? at(inner->left, index)
            : at(inner->right, index - inner->size)
    } else {
        Outer<T>* outer = (Outer<T>*) outer;
        return outer->data[index];
    }
}

template <typename T>
size_t Rope<T>::size(const Node<T>* node) {
    size_t total = node->size;

    if (node->isInner) {
        Inner<T>* inner = (Inner<T>*) node;
        total += size(inner->right);
    }
    
    return total;
}

} // namespace Rope