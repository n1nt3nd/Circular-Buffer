#pragma once

#include <iterator>

template<typename Alloc>
struct nonconst_traits {
  typedef typename Alloc::value_type value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef nonconst_traits<Alloc> nonconst_self;
};

template<typename Alloc>
struct const_traits {
  typedef typename Alloc::value_type value_type;
  typedef const value_type* pointer;
  typedef const value_type& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef nonconst_traits<Alloc> nonconst_self;
};

template<typename Container, typename Traits>
class CCircularBufferIter {

 public:
  typedef CCircularBufferIter<Container, typename Traits::nonconst_self> nonconst_self;
  typedef std::random_access_iterator_tag iterator_category;
  typedef typename Traits::value_type value_type;
  typedef typename Traits::difference_type difference_type;
  typedef typename Traits::reference reference;
  typedef typename Traits::pointer pointer;
  typedef typename Traits::size_type size_type;

  const Container* buff_;
  pointer iter_;

 public:
  CCircularBufferIter(const Container* cb, const pointer iter) : buff_(cb), iter_(iter) {}

  CCircularBufferIter() : buff_(0), iter_(0) {}

  CCircularBufferIter(const nonconst_self& it) : buff_(it.buff_), iter_(it.iter_) {}

  CCircularBufferIter& operator=(const CCircularBufferIter& it) {
    if (this == &it)
      return *this;
    buff_ = it.buff_;
    iter_ = it.iter_;

    return *this;
  }

  reference operator*() const {
    return *iter_;
  }

  pointer operator->() const {
    return &(operator*());
  }

  template<typename Traits0>
  difference_type operator-(const CCircularBufferIter<Container, Traits0>& it) const {
    return LinearizePointer(*this) - LinearizePointer(it);
  }

  CCircularBufferIter& operator++() {
    buff_->Inc(iter_);
    if (iter_ == buff_->last_)
      iter_ = 0;

    return *this;
  }

  CCircularBufferIter operator++(int) {
    CCircularBufferIter<Container, Traits> tmp = *this;
    ++*this;

    return tmp;
  }

  CCircularBufferIter& operator--() {
    if (iter_ == 0)
      iter_ = buff_->last_;
    buff_->Dec(iter_);

    return *this;
  }

  CCircularBufferIter operator--(int) {
    CCircularBufferIter<Container, Traits> tmp = *this;
    --*this;

    return tmp;
  }

  CCircularBufferIter& operator+=(difference_type n) {
    if (n > 0) {
      iter_ = buff_->Add(iter_, n);
      if (iter_ == buff_->last_)
        iter_ = 0;
    } else if (n < 0) {
      *this -= -n;
    }

    return *this;
  }

  CCircularBufferIter operator+(difference_type n) const {
    return CCircularBufferIter<Container, Traits>(*this) += n;
  }

  CCircularBufferIter& operator-=(difference_type n) {
    if (n > 0) {
      iter_ = buff_->Sub(iter_ == 0 ? buff_->last_ : iter_, n);
    } else if (n < 0) {
      *this += -n;
    }

    return *this;
  }

  CCircularBufferIter operator-(difference_type n) const {
    return CCircularBufferIter<Container, Traits>(*this) -= n;
  }

  reference operator[](difference_type n) const {
    return *(*this + n);
  }

  template<class Traits0>
  typename Traits0::pointer LinearizePointer(const CCircularBufferIter<Container, Traits0>& it) const {
    return it.iter_ == 0 ? buff_->begin_ + buff_->Size() :
           (it.iter_ < buff_->first_ ? it.iter_ + (buff_->end_ - buff_->first_)
                                     : buff_->begin_ + (it.iter_ - buff_->first_));
  }

  template<class Traits0>
  bool operator==(const CCircularBufferIter<Container, Traits0>& it) const {
    return iter_ == it.iter_;
  }

  template<class Traits0>
  bool operator!=(const CCircularBufferIter<Container, Traits0>& it) const {
    return iter_ != it.iter_;
  }

  template<class Traits0>
  bool operator<(const CCircularBufferIter<Container, Traits0>& it) const {
    return LinearizePointer(*this) < LinearizePointer(it);
  }

  template<class Traits0>
  bool operator>(const CCircularBufferIter<Container, Traits0>& it) const {
    return it < *this;
  }

  template<class Traits0>
  bool operator<=(const CCircularBufferIter<Container, Traits0>& it) const {
    return !(it < *this);
  }

  template<class Traits0>
  bool operator>=(const CCircularBufferIter<Container, Traits0>& it) const {
    return !(*this < it);
  }

};