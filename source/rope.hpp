#pragma once

#include <iostream>
#include <memory>
#include <utility>

namespace Util {

template <typename TData>
class Rope {
private:
    struct Node {
        bool inner;
        size_t size;
    };

    struct Inner : public Node {
        Node* left;
        Node* right;
    };

    struct Outer : public Node {
        TData* data;
    };

    Node* root;

public:
    Rope();

    Rope(size_t size, TData* data);

    ~Rope();

    Rope(const Rope<TData>& other);

    Rope(Rope<TData>&& other);

    Rope<TData>& operator=(const Rope<TData>& other);

    Rope<TData>& operator=(Rope<TData>&& other);

    TData& operator[](size_t index) const;

    void append(Rope<TData>&& other);

    void insert(size_t index, Rope<TData>&& other);

    void erase(size_t begin, size_t end);

    void clear();

    std::pair<Rope<TData>, Rope<TData>> split(size_t index);

    TData* array() const;

    TData& at(size_t index) const;

    size_t size() const;

private:
    static Inner* createInner(Node* left, Node* right);

    static Outer* createOuter(size_t size, TData* data);

    static Outer* createEmpty();

    static Node* copy(Node* node);

    static void clear(Node* node);

    static void array(Node* node, TData* begin);

    static TData& at(Node* node, size_t index);

    static size_t size(Node* node);

    template <typename TOther>
    friend std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope);
};

template <typename TData>
Rope<TData>::Rope() 
    : root(createEmpty())
{
    // empty
}

template <typename TData>
Rope<TData>::Rope(size_t size, TData* data) 
    : root(createOuter(size, data))
{
    // empty
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
    other.root = nullptr;
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
    other.root = nullptr;

    return *this;
}

template <typename TData>
TData& Rope<TData>::operator[](size_t index) const {
    return at(index);
}

template <typename TData>
void Rope<TData>::append(Rope<TData>&& other) {
    root = createInner(root, other.root);
}

template <typename TData>
void Rope<TData>::insert(size_t index, Rope<TData>&& other) {
}

template <typename TData>
void Rope<TData>::erase(size_t begin, size_t end) {
}

template <typename TData>
void Rope<TData>::clear() {
    clear(root);
    root = createEmpty();
}

template <typename TData>
std::pair<Rope<TData>, Rope<TData>> Rope<TData>::split(size_t index) {
    return std::pair<Rope<TData>, Rope<TData>>();
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
Rope<TData>::Inner *Rope<TData>::createInner(Node *left, Node *right) {
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
Rope<TData>::Outer* Rope<TData>::createEmpty() {
    return createOuter(0, nullptr);
}

template <typename TData>
Rope<TData>::Node* Rope<TData>::copy(Node* node) {
    if (node->inner) {
        Inner* inner = (Inner*) node;
        Node* left = copy(inner->left);
        Node* right = copy(inner->right);

        return createInner(left, inner);
    } else {
        Outer* outer = (Outer*) node;
        return createOuter(outer->size, outer->data);
    }
}

template <typename TData>
void Rope<TData>::clear(Node* node) {
    if (node->inner) {
        Inner* inner = (Inner*) node;

        clear(inner->left);
        clear(inner->right);
    } else {
        Outer* outer = (Outer*) node;
        delete outer->data;
    }

    delete node;
}

template <typename TData>
void Rope<TData>::array(Node* node, TData* begin) {
    if (node->inner) {
        Inner* inner = (Inner*) node;

        array(inner->left, begin);
        array(inner->right, begin + inner->size);
    } else {
        Outer* outer = (Outer*) node;
        std::copy(outer->data, outer->data + outer->size, begin);
    }
}

template <typename TData>
TData& Rope<TData>::at(Node* node, size_t index) {
    if (node->inner) {
        Inner* inner = (Inner*) node;
        
        return index < inner->size
            ? at(node->left, index)
            : at(node->right, index - inner->size);
    } else {
        Outer* outer = (Outer*) node;
        return outer->data[index];
    }
}

template <typename TData>
size_t Rope<TData>::size(Node* node) {
    size_t total = node->size;

    if (node->inner) {
        Inner* inner = (Inner*) node;
        total += size(inner->right);
    }

    return total;
}

template <typename TOther>
std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope) {
    size_t size = rope.size();
    TOther* data = rope.array();

    for (size_t i = 0; i < size; i++) {
        os << data[i];
    }

    return os;
}

} // namespace Util