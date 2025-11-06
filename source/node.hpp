#pragma once

/// The rope namespace
namespace Rope {

/// An inner node of the rope
template <typename T>
struct Inner;

/// An outer node of the rope
template <typename T>
struct Outer;

/// The base class for inner and outer nodes
template <typename T>
class Node {
private:
    /// True if it is an inner node
    bool inner;

    /// The size of the node
    size_t size;

protected:
    /// Constructor
    Node(bool inner, size_t size);

public:
    /// Destructor
    ~Node();

    Node(const Node<T>& other);

    Node(Node<T>&& other);

    Node<T>& operator=(const Node<T>& other);

    Node<T>& operator=(Node<T>&& other);

    // Copies the provided node
    Node<T>* copy() const;

    // Copies all data from the provided node into the provided pointer
    void array(T* begin) const;

    // Returns the data at the provided index for the provided node
    T& at(size_t index) const;

    // Returns the total size of the provided node
    size_t total() const;
};

/// An inner node of the rope
template <typename T>
class Inner : public Node<T> {
private:
    /// The left child
    Node<T>* left;

    /// The right child
    Node<T>* right;
};

// An outer node of the rope
template <typename T>
class Outer : public Node<T> {
private:
    /// The data
    T* data;
};

template <typename T>
Node<T>::~Node() {
    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;

        delete inner->left;
        delete inner->right;
    } else {
        Outer<T>* outer = (Outer<T>*) this;
        delete outer->data;
    }
}

template <typename T>
Node<T>::Node(const Node<T>& other) 
    : inner(other.inner)
    , size(other.size) 
{
    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;
        
    }
}

template <typename T>
Node<T>::Node(Node<T>&& other) {
}

template <typename T>
Node<T>& Node<T>::operator=(const Node<T>& other) {
    // TODO: insert return statement here
}

template <typename T>
Node<T>& Node<T>::operator=(Node<T>&& other) {
    // TODO: insert return statement here
}

template <typename T>
Inner<T>* Node<T>::createInner(Node<T>* left, Node<T>* right) {
    Inner<T>* inner = new Inner<T>();

    inner->inner = true;
    inner->size = size(left);
    inner->left = left;
    inner->right = right;

    return inner;
}

template <typename T>
Outer<T>* Node<T>::createOuter(size_t size, T* data) {
    Outer<T>* outer = new Outer<T>();

    outer->inner = false;
    outer->size = size;
    outer->data = data;

    return outer;
}

template <typename T>
Node<T>* Node<T>::copy() const {
    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;
        Node<T>* left = copy(inner->left);
        Node<T>* right = copy(inner->right);

        return createInner(left, right);
    } else {
        Outer<T>* outer = (Outer<T>*) this;
        T* data = new T[outer->size];

        std::copy(outer->data, outer->data + outer->size, data);
        return createOuter(outer->size, data);
    }
}

template <typename T>
void Node<T>::array(T* begin) const {
    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;
        
        array(inner->left, begin);
        array(inner->right, begin + inner->size);
    } else {
        Outer<T>* outer = (Outer<T>*) this;
        std::copy(outer->data, outer->data + outer->size, begin);
    }
}

template <typename T>
T& Node<T>::at(size_t index) const {
    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;

        return index < inner->size
            ? at(inner->left, index)
            : at(inner->right, index - inner->size);
    } else {
        Outer<T>* outer = (Outer<T>*) this;
        return outer->data[index];
    }
}

template <typename T>
size_t Node<T>::total() const {
    size_t total = size;

    if (inner) {
        Inner<T>* inner = (Inner<T>*) this;
        total += size(inner->right);
    }
    
    return total;
}

} // namespace Rope