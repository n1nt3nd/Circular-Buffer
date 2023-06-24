#pragma once

#include "../CCircularBuffer/CCircularBuffer.h"

template<typename T, typename Alloc = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T, Alloc> {
 public:
  typedef typename CCircularBuffer<T, Alloc>::value_type value_type;
  typedef typename CCircularBuffer<T, Alloc>::alloc_traits alloc_traits;

  using CCircularBuffer<T, Alloc>::MaxSize;
  using CCircularBuffer<T, Alloc>::begin;
  using CCircularBuffer<T, Alloc>::end;
  using CCircularBuffer<T, Alloc>::cbegin;
  using CCircularBuffer<T, Alloc>::cend;
  using CCircularBuffer<T, Alloc>::Front;
  using CCircularBuffer<T, Alloc>::Back;
  using CCircularBuffer<T, Alloc>::Empty;
  using CCircularBuffer<T, Alloc>::Size;
  using CCircularBuffer<T, Alloc>::swap;
  using CCircularBuffer<T, Alloc>::Full;
  using CCircularBuffer<T, Alloc>::Capacity;
  using CCircularBuffer<T, Alloc>::CCircularBuffer;
  using CCircularBuffer<T, Alloc>::Reserve;
  using CCircularBuffer<T, Alloc>::Inc;
  using CCircularBuffer<T, Alloc>::Dec;

  void PushBack(const value_type& item) {
    if (Full()) {
      if (Empty())
        Reserve(1);
      else Reserve(Capacity() * 2);
    }

    alloc_traits::construct(this->allocator_, std::to_address(this->last_), item);
    Inc(this->last_);
    ++this->size_;
  }

  void PushFront(const value_type& item) {
    if (Full()) {
      if (Empty())
        Reserve(1);
      else Reserve(Capacity() * 2);
    }

    Dec(this->first_);
    alloc_traits::construct(this->allocator_, std::to_address(this->first_), item);
    ++this->size_;
  }

};

template<typename T, typename Alloc>
void swap(CCircularBufferExt<T, Alloc>& lhs, CCircularBufferExt<T, Alloc>& rhs) {
  lhs.swap(rhs);
}
