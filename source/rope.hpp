#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <utility>
#include "tree.hpp"

/// The util namespace
namespace Util {

/// The rope data structure
template <typename TData>
class Rope {
    /// The base struct for inner and outer nodes
    struct Node {
        /// True if it is an inner node
        bool inner;
        /// The size of the node
        size_t size;
    };

    /// The inner node
    struct Inner : public Node {
        /// The left child of the inner node
        Node* left;
        /// The right child of the inner node
        Node* right;
    };

    /// The outer node
    struct Outer : public Node {
        /// The data of the node
        TData* data;
    };

    /// The iterator for the rope
    template <typename TOther>
    class Iterator {
        /// The stack of nodes
        std::stack<Node*> nodes;

        /// The index of the current data
        size_t index;

    public:
        /// Constructs an end iterator
        Iterator();

        /// Constructs an iterator with the provided root
        Iterator(Node* root);

        /// Returns a ref to the current data
        TOther& operator*() const;

        /// Returns a pointer to the current data
        TOther* operator->() const;

        /// Increments this iterator
        Iterator<TOther>& operator++();

        /// Returns true if the two iterators are the same
        bool operator!=(const Iterator<TOther>& other) const;
    };

    typedef Iterator<TData> Iter;
    typedef Iterator<const TData> ConstIter;

private:
    /// The root node
    Node* root;

    /// Constructs a rope with the provided root
    Rope(Node* root);

    /// Constructs a rope with the provided data
    Rope(size_t size, TData* data);

public:
    /// Constructs an empty rope
    static Rope<TData> empty();

    /// Constructs a rope by copying the provided data
    static Rope<TData> copy(size_t size, TData* data);

    /// Constructs a rope by moving the provided data
    /// The data must be heap allocated
    static Rope<TData> move(size_t size, TData* data);

    /// The destructor
    ~Rope();

    /// The copy constructor
    Rope(const Rope<TData>& other);

    /// The move constructor
    Rope(Rope<TData>&& other);

    /// The copy assignment operator
    Rope<TData>& operator=(const Rope<TData>& other);

    /// The move assignment operator
    Rope<TData>& operator=(Rope<TData>&& other);

    /// The index operator
    TData& operator[](size_t index) const;

    /// Appends the provided rope
    /// The provided rope is cleared durring the process
    void append(Rope<TData>&& other);

    /// Inserts the provided rope at the provided index
    /// The provided rope is cleared durring the process
    void insert(size_t index, Rope<TData>&& other);

    /// Erases the data between the provided begin and end indices
    void erase(size_t begin, size_t end);

    /// Clears the rope
    void clear();

    /// Splits the rope at the provided index
    /// This rope is cleared durring the process
    std::pair<Rope<TData>, Rope<TData>> split(size_t index);

    /// Creates an array containing the entire data of the rope
    TData* array() const;

    /// Returns the data at the specified index
    TData& at(size_t index) const;

    /// Returns the size of the rope
    size_t size() const;

    /// Returns the begin iterator
    Iter begin();

    /// Returns the end iterator
    Iter end();

    /// Returns the const begin iterator
    ConstIter begin() const;

    /// Returns the const end iterator
    ConstIter end() const;

private:
    static Node* createEmpty();

    static Inner* createInner(Node* left, Node* right);

    static Outer* createOuter(size_t size, TData* data);

    static Node* copy(Node* node);

    static void clear(Node* node);

    static std::pair<Node*, Node*> split(Node* node, size_t index);

    static void array(Node* node, TData* begin);

    static TData& at(Node* node, size_t index);

    static size_t size(Node* node);

    template <typename TOther>
    friend std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope);
};

template <typename TData>
Rope<TData>::Rope(Node* root) 
    : root(root)
{
    // empty
}

template <typename TData>
Rope<TData>::Rope(size_t size, TData *data)
    : root(createOuter(size, data))
{
    // empty
}

template <typename TData>
Rope<TData> Rope<TData>::empty() {
    return Rope<TData>(0, nullptr);
}

template <typename TData>
Rope<TData> Rope<TData>::copy(size_t size, TData* data) {
    TData* copy = new TData[size];
    std::copy(data, data + size, copy);

    return move(size, copy);
}

template <typename TData>
Rope<TData> Rope<TData>::move(size_t size, TData* data) {
    return Rope<TData>(size, data);
}

template <typename TData>
Rope<TData>::~Rope() {
    clear();
}

template <typename TData>
Rope<TData>::Rope(const Rope<TData>& other) 
    : root(copy(other.root))
{
    // empty
}

template <typename TData>
Rope<TData>::Rope(Rope<TData>&& other) 
    : root(other.root)
{
    other.root = createEmpty();
}

template <typename TData>
Rope<TData>& Rope<TData>::operator=(const Rope<TData>& other) {
    clear();
    root = copy(other.root);

    return *this;
}

template <typename TData>
Rope<TData>& Rope<TData>::operator=(Rope<TData>&& other) {
    clear();

    root = other.root;
    other.root = createEmpty();

    return *this;
}

template <typename TData>
TData& Rope<TData>::operator[](size_t index) const {
    return at(index);
}

template <typename TData>
void Rope<TData>::append(Rope<TData>&& other) {
    if (other.size() > 0) {
        root = size() > 0
            ? createInner(root, other.root)
            : other.root;
        
        other.root = createEmpty();
    }
}

template <typename TData>
void Rope<TData>::insert(size_t index, Rope<TData>&& other) {
    auto [left, right] = split(index);

    left.append(other);
    left.append(right);

    std::swap(root, left.root);
}

template <typename TData>
void Rope<TData>::erase(size_t begin, size_t end) {
    Rope<TData> left;
    Rope<TData> center;
    Rope<TData> right;

    std::tie(left, center) = split(begin);
    std::tie(center, right) = center.split(end);
    
    left.append(right);
    std::swap(root, left.root);
}

template <typename TData>
void Rope<TData>::clear() {
    clear(root);
    root = createEmpty();
}

template <typename TData>
std::pair<Rope<TData>, Rope<TData>> Rope<TData>::split(size_t index) {
    auto [left, right] = split(root, index);
    root = createEmpty();

    return std::make_pair(Rope<TData>(left), Rope<TData>(right));
}

template <typename TData>
TData* Rope<TData>::array() const {
    TData* data = new TData[size()];
    array(root, data);

    return data;
}

template <typename TData>
TData& Rope<TData>::at(size_t index) const {
    return at(root, index);
}

template <typename TData>
size_t Rope<TData>::size() const {
    return size(root);
}

template <typename TData>
Rope<TData>::Iter Rope<TData>::begin() {
    return Iter(root);
}

template <typename TData>
Rope<TData>::Iter Rope<TData>::end() {
    return Iter();
}

template <typename TData>
Rope<TData>::ConstIter Rope<TData>::begin() const {
    return ConstIter(root);
}

template <typename TData>
Rope<TData>::ConstIter Rope<TData>::end() const {
    return ConstIter();
}

template <typename TData>
Rope<TData>::Node* Rope<TData>::createEmpty() {
    return createOuter(0, nullptr);
}

template <typename TData>
Rope<TData>::Inner* Rope<TData>::createInner(Node* left, Node* right)
{
    Inner* inner = new Inner();

    inner->inner = true;
    inner->size = size(left);
    inner->left = left;
    inner->right = right;

    return inner;
}

template <typename TData>
Rope<TData>::Outer* Rope<TData>::createOuter(size_t size, TData* data) {
    Outer* outer = new Outer();

    outer->inner = false;
    outer->size = size;
    outer->data = data;

    return outer;
}

template <typename TData>
Rope<TData>::Node *Rope<TData>::copy(Node *node)
{
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        Node* left = copy(inner->left);
        Node* right = copy(inner->right);

        return createInner(left, inner);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        return createOuter(outer->size, outer->data);
    }
}

template <typename TData>
void Rope<TData>::clear(Node* node) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);

        clear(inner->left);
        clear(inner->right);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        delete[] outer->data;
    }

    delete node;
}

template <typename TData>
std::pair<typename Rope<TData>::Node*, typename Rope<TData>::Node*> Rope<TData>::split(Node* node, size_t index) {
    Node* left;
    Node* right;
    
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);

        if (index < inner->size) {
            std::tie(left, right) = split(inner->left, index);

            inner->left = right;
            right = inner;
        } else if (index > inner->size) {
            std::tie(left, right) = split(inner->right, inner->size - index);

            inner->right = left;
            left = inner;
        } else {
            left = inner->left;
            right = inner->right;

            delete inner;
        }
    } else {
        Outer* outer = static_cast<Outer*>(node);

        size_t leftSize = index;
        size_t rightSize = outer->size - index;

        TData* leftData = new TData[leftSize];
        TData* rightData = new TData[rightSize];

        std::copy(outer->data, outer->data + leftSize, leftData);
        std::copy(outer->data + leftSize, outer->data + outer->size, rightData);

        left = createOuter(leftSize, leftData);
        right = createOuter(rightSize, rightData);

        delete[] outer->data;
        delete outer;
    }

    return std::make_pair(left, right);
}

template <typename TData>
void Rope<TData>::array(Node* node, TData* begin) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);

        array(inner->left, begin);
        array(inner->right, begin + inner->size);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        std::copy(outer->data, outer->data + outer->size, begin);
    }
}

template <typename TData>
TData& Rope<TData>::at(Node* node, size_t index) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        
        return index < inner->size
            ? at(node->left, index)
            : at(node->right, index - inner->size);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        return outer->data[index];
    }
}

template <typename TData>
size_t Rope<TData>::size(Node* node) {
    size_t total = node->size;

    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        total += size(inner->right);
    }

    return total;
}

template <typename TOther>
std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope) {
    for (const TOther& data : rope) {
        os << data;
    }

    return os;
}

template <typename TData>
template <typename TOther>
Rope<TData>::Iterator<TOther>::Iterator() 
    : nodes()
    , index(0)
{
    // empty
}

template <typename TData>
template <typename TOther>
Rope<TData>::Iterator<TOther>::Iterator(Node *root)
    : nodes()
    , index(0)
{
    if (size(root) > 0) {
        do {
            nodes.push(root);
        } while (root->inner && (root = static_cast<Inner*>(root)->left));
    }
}

template <typename TData>
template <typename TOther>
TOther& Rope<TData>::Iterator<TOther>::operator*() const {
    Outer* outer = static_cast<Outer*>(nodes.top());
    return outer->data[index];
}

template <typename TData>
template <typename TOther>
TOther* Rope<TData>::Iterator<TOther>::operator->() const {
    return &(this->operator*());
}

template <typename TData>
template <typename TOther>
Rope<TData>::Iterator<TOther>& Rope<TData>::Iterator<TOther>::operator++() {
    Node* node = nodes.top();
    index += 1;

    if (index == node->size) {
        Inner* inner;
        index = 0;

        do {
            nodes.pop();

            if (nodes.empty()) {
                return *this;
            }

            inner = static_cast<Inner*>(nodes.top());
        } while (node == inner->right && (node = inner));

        node = inner->right;

        while (node->inner) {
            inner = static_cast<Inner*>(node);
            node = inner->left;
        }
    }

    return *this;
}

template <typename TData>
template <typename TOther>
bool Rope<TData>::Iterator<TOther>::operator!=(const Iterator<TOther>& other) const {
    Node* node = nodes.size() > 0
        ? nodes.top()
        : nullptr;
    Node* otherNode = other.nodes.size() > 0
        ? other.nodes.top()
        : nullptr;

    return node != otherNode || index != other.index;
}

} // namespace Util