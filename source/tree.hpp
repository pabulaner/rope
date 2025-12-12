#include <stddef.h>

namespace Rope {

template <typename TData, size_t TDataSize = 1024>
class Rope {
    typedef Rope<TData, TDataSize> TRope;

    constexpr size_t MinSize = TDataSize >> 2;
    constexpr size_t MaxSize = TDataSize;

    struct Node {
        bool inner;

        size_t size;
        uint8_t height;
    };

    struct Inner : Node {
        Node* left;
        Node* right;
    };

    struct Outer : Node {
        TData* data;
    };

    Node* root;

public:
    Rope();

    Rope(size_t size, TData* data);

    ~Rope();

    Rope(const TRope& other);

    Rope(TRope&& other);

    TRope& operator=(const TRope& other);

    TRope& operator=(TRope&& other);

    TData& operator[](size_t index);

    TData& at(size_t index);

    TData* array();

    void append(TRope&& other);

    void insert(TRope&& other, size_t index);

    void remove(size_t begin, size_t end);

    std::pair<TRope, TRope> split(size_t index);

    size_t size();

private:
    static Inner* createInner(Node* left, Node* right);

    static Outer* createOuter(size_t size, TData* data);

    static Outer* createEmpty();

    static Node* copy(Node* node);

    static void destroy(Node* node);

    static void update(Inner* node);

    static Node* combine(Outer* left, Outer* right);

    static Node* rotateLeft(Inner* inner);

    static Node* rotateRight(Inner* inner);

    static std::pair<Inner*, Outer*> leftmost(Node* node);

    static std::pair<Inner*, Outer*> rightmost(Node* node);

    static TData& at(Node* node, size_t index);

    static void array(Node* node, TData* array);

    static Node* append(Node* left, Node* right);

    static size_t size(Node* node);
};

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Rope() 
    : root(createEmpty())
{
    // empty
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Rope(size_t size, TData* data) 
    : root(createOuter(size, data))
{
    // empty
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::~Rope() {
    destroy(root);
}

template <typename TData, size_t TDataSize>
TData& Rope<TData, TDataSize>::operator[](size_t index) {
    return at(index);
}

template <typename TData, size_t TDataSize>
TData& Rope<TData, TDataSize>::at(size_t index) {
    return at(root, index);
}

template <typename TData, size_t TDataSize>
TData *Rope<TData, TDataSize>::array() {
    size_t size = size();
    TData* result = new TData[size];

    array(root, result);
    return result;
}

template <typename TData, size_t TDataSize>
void Rope<TData, TDataSize>::append(TRope&& other) {
    if (other.size() > 0) {
        root = size() > 0
            ? append(root, other.root)
            : other.root;

        other.root = createEmpty();
    }
}

template <typename TData, size_t TDataSize>
size_t Rope<TData, TDataSize>::size() {
    return size(root);
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Inner* Rope<TData, TDataSize>::createInner(Node* left, Node* right) {
    Inner* inner = new Inner();

    inner->inner = true;
    inner->size = size(left);
    inner->left = left;
    inner->right = right;

    return inner;
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Outer* Rope<TData, TDataSize>::createOuter(size_t size, TData* data) {
    Outer* outer = new Outer();

    outer->inner = false;
    outer->size = size;
    outer->data = new TData[MaxSize];

    std::copy(data, data + size, outer->data);
    return outer;
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Node* Rope<TData, TDataSize>::copy(Node* node) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        Node* left = copy(inner->left);
        Node* right = copy(inner->right);

        return createInner(left, right);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        return createOuter(outer->size, outer->data);
    }
}

template <typename TData, size_t TDataSize>
void Rope<TData, TDataSize>::destroy(Node* node) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        
        destroy(inner->left);
        destroy(inner->right);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        delete outer->data;
    }

    delete node;
}

template <typename TData, size_t TDataSize>
void Rope<TData, TDataSize>::update(Inner *inner) {
    inner->size = inner->left->size + inner->right->size;
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Node* Rope<TData, TDataSize>::combine(Outer* left, Outer* right) {
    size_t total = left->size + right->size;

    if (total <= MaxSize) {
        std::copy(right->data, right->data + right->size, left->data + left->size);
        destroy(right);

        return right;
    }

    if (left->size < MinSize) {
        size_t delta = MinSize - left->size;

        std::copy(right->data, right->data + delta, left->data + left->size);
        std::copy(right->data + delta, right->data + right->size, right->data);

        left->size = MinSize;
        right->size -= delta;
    } else if (right->size < MinSize) {
        size_t delta = MinSize - right->size;

        std::copy(right->data, right->data + right->size, right->data + delta);
        std::copy(left->data + left->size - delta, left->data + left->size, right->data);

        right->size = MinSize;
        left->size -= delta;
    }

    return createInner(left, right);
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Node* Rope<TData, TDataSize>::rotateLeft(Inner* inner) {
    Inner* pivot = static_cast<Inner*>(inner->right);
    Node* tmp = pivot->left;

    inner->right = tmp;
    pivot->left = inner;

    update(inner);
    update(pivot);

    return pivot;
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Node* Rope<TData, TDataSize>::rotateRight(Inner* inner) {
    Inner* pivot = static_cast<Inner*>(inner->left);
    Node* tmp = pivot->right;

    inner->left = tmp;
    pivot->right = inner;

    update(inner);
    update(pivot);

    return pivot;
}

template <typename TData, size_t TDataSize>
std::pair<typename Rope<TData, TDataSize>::Inner*, typename Rope<TData, TDataSize>::Outer*> Rope<TData, TDataSize>::leftmost(Node* node)
{
    Inner* inner = nullptr;

    while (node->inner) {
        inner = static_cast<Inner*>(node);
        node = inner->left;
    }

    Outer* outer = static_cast<Outer*>(node);
    return {inner, outer};
}

template <typename TData, size_t TDataSize>
std::pair<typename Rope<TData, TDataSize>::Inner*, typename Rope<TData, TDataSize>::Outer*> Rope<TData, TDataSize>::rightmost(Node* node)
{
    Inner* inner = nullptr;

    while (node->inner) {
        inner = static_cast<Inner*>(node);
        node = inner->right;
    }

    Outer* outer = static_cast<Outer*>(node);
    return {inner, outer};
}

template <typename TData, size_t TDataSize>
TData& Rope<TData, TDataSize>::at(Node* node, size_t index) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        return index < inner->size
            ? at(inner->left, index)
            : at(inner->right, index - inner->size);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        return outer->data[index];
    }
}

template <typename TData, size_t TDataSize>
void Rope<TData, TDataSize>::array(Node* node, TData* array) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);

        array(inner->left, array);
        array(inner->right, array + inner->size);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        std::copy(outer->data, outer->data + outer->size, array);
    }
}

template <typename TData, size_t TDataSize>
Rope<TData, TDataSize>::Node* Rope<TData, TDataSize>::append(Node* left, Node* right) {
    if (left->inner && right->inner) {
        Inner* inner = createInner(left, right);
        return inner;
    }

    if (left->inner) {
        Outer* outer = static_cast<Outer*>(right);
        auto [parent, right] = rightmost(left);
        size_t total = outer->size + right->size;
        
        TData* outerBegin = outer->data;
        TData* outerEnd = outerBegin + outer->size;
        TData* rightBegin = right->data;
        TData* rightEnd = rightBegin + right->size;

        if (total <= MaxSize) {
            std::copy(outerBegin, outerEnd, rightEnd);
            right->size = total;

            destroy(outer);
        } else {
            size_t size = MinSize - outer->size;

            std::copy(outerBegin, outerEnd, outerBegin + size);
            std::copy(rightEnd - size, rightEnd, outerBegin);

            outer->size = MinSize;
            right->size -= size;
        }

        Node* inner = createInner(right, outer);
        parent->right = inner;

        return left;
    }

    if (right->inner) {
        Outer* outer = static_cast<Outer*>(left);
        auto [parent, left] = leftmost(right);
        size_t total = outer->size + left->size;
        
        TData* outerBegin = outer->data;
        TData* outerEnd = outerBegin + outer->size;
        TData* leftBegin = left->data;
        TData* leftEnd = leftBegin + left->size;

        if (total <= MaxSize) {
            std::copy(leftBegin, leftEnd, leftBegin + outer->size);
            std::copy(outerBegin, outerEnd, leftBegin);
            
            left->size = total;
            destroy(outer);
        } else {
            size_t size = MinSize - outer->size;

            std::copy(leftBegin, leftEnd - size, outerEnd);
            std::copy(leftBegin + size, leftEnd, leftBegin);

            outer->size = MinSize;
            right->size -= size;
        }

        Node* inner = createInner(left, outer);
        parent->left = inner;

        return right;
    }

    return nullptr;
}

template <typename TData, size_t TDataSize>
size_t Rope<TData, TDataSize>::size(Node* node) {
    if (node->inner) {
        Inner* inner = static_cast<Inner*>(node);
        return inner->size + size(inner->right);
    } else {
        Outer* outer = static_cast<Outer*>(node);
        return outer->size;
    }
}

} // namespace Rope