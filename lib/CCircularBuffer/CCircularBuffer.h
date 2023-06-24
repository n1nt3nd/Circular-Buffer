#pragma once

#include "../CCircularBufferIter/CCircularBufferIter.h"

#include <memory>

template<typename T, typename Alloc = std::allocator<T>>
class CCircularBuffer {
 public:
  typedef typename Alloc::value_type value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef CCircularBufferIter<CCircularBuffer<T, Alloc>, nonconst_traits<Alloc>> iterator;
  typedef CCircularBufferIter<CCircularBuffer<T, Alloc>, const_traits<Alloc>> const_iterator;
  typedef Alloc allocator_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 protected:
  pointer begin_;
  pointer end_;
  pointer first_;
  pointer last_;
  size_type size_;
  Alloc allocator_;
  template<typename Container, typename Traits> friend
  class CCircularBufferIter;
  typedef __gnu_cxx::__alloc_traits<allocator_type> alloc_traits;

  template<class Pointer>
  void Inc(Pointer& p) const {
    if (++p == end_)
      p = begin_;
  }

  template<class Pointer>
  void Dec(Pointer& p) const {
    if (p == begin_)
      p = end_;
    --p;
  }

  template<class Pointer>
  Pointer Add(Pointer p, difference_type n) const {
    return p + (n < (end_ - p) ? n : n - (end_ - begin_));
  }

  template<class Pointer>
  Pointer Sub(Pointer p, difference_type n) const {
    return p - (n > (p - begin_) ? n - (end_ - begin_) : n);
  }

 public:
  iterator begin() {
    return iterator(this, Empty() ? 0 : first_);
  }

  const_iterator begin() const {
    return const_iterator(this, Empty() ? 0 : first_);
  }

  iterator end() {
    return iterator(this, 0);
  }

  const_iterator end() const {
    return const_iterator(this, 0);
  }

  const_iterator cbegin() const {
    return begin();
  }

  const_iterator cend() const {
    return end();
  }

  size_type Size() const {
    return size_;
  }

  size_type Capacity() const {
    return end_ - begin_;
  }

  bool Empty() const {
    return Size() == 0;
  }

  bool Full() const {
    return Capacity() == Size();
  }

  reference operator[](size_type index) {
    return *Add(first_, index);
  }

  const_reference operator[](size_type index) const {
    return *Add(first_, index);
  }

  reference Front() {
    return *first_;
  }

  const_reference Front() const {
    return *first_;
  }

  reference Back() {
    return *((last_ == begin_ ? end_ : last_) - 1);
  }

  const_reference Back() const {
    return *((last_ == begin_ ? end_ : last_) - 1);
  }

  size_type MaxSize() const {
    return alloc_traits::max_size(allocator_);
  }

  explicit CCircularBuffer(const allocator_type& alloc = allocator_type())
      : allocator_(alloc), begin_(0), end_(0), first_(0), last_(0), size_(0) {}

  explicit CCircularBuffer(size_type capacity, const allocator_type& alloc = allocator_type())
      : allocator_(alloc), size_(0) {
    InitializeBuffer(capacity);
    first_ = last_ = begin_;
  }

  CCircularBuffer(size_type n, const value_type& item, const allocator_type& alloc = allocator_type())
      : allocator_(alloc), size_(n) {
    InitializeBuffer(n, item);
    first_ = last_ = begin_;
  }

  CCircularBuffer(const CCircularBuffer<T, Alloc>& other) : allocator_(Alloc()), size_(other.Size()) {
    //Assign(other.begin(), other.end());
      RangeInitialize(other.begin(), other.end(), other.Capacity());
  }

  template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
  CCircularBuffer(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
      : allocator_(alloc) {
    RangeInitialize(first, last, last - first);
  }

  CCircularBuffer(const std::initializer_list<value_type>& il, const allocator_type& alloc = allocator_type())
      : allocator_(alloc) {
    RangeInitialize(il.begin(), il.end(), il.size());
  }

  virtual void PushBack(const value_type& item) {
    if (Full()) {
      if (Empty())
        return;
      *last_ = item;
      Inc(last_);
      first_ = last_;
    } else {
      alloc_traits::construct(allocator_, std::to_address(last_), item);
      Inc(last_);
      ++size_;
    }
  }

  virtual void PushFront(const value_type& item) {
    if (Full()) {
      if (Empty())
        return;
      Dec(first_);
      *first_ = item;
      last_ = first_;
    } else {
      Dec(first_);
      alloc_traits::construct(allocator_, std::to_address(first_), item);
      ++size_;
    }
  }

  void Reserve(size_type new_capacity) {
    if (new_capacity == Capacity())
      return;

    CCircularBuffer copy(begin(), end());
    Destroy();
    RangeInitialize(copy.begin(), copy.end(), new_capacity);
  }

  CCircularBuffer<T, Alloc>& operator=(const CCircularBuffer<T, Alloc>& other) {
    if (this == &other)
      return *this;
    Destroy();
    RangeInitialize(other.begin(), other.end(), other.Capacity());

    return *this;
  }

  CCircularBuffer<T, Alloc>& operator=(std::initializer_list<value_type> other) {
    Destroy();
    RangeInitialize(other.begin(), other.end(), other.size());

    return *this;
  }

  void Assign(size_type n, const value_type& item) {
    Clear();
    alloc_traits::deallocate(allocator_, std::to_address(begin_), Capacity());
    size_ = n;
    InitializeBuffer(n, item);
    first_ = last_ = begin_;
  }

  template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
  void Assign(InputIterator first, InputIterator last) {
    Clear();
    alloc_traits::deallocate(allocator_, std::to_address(begin_), Capacity());
    RangeInitialize(first, last, last - first);
  }

  void Assign(std::initializer_list<value_type> other) {
    Clear();
    RangeInitialize(other.begin(), other.end(), other.size());
  }

  void PopBack() {
    Dec(last_);
    alloc_traits::destroy(allocator_, last_);
    --size_;
  }

  void PopFront() {
    alloc_traits::destroy(allocator_, first_);
    Inc(first_);
    --size_;
  }

  iterator Erase(const_iterator pos) {
    return Erase(pos, pos + 1);
  }

  iterator Erase(const_iterator first, const_iterator last) {
    CCircularBuffer copy(last, cend());

    pointer p = Add(first_, std::distance(cbegin(), first));
    for (size_t i = 0; i < copy.Size(); ++i) {
      PopBack();
    }

    for (size_t i = 0; i < std::distance(first, last); ++i) {
      PopBack();
    }

    for (const auto& item : copy) {
      PushBack(item);
    }

    return (last_ == p ? end() : iterator(this, p));
  }

  iterator Insert(const_iterator pos, const value_type& item) {
    return Insert(pos, 1, item);
  }

  iterator Insert(const_iterator pos, size_type n, const value_type& value) {
    CCircularBuffer copy(pos, cend());

    for (size_t i = 0; i < copy.Size(); ++i) {
      PopBack();
    }

    pointer pos_to_insert = last_;
    for (; n > 0; --n) {
      if (Full() && pos_to_insert == last_)
        Inc(pos_to_insert);
      PushBack(value);
    }

    for (const auto& item : copy) {
      if (Full() && pos_to_insert == last_)
        Inc(pos_to_insert);
      PushBack(item);
    }

    return iterator(this, pos_to_insert);
  }

  template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
  iterator Insert(const_iterator pos, InputIterator first, InputIterator last) {
    CCircularBuffer copy(pos, cend());

    for (size_t i = 0; i < copy.Size(); ++i) {
      PopBack();
    }

    pointer pos_to_insert = last_;
    for (; first != last; ++first) {
      if (Full() && pos_to_insert == last_)
        Inc(pos_to_insert);
      PushBack(*first);
    }

    for (const auto& item : copy) {
      if (Full() && pos_to_insert == last_)
        Inc(pos_to_insert);
      PushBack(item);
    }

    return iterator(this, pos_to_insert);
  }

  iterator Insert(const_iterator pos, const std::initializer_list<value_type>& il) {
    return Insert(pos, il.begin(), il.end());
  }

  void swap(CCircularBuffer<T, Alloc>& cb) {
    std::swap(begin_, cb.begin_);
    std::swap(end_, cb.end_);
    std::swap(first_, cb.first_);
    std::swap(last_, cb.last_);
    std::swap(size_, cb.size_);
    std::swap(allocator_, cb.allocator_);
  }

  void Clear() {
    Destroy_elements();
    first_ = last_ = begin_;
    size_ = 0;
  }

  ~CCircularBuffer() {
    Destroy();
  }

 private:

  void InitializeBuffer(size_type capacity) {
    begin_ = alloc_traits::allocate(allocator_, capacity);
    end_ = begin_ + capacity;
  }

  void InitializeBuffer(size_type capacity, const value_type& item) {
    InitializeBuffer(capacity);
    FillWithAlloc(begin_, Size(), item);
  }

  void FillWithAlloc(pointer first, size_type n, const T& item) {
    for (; n > 0; ++first, --n)
      alloc_traits::construct(allocator_, std::to_address(first), item);
  }

  template<typename InputIterator>
  void RangeInitialize(InputIterator first, InputIterator last, size_type capacity) {
    size_ = last - first;
    pointer begin = alloc_traits::allocate(allocator_, capacity);
    pointer end = CopyWithAlloc(first, last, begin);
    first_ = begin_ = begin;
    end_ = begin_ + capacity;
    last_ = (end == end_ ? begin_ : end);
  }

  template<typename InputIterator, typename ForwardIterator>
  pointer CopyWithAlloc(InputIterator first, InputIterator last, ForwardIterator dest) {
    for (; first != last; ++first, ++dest)
      alloc_traits::construct(allocator_, std::to_address(dest), *first);

    return dest;
  }

  void Destroy_elements() {
    for (size_type i = 0; i < Size(); ++i, Inc(first_))
      alloc_traits::destroy(allocator_, std::to_address(first_));
  }

  void Destroy() {
    Destroy_elements();
    alloc_traits::deallocate(allocator_, std::to_address(begin_), Capacity());
  }

};

template<typename T, typename Alloc>
bool operator==(const CCircularBuffer<T, Alloc>& lhs, const CCircularBuffer<T, Alloc>& rhs) {
  return lhs.Size() == rhs.Size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename Alloc>
bool operator!=(const CCircularBuffer<T, Alloc>& lhs, const CCircularBuffer<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template<typename T, typename Alloc>
void swap(CCircularBuffer<T, Alloc>& lhs, CCircularBuffer<T, Alloc>& rhs) {
  lhs.swap(rhs);
}
