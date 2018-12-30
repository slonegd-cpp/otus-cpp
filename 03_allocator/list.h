#pragma once

#include <memory>


template<class T, class Allocator = std::allocator<T>> 
class list
{
public:
    using value_type = T;

    struct Node {
        T value;
        Node* next;
        Node(T value, Node* next = nullptr) : value(value), next(next) {}
        Node() = default;
    };

    struct iterator : std::iterator<std::forward_iterator_tag, T> {
        Node* p;

        iterator()                       : p {nullptr} {}
        iterator (const iterator& other) : p {other.p} {}
        iterator (Node* other)           : p {other}   {}

        iterator& operator= (const iterator& other)
        {
            p = other.p;
            return *this;
        }

        T& operator*() const
        {
            return p->value;
        }

        T* operator->() const
        {
            return &(p->value);
        }

        bool operator==(const iterator &other) const
        {
            return p == other.p;
        }

        bool operator!=(const iterator &other) const
        {
            return p != other.p;
        }

        iterator& operator++()
        {
            p = p->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp {*this};
            p = p->next;
            return tmp;   
        }
    };


    list() : first(nullptr), last(nullptr) { }


    void push (T arg)
    {
        auto p = alloc.allocate(1);
        if (first != nullptr)
            last = last->next = p;
        else
            last = first = p;
        alloc.construct(last, arg);
    }


    iterator begin() noexcept { return iterator {first};      }
    iterator end()   noexcept { return iterator {last->next}; }


    ~list()
    {
        auto it = first;
        while (it != nullptr) {
            auto tmp = it;
            it = it->next;
            alloc.destroy(tmp);
            alloc.deallocate(tmp, 1);
        }          
    }



private:
    Node* first;
    Node* last;
    typename Allocator::template rebind<Node>::other alloc;
};

