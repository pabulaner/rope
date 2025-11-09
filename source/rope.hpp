#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <utility>

/// The util namespace
namespace Util {

/// The rope data structure
template <typename TData>
class Rope {
    /// Declaration of the node struct
    struct Node;
    /// Declaration of the inner struct
    struct Inner;
    /// Declaration of the outer struct
    struct Outer;

    /// Typedef of the data ptr
    typedef std::unique_ptr<TData> DataPtr;
    /// Typedef of the node ptr
    typedef std::unique_ptr<Node> NodePtr;
    /// Typedef of the inner ptr
    typedef std::unique_ptr<Inner> InnerPtr;
    /// Typedef of the outer ptr
    typedef std::unique_ptr<Outer> OuterPtr;

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
        NodePtr left;
        /// The right child of the inner node
        NodePtr right;
    };

    /// The outer node
    struct Outer : public Node {
        /// The data of the node
        DataPtr data;
    };

public:
    /// The iterator for iterating other the data inside a rope
    class Iterator {
        /// The stack holding the nodes
        std::stack<Node*> nodes;
        /// The current index into the data
        size_t index;

    public:
        /// The onstructor for an end iterator
        Iterator();

        /// The constructor for a begin iterator
        Iterator(Node* root);

        /// The deref operator
        TData& operator*() const;

        /// The increment operator
        Iterator& operator++();

        /// The not equals operator
        bool operator!=(const Iterator& other);
    };

private:
    /// The root node
    NodePtr root;

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
    void append(Rope<TData>&& other);

    /// Inserts the provided rope at the provided index
    void insert(size_t index, Rope<TData>&& other);

    /// Erases the data between the provided begin and end indices
    void erase(size_t begin, size_t end);

    /// Clears the rope
    void clear();

    /// Splits the rope at the provided index
    /// This rope is cleared durring the process
    std::pair<Rope<TData>, Rope<TData>> split(size_t index);

    /// Creates an array containing the entire data of the rope
    DataPtr array() const;

    /// Returns the data at the specified index
    TData& at(size_t index) const;

    /// Returns the size of the rope
    size_t size() const;

    /// Returns the begin iterator
    Iterator begin();

    /// Returns the end iterator
    Iterator end();

private:
    static InnerPtr createInner(NodePtr left, NodePtr right);

    static OuterPtr createOuter(size_t size, DataPtr data);

    static OuterPtr createEmpty();

    static NodePtr copy(NodePtr node);

    static void clear(NodePtr node);

    static void array(NodePtr node, DataPtr begin);

    static TData& at(NodePtr node, size_t index);

    static size_t size(NodePtr node);

    template <typename TOther>
    friend std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope);
};

template <typename TData>
Rope<TData>::Iterator::Iterator() 
    : nodes()
    , index(0)
{
    // empty
}

template <typename TData>
Rope<TData>::Iterator::Iterator(Node* node) 
    : nodes()
    , index(0) 
{
    do {
        nodes.push(node);
    } while (node->inner && (node = static_cast<Inner*>(node)->left));
}

template <typename TData>
TData& Rope<TData>::Iterator::operator*() const {
    Outer* outer = static_cast<Outer*>(nodes.top());
    return outer->data[index];
}

template <typename TData>
Rope<TData>::Iterator& Rope<TData>::Iterator::operator++() {
    Outer* outer = static_cast<Outer*>(nodes.top());

    if (index < outer->size) {
        index += 1;
    } else {
        Node* node;
        index = 0;

        do {
            node = nodes.pop();
        } while (nodes.size() > 0 && static_cast<Inner*>(nodes.top())->right == node);

        if (nodes.size() > 0) {
            Inner* inner = static_cast<Inner*>(node);
            nodes.push(inner->right);

            while ((node = nodes.top())->inner) {
                inner = static_cast<Inner*>(node);
                nodes.push(inner->left);
            }
        }
    }

    return *this;
}

template <typename TData>
bool Rope<TData>::Iterator::operator!=(const Iterator &other) {
    if (nodes.size() != other.nodes.size()) {
        return true;
    }

    if (nodes.size() > 0) {
        Node* left = nodes.top();
        Node* right = other.nodes.top();

        if (left != right) {
            return true;
        }

        return index != other.index;
    }

    return false;
}

template <typename TData>
Rope<TData> Rope<TData>::empty() {
    Rope<TData> rope;
    rope.root = createEmpty();

    return rope;
}

template <typename TData>
Rope<TData> Rope<TData>::copy(size_t size, TData* data) {
    TData* copy = new TData[size];
    std::copy(data, data + size, copy);

    return move(size, copy);
}

template <typename TData>
Rope<TData> Rope<TData>::move(size_t size, TData* data) {
    Rope<TData> rope;
    rope.root = createOuter(size, data);

    return Rope<TData>();
}

template <typename TData>
Rope<TData>::~Rope()
{
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
Rope<TData>::DataPtr Rope<TData>::array() const {
    DataPtr data = new TData[size()];
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
Rope<TData>::InnerPtr Rope<TData>::createInner(NodePtr left, NodePtr right) {
    InnerPtr inner = std::make_unique<Inner>();

    inner->inner = true;
    inner->size = size(left);
    inner->left = left;
    inner->right = right;

    return inner;
}

template <typename TData>
Rope<TData>::OuterPtr Rope<TData>::createOuter(size_t size, DataPtr data) {
    OuterPtr outer = std::make_unique<Outer>();

    outer->inner = false;
    outer->size = size;
    outer->data = std::move(data);

    return outer;
}

template <typename TData>
Rope<TData>::OuterPtr Rope<TData>::createEmpty() {
    return createOuter(0, nullptr);
}

template <typename TData>
Rope<TData>::NodePtr Rope<TData>::copy(NodePtr node) {
    if (node->inner) {
        InnerPtr inner = static_cast<InnerPtr>(node);
        NodePtr left = copy(inner->left);
        NodePtr right = copy(inner->right);

        return createInner(left, inner);
    } else {
        OuterPtr outer = static_cast<OuterPtr>(node);
        return createOuter(outer->size, outer->data);
    }
}

template <typename TData>
void Rope<TData>::clear(NodePtr node) {
    if (node->inner) {
        InnerPtr inner = static_cast<InnerPtr>(node);

        clear(inner->left);
        clear(inner->right);
    } else {
        OuterPtr outer = static_cast<OuterPtr>(node);
        delete outer->data;
    }

    delete node;
}

template <typename TData>
void Rope<TData>::array(NodePtr node, DataPtr begin) {
    if (node->inner) {
        InnerPtr inner = static_cast<InnerPtr>(node);

        array(inner->left, begin);
        array(inner->right, begin + inner->size);
    } else {
        OuterPtr outer = static_cast<OuterPtr>(node);
        std::copy(outer->data, outer->data + outer->size, begin);
    }
}

template <typename TData>
TData& Rope<TData>::at(NodePtr node, size_t index) {
    if (node->inner) {
        InnerPtr inner = static_cast<InnerPtr>(node);
        
        return index < inner->size
            ? at(node->left, index)
            : at(node->right, index - inner->size);
    } else {
        OuterPtr outer = static_cast<OuterPtr>(node);
        return outer->data[index];
    }
}

template <typename TData>
size_t Rope<TData>::size(NodePtr node) {
    size_t total = node->size;

    if (node->inner) {
        InnerPtr inner = static_cast<InnerPtr>(node);
        total += size(inner->right);
    }

    return total;
}

template <typename TOther>
std::ostream& operator<<(std::ostream& os, const Rope<TOther>& rope) {
    for (TOther& value : rope) {
        os << value;
    }

    return os;
}

} // namespace Util