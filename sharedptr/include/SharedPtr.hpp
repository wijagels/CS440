#include <cstddef>
#include <mutex>
#include <type_traits>
#include <utility>

namespace cs540 {
std::mutex mutex_;  // lol
class RefType {
 public:
  RefType() : ref_count_{1} {}
  virtual ~RefType() {}

  void ref(void) {
    std::unique_lock<std::mutex> lock(mutex_);
    ref_count_++;
  }

  void unref(void) {
    std::unique_lock<std::mutex> lock(mutex_);
    ref_count_--;
    if (ref_count_ == 0) {
      delete this;
    }
  }

  int ref_count_;
};

template <typename U>
class RefDerived : public RefType {
 public:
  RefDerived() = delete;
  RefDerived(U *ptr) : RefType{}, pointer_{ptr} {}

  ~RefDerived() { delete pointer_; }

 private:
  U *pointer_;
};

template <typename T>
class SharedPtr {
 public:
  using element_type = T;
  /* Constructors and Destructor */
  SharedPtr() : ptr_{nullptr}, bare_ptr_{nullptr} {}

  ~SharedPtr() {
    if (ptr_ != nullptr) {
      ptr_->unref();
    }
  }

  template <typename U>
  explicit SharedPtr(U *ptr) : ptr_{new RefDerived<U>{ptr}}, bare_ptr_{ptr} {}

  explicit SharedPtr(const SharedPtr &p)
      : ptr_{p.ptr_}, bare_ptr_{p.bare_ptr_} {
    if (ptr_ != nullptr) ptr_->ref();
  }

  template <typename U>
  SharedPtr(const SharedPtr<U> &p)
      : ptr_{p.ptr_}, bare_ptr_{static_cast<T *>(p.bare_ptr_)} {
    if (ptr_ != nullptr) ptr_->ref();
  }

  SharedPtr(SharedPtr &&p)
      : ptr_{std::move(p.ptr_)}, bare_ptr_{std::move(p.bare_ptr_)} {}

  template <typename U>
  SharedPtr(SharedPtr<U> &&p)
      : ptr_{std::move(p.ptr_)}, bare_ptr_{std::move(p.bare_ptr_)} {}

  template <typename U>
  SharedPtr(const SharedPtr<U> &other, element_type *ptr)
      : ptr_{other.ptr_}, bare_ptr_{ptr} {}

  /* Assignment operators */
  SharedPtr &operator=(const SharedPtr &rhs) {
    if (rhs == *this) return *this;
    if (ptr_ != nullptr) ptr_->unref();
    ptr_ = rhs.ptr_;
    bare_ptr_ = rhs.bare_ptr_;
    if (ptr_ != nullptr) ptr_->ref();
    return *this;
  }

  template <typename U>
  SharedPtr<T> &operator=(const SharedPtr<U> &rhs) {
    if (rhs == *this) return *this;
    if (ptr_ != nullptr) ptr_->unref();
    ptr_ = rhs.ptr_;
    if (ptr_ != nullptr) ptr_->ref();
    return *this;
  }

  SharedPtr &operator=(SharedPtr &&p) {
    ptr_ = std::move(p.ptr_);
    return *this;
  }

  template <typename U>
  SharedPtr &operator=(SharedPtr<U> &&p) {
    ptr_ = std::move(p.ptr_);
    bare_ptr_ = std::move(p.bare_ptr_);
    return *this;
  }

  /* Modifiers */

  /**
  * The smart pointer is set to point to the null pointer. The reference count
  * for the currently pointed to object, if any, is decremented.
  */
  void reset(void) {
    if (ptr_ != nullptr) ptr_->unref();
    ptr_ = nullptr;
    bare_ptr_ = nullptr;
  }

  /**
   * Replace owned resource with another pointer. If the owned resource has no
   * other references, it is deleted. If p has been associated with some other
   * smart pointer, the behavior is undefined.
   */
  template <typename U>
  void reset(U *p) {
    reset();
    ptr_ = new RefDerived<U>{p};
    bare_ptr_ = p;
  }

  /* Observers */

  element_type *get() const { return bare_ptr_; }
  element_type &operator*() const { return *this->get(); }
  element_type *operator->() const { return this->get(); }
  explicit operator bool() const { return bare_ptr_ != nullptr; }

  RefType *ptr_ = nullptr;
  element_type *bare_ptr_ = nullptr;
};

template <typename T1, typename T2>
bool operator==(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs) {
  return lhs.get() == rhs.get();
}

template <typename T>
bool operator==(const SharedPtr<T> &rhs, std::nullptr_t) {
  return !rhs;
}

template <typename T>
bool operator==(std::nullptr_t, const SharedPtr<T> &lhs) {
  return !lhs;
}

template <typename T1, typename T2>
bool operator!=(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator!=(const SharedPtr<T> &lhs, std::nullptr_t) {
  return lhs;
}

template <typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T> &rhs) {
  return rhs;
}

template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
  return SharedPtr<T>{sp, static_cast<T *>(sp.get())};
}

template <typename T, typename U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
  return SharedPtr<T>{sp, dynamic_cast<T *>(sp.get())};
}
}  // namespace cs540
