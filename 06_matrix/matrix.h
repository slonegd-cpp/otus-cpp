#pragma once

#include <list>
#include <tuple>
#include <algorithm>
#include <utility>
#include <cstring>
#include <memory>
		

/// добавляет к tuple (T) тип U
template<typename T, typename U>
using AppendToTuple = decltype(std::tuple_cat(T{}, std::make_tuple(U{})));

/// генерирует тип tuple, состоящий из n int
template<int n>
struct GenTupleOfInt {
   using type = AppendToTuple<typename GenTupleOfInt<n-1>::type, int>;
};
template<>
struct GenTupleOfInt<1> {
   using type = std::tuple<int>;
};

/// тип для хранения координат элемента матрицы
template<int n>
using Coords = typename GenTupleOfInt<n>::type;

/// тип для хранения одного элемента матрицы с координатами
template<class T, int n>
using Element = AppendToTuple<Coords<n>,T>;

/// тип списка всех элементов матрицы
template<class T, int n>
using Elements = std::list<Element<T,n>>;

/// сравнивает первые n элементов у двух tuple
template<int n, class T, class U,  int index = n-1>
struct partialCompareTuple_ {
   bool operator() (const T& t1, const U& t2)
   {
      return std::get<index>(t1) == std::get<index>(t2)
        and partialCompareTuple_<n,T,U,index-1>()(t1,t2);
   }
};
template<int n, class T, class U>
struct partialCompareTuple_<n,T,U,-1> {
   bool operator() (const T& t1, const U& t2)
   {
      return true;
   }
};
template<int n, class T, class U>
bool partialCompareTuple (const T& t1, const U& t2)
{
   return partialCompareTuple_<n,T,U,n-1>()(t1,t2);
}



template<class T, int free_, int n = 2>
class Matrix
{
public:
   Matrix() : pointer (std::make_shared<Elements<T,n>>()) { }


   /// структура, которая "знает" с каким элементом матрицы надо работать
   template<int index>
   struct ProxyElement
   {
      /// указатель на список элементов матрицы
      std::weak_ptr<Elements<T,n>> elements;
      /// координаты элемента матрицы
      Coords<n> x;

      ProxyElement<index> operator = (T v) const
      {
         static_assert (index == n, "неполная размерность");
         auto sameX = [&](const auto& element) {
            return partialCompareTuple<n>(x, element);
         };
         auto p = elements.lock();
         if (p) {
            if (v != free_) {
               auto it = std::find_if (
                  p->begin(),
                  p->end(),
                  sameX
               );
               if (it == p->end())
                  p->push_back(std::tuple_cat(x,std::make_tuple(v)));
               else
                  std::get<n>(*it) = v;
            } else {
               p->remove_if (sameX);
            }
         } else {
            throw std::bad_weak_ptr();
         }
         return *this;
      }


      ProxyElement<index+1> operator [] (int xnext)
      {
         static_assert (index < n, "выход за пределы размеров матрицы");
         std::get<index>(x) = xnext;
         return *reinterpret_cast<ProxyElement<index+1>*>(this);
      }


      operator T() const
      {
         static_assert (index == n, "неполная размерность");
         T res = free_;
         auto p = elements.lock();
         if (p) 
            for (const auto& element : *p) {
               if (partialCompareTuple<n>(x, element))
                  return std::get<n>(element);
            }
         else
            throw std::bad_weak_ptr();
         return res;
      }
   };


   /// передача первой координаты элемента
   /// чтобы можно было использовать [][]
   ProxyElement<1> operator [] (int x)
   {
      auto element = ProxyElement<1>();
      element.elements = pointer;
      std::get<0>(element.x) = x;
      return element;
   }

   auto size()  { return pointer->size();  }
   auto begin() { return pointer->begin(); }
   auto end()   { return pointer->end();   }
   template<class Compare>
   void sort (Compare comp) { pointer->sort(comp); }


private:
   std::shared_ptr<Elements<T,n>> pointer;

};



