#include "rope.hpp"

namespace Rope {

template <typename T>
Rope<T>::Rope() 
    : size(0)
    , root(nullptr)
{
    // empty
}

template <typename T>
Rope<T>::Rope(size_t size, T* data) 
    : size(size)
    , root(new Outer{size, data})
{
    // empty
}

template <typename T>
Rope<T>::~Rope() {

}

template <typename T>
size_t Rope<T>::size(Node<T>* node) const {
    return node->size & SIZE_MASK;
}

template <typename T>
bool Rope<T>::isInner(Node<T>* node) const {
    return node->size & SIZE_MASK;
}

} // namespace Rope