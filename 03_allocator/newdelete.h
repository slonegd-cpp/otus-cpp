#pragma once

#include <stdint.h>
#include <cstddef>
#include <new>

namespace my {

    extern std::size_t alloc_counter;
    void* malloc(std::size_t size) throw (std::bad_alloc);
    void free(void* p) noexcept;

} // namespace my

extern "C++" {

    inline void* operator new(std::size_t size) _GLIBCXX_THROW (std::bad_alloc)
    {
        return my::malloc(size);
    }

    inline void operator delete(void* p) noexcept
    {
        my::free(p);
    }

    inline void* operator new[](std::size_t size) _GLIBCXX_THROW (std::bad_alloc)
    {
        return my::malloc(size);
    }

    inline void operator delete[](void* p) throw()
    {
        my::free(p);
    }

    inline void* operator new(std::size_t size, const std::nothrow_t&) noexcept
    {
        return my::malloc(size);
    }

    inline void operator delete(void* p, const std::nothrow_t&) noexcept
    {
        my::free(p);
    }

    inline void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
    {
        return my::malloc(size);
    }

    inline void operator delete[](void* p, const std::nothrow_t&) noexcept
    {
        my::free(p);
    }

    inline void operator delete(void* p, long unsigned int)
    {
        my::free(p);
    }

    inline void operator delete [](void* p, long unsigned int)
    {
        my::free(p);
    }

} // extern "C++"
