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

} // namespace Rope