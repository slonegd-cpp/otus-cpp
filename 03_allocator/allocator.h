/**
 * класс аллокатора, который должен проходить тесты
 * из файла allocator_test.cpp
 * 
 * b. (-) Вы попали в ловушку!
 * Использование в вашем аллокаторе std::vector<T> allocated;
 * будет вызывать для T лишний конструктор, что не верно.
 * Нам нужно выделить память. 
 * d. (-) Попробуйте уйти от использования голых указателей в вашем контейнере.
 * f.(!) Попробуйте доработать ваш аллокатор до полноценного,
 * который бы работал и с std::vector,
 * и который бы поддерживал полноценную работу с std::map,
 * т.е. операции вставки и удаления и т.д.
 */

#pragma once

#include <memory>
#include <vector>
#include <list>

template<class T, size_t reserve>
class MyAllocator
{
public:
    using value_type = T;

    template<class U>
    struct rebind { using other = MyAllocator<U, reserve>; };


    MyAllocator() : index(0) { }


    T* allocate (size_t n)
    {
        size_t memoryIndex = index % reserve;
        index++;
        if (memoryIndex == 0) {
            if (head != nullptr)
                head->tail = std::move(head);
            head = std::make_unique<Pool>();
        }

        return reinterpret_cast<T*>(&(head->memory[memoryIndex]));
    }


    void deallocate (T* p, size_t n)
    {
    }



    // allocator_traits
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;


    template<class U, class ... Types>
    void construct (U* p, Types&& ... args)
    {
        new(p) U (std::forward<Types>(args) ... );
    }


    void destroy (T* p)
    {
        p->~T();
    }


private:
    struct Pool {
        std::array<std::array<char,sizeof(T)>, reserve> memory;
        std::unique_ptr<Pool> tail;
    };
    std::unique_ptr<Pool> head;
    size_t index;
};