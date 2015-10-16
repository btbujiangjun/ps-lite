#pragma once
#include "./base.h"
#include <memory>
namespace ps {

/**
 * \brief Shared array
 *
 * A smart array that retains shared ownership.  It provides similar
 * functionality to std::vector, but both copy are assign are passing by
 * pointers.
 *
 * \tparam V the value type
 */
template<typename V>
class SArray {
 public:
  /** \brief empty constructor */
  SArray() : size_(0), capacity_(0) { }

  /** \brief empty deconstrcutor */
  ~SArray() { }

  /**
   * \brief Create an array with length n with initialized value
   * \param size the length
   * \param val the initial length (0 in default)
   */
  explicit SArray(size_t size, V val = 0) { resize(size, val); }


  /**
   * \brief construct from another SArray.
   *
   * Zero-copy constructor, namely just copy the pointer
   *
   * \tparam W the value type of the source array
   * \param arr the source array
   */
  template <typename W>
  explicit SArray(const SArray<W>& arr) { *this = arr; }

  /**
   * \brief construct from another SArray.
   *
   * Zero-copy constructor, namely just copy the pointer
   *
   * \tparam W the value type of the source array
   * \param arr the source array
   */
  template <typename W> void operator=(const SArray<W>& arr) {
    size_ = arr.size() * sizeof(W) / sizeof(V);
    CHECK_EQ(size_ * sizeof(V), arr.size() * sizeof(W)) << "cannot be divided";
    capacity_ = arr.capacity() * sizeof(W) / sizeof(V);
    ptr_ = std::shared_ptr<V>(arr.ptr(), reinterpret_cast<V*>(arr.data()));
  }

  /**
   * \brief construct from a c-array
   *
   * Zero-copy constructor, namely just copy the pointer
   *
   * \param data the source data
   * \param size the length
   * \param deletable whether or not can call `delete [] data` when the reference
   * count goes 0
   */

  SArray(V* data, size_t size, bool deletable = false) {
    if (deletable) {
      reset(data, size, [](V* data){ delete [] data; });
    } else {
      reset(data, size, [](V* data) { });
    }
  }


  /**
   * \brief copy from a c-array
   *
   * \param data the source data
   * \param size the length
   */
  void CopyFrom(const V* data, size_t size) {
    resize(size);
    memcpy(data(), data, size*sizeof(V));
  }

  /**
   * \brief copy from an iterator
   */
  template <typename ForwardIt>
  void CopyFrom(const ForwardIt& first, const ForwardIt& last) {
    int size = (int)std::distance(first, last);
    V* data = new V[size];
    reset(data, size, true);
    auto it = first;
    while (size > 0) { *data = *it; ++ data; ++ it; }
  }

  /**
   * \brief construct from a std::vector, copy the data
   */
  SArray(const std::vector<V>& vec) { CopyFrom(vec.data(), vec.size()); }

  /** @brief Copy from a initializer_list */
  template <typename W> SArray(const std::initializer_list<W>& list) {
    CopyFrom(list.begin(), list.end());
  }

  /** @brief Copy from a initializer_list */
  template <typename W> void operator=(const std::initializer_list<W>& list) {
    CopyFrom(list.begin(), list.end());
  }

  /**
   * @brief Reset the current data pointer with a deleter
   */
  template <typename Deleter>
  void reset(V* data, size_t size, Deleter del) {
    size_ = size; capacity_ = size; ptr_.reset(data, del);
  }


  /**
   * @brief Resizes the array to size elements
   *
   * If size <= capacity_, then only change the size. otherwise, append size -
   * current_size entries (without value initialization)
   */
  void resize(size_t size) {
    if (capacity_ >= size) { size_ = size; return; }
    V* new_data = new V[size+5];
    memcpy(new_data, data(), size_*sizeof(V));
    reset(new_data, size, [](V* data){ delete [] data; });
  }

  /**
   * @brief Resizes the array to size elements
   *
   * If size <= capacity_, then only change the size. otherwise, append size -
   * current_size entries, and then set new value to val
   */
  void resize(size_t size, V val) {
    size_t cur_n = size_;
    resize(size);
    if (size <= cur_n) return;
    V* p = data() + cur_n;
    if (val == 0) {
      memset(p, 0, (size - cur_n)*sizeof(V));
    } else {
      for (size_t i = 0; i < size - cur_n; ++i) { *p = val; ++p; }
    }
  }

  /**
   * @brief Requests that the capacity be at least enough to contain n elements.
   */
  void reserve(size_t size) {
    if (capacity_ >= size) { return; }
    size_t old_size = size_;
    resize(size);
    size_ = old_size;
  }

  /** @brief release the memory */
  void clear() { reset(nullptr, 0, [](V* data) {}); }


  inline bool empty() const { return size() == 0; }
  inline size_t size() const { return size_; }
  inline size_t capacity() const { return capacity_; }

  inline V* begin() { return data(); }
  inline const V* begin() const { return data(); }
  inline V* end() { return data() + size(); }
  inline const V* end() const { return data() + size(); }

  inline V* data() const { return ptr_.get(); }

  /** \brief get the shared pointer */
  inline std::shared_ptr<V>& ptr() { return ptr_; }
  /** \brief get the const shared pointer */
  inline const std::shared_ptr<V>& ptr() const { return ptr_; }

  inline V back() const { CHECK(!empty()); return data()[size_-1]; }
  inline V front() const { CHECK(!empty()); return data()[0]; }
  inline V& operator[] (int i) { return data()[i]; }
  inline const V& operator[] (int i) const { return data()[i]; }

  inline void push_back(const V& val) {
    if (size_ == capacity_) reserve(size_*2+5);
    data()[size_++] = val;
  }

  void pop_back() { if (size_) --size_; }

  void append(const SArray<V>& arr) {
    if (arr.empty()) return;
    auto orig_size = size_;
    resize(size_ + arr.size());
    memcpy(data()+orig_size, arr.data(), arr.size()*sizeof(V));
  }

 private:
  size_t size_;
  size_t capacity_;
  std::shared_ptr<V> ptr_;

};

/**
 * \brief print a debug string
 */
// template <typename V>
// std::ostream& operator<<(std::ostream& os, const SArray<V>& obj) {
  // os << obj.blob();
  // return os;
// }

}  // namespace ps
