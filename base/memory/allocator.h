#ifndef ENGINEDEV_ALLOCATOR_H
#define ENGINEDEV_ALLOCATOR_H

// #include <stdlib.h>

// typedef struct allocator
// {
//     void* internal;
//     void* (realloc)(struct allocator* a, void* ptr, size_t new_size, size_t align);
// } allocator;

// struct allocator_api
// {
//     allocator* system;
// }

template <typename T>
struct allocator
{
    using value_type = T;
    allocator() noexcept;
    template <class U> custom_allocator(const custom_allocator<U>&) noexcept;
    T* allocate(size_t n);
    void deallocate(T* p, size_t n);
};

template <class T, class U>
constexpr bool operator== (const T&, const U&) noexcept;

template <class T, class U>
constexpr bool operator!= (const T&, const U&) noexcept;


#endif