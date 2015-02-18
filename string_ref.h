#ifndef FMT_STRING_REF_H
#define FMT_STRING_REF_H

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <string>
#include <cstddef>

#include "config.h"

namespace fmt {

/**
  \rst
  A string reference. It can be constructed from a C string or
  ``std::string``.
  
  You can use one of the following typedefs for common character types:

  +------------+-------------------------+
  | Type       | Definition              |
  +============+=========================+
  | StringRef  | BasicStringRef<char>    |
  +------------+-------------------------+
  | WStringRef | BasicStringRef<wchar_t> |
  +------------+-------------------------+

  This class is most useful as a parameter type to allow passing
  different types of strings to a function, for example::

    template <typename... Args>
    std::string format(StringRef format_str, const Args & ... args);

    format("{}", 42);
    format(std::string("{}"), 42);
  \endrst
 */

template <typename Char>
class BasicStringRef {
 public:
  typedef Char                                     value_type;
  typedef const Char*                              pointer;
  typedef const Char*                              const_pointer;
  typedef const Char&                              reference;
  typedef const Char&                              const_reference;
  typedef const_pointer                            const_iterator;
  typedef const_iterator                           iterator;
  typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
  typedef const_reverse_iterator                   reverse_iterator;
  typedef size_t                                   size_type;
  typedef ptrdiff_t                                difference_type;

 private:
  const Char * data_;
  std::size_t size_;

 public:
  /**
    Constructs a string reference object from a C string and a size.
   */
  BasicStringRef(const Char *s, size_type len) FMT_NOEXCEPT
    : data_(s), size_(len) {
      assert(s && "s should not be null");
    }

  /**
    Constructs a string reference object from a C string computing
    the size with ``std::char_traits<Char>::length``.
   */
  BasicStringRef(const Char *s) FMT_NOEXCEPT
    : data_(s), size_(std::char_traits<Char>::length(s)) {
      assert(s && "s should not be null");
    }

  /**
    Constructs a string reference from an `std::string` object.
   */
  BasicStringRef(const std::basic_string<Char> &s) FMT_NOEXCEPT
  : data_(s.data()), size_(s.size()) {}

  /**
    Converts a string reference to an `std::string` object.
   */
  operator std::basic_string<Char>() const {
    return std::basic_string<Char>(data(), size());
  }

  /**
    Returns the pointer to a C string.
   */
  FMT_DEPRECATED("Use data() instead")
  const Char *c_str() const FMT_NOEXCEPT { return data_; }

  /**
    Returns the pointer to the referenced string.
    Note: may return a pointer that is not NUL-terminated
   */
  const Char *data() const FMT_NOEXCEPT { return data_; }

  /**
    Returns the string size.
   */
  size_type size() const FMT_NOEXCEPT { return size_; }

  /**
    Returns the string size.
   */
  size_type length() const FMT_NOEXCEPT { return size_; }

  const_iterator begin() const FMT_NOEXCEPT { return cbegin(); }
  const_iterator end() const FMT_NOEXCEPT { return cend(); }

  const_iterator cbegin() const FMT_NOEXCEPT { return data(); }
  const_iterator cend() const FMT_NOEXCEPT { return data() + size(); }

  const_reverse_iterator rbegin() const FMT_NOEXCEPT { return const_reverse_iterator(cend()); }
  const_reverse_iterator rend() const FMT_NOEXCEPT { return const_reverse_iterator(cbegin()); }
  const_reverse_iterator crbegin() const FMT_NOEXCEPT { return rbegin(); }
  const_reverse_iterator crend() const FMT_NOEXCEPT { return rend(); }

  const_reference operator[](size_type pos) const FMT_NOEXCEPT {
    assert(pos < size());
    return data()[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size()) {
      FMT_THROW(std::out_of_range("BasicStringRef::at()"));
    }
    return operator[](pos);
  }

  /**
    Returns whether the string is empty
   */
  bool empty() const FMT_NOEXCEPT { return size() == 0; }
  
  int compare(BasicStringRef bsr) const FMT_NOEXCEPT {
    size_type rlen = std::min(size(), bsr.size());
    int result = std::char_traits<Char>::compare(data(), bsr.data(), rlen);
    if (result == 0) {
      result = size() - bsr.size();
    }
    return result;
  }

  friend bool operator==(BasicStringRef lhs, BasicStringRef rhs) FMT_NOEXCEPT {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
  }
  friend bool operator!=(BasicStringRef lhs, BasicStringRef rhs) FMT_NOEXCEPT {
    return !(lhs == rhs);
  }
};

typedef BasicStringRef<char> StringRef;
typedef BasicStringRef<wchar_t> WStringRef;

} // namespace fmt

#endif // FMT_STRING_REF_H
