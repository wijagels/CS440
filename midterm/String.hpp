#ifndef CS540_STRING_HPP
#define CS540_STRING_HPP

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace cs540 {

class String {
 public:
  String() : String{""} {}
  explicit String(std::string other) { m_valobj = new ValObj(other); }
  String(const String &other) { m_valobj = other.m_valobj->newref(); }
  ~String() { m_valobj->~ValObj(); }

  String &operator=(const String &rhs) {
    if (&rhs == this) {
      return *this;
    }
    m_valobj->~ValObj();
    m_valobj = rhs.m_valobj;
    return *this;
  }

  String &operator=(const std::string &rhs) {
    m_valobj->~ValObj();
    m_valobj->reset_str(rhs);
    return *this;
  }

  String &operator=(const char *rhs) {
    m_valobj->~ValObj();
    m_valobj->reset_str(std::string(rhs));
    return *this;
  }

  std::string to_std_string(void) const { return std::string(m_valobj->m_buf); }

  operator std::string() const { return std::string(m_valobj->m_buf); }

  char &operator[](size_t pos) { return m_valobj->m_buf[pos]; }

  const char &operator[](size_t pos) const { return m_valobj->m_buf[pos]; }

  String operator+(const String &rhs) const {
    return String(std::string(*this) + std::string(rhs));
  }

  String operator+(const std::string rhs) const {
    return String(std::string(*this) + rhs);
  }

  friend std::ostream &operator<<(std::ostream &stream, const String &rhs) {
    stream << rhs.to_std_string();
    return stream;
  }

 protected:
  struct ValObj {
    // Need member functions here.
    explicit ValObj(std::string init) : m_refcount{1} {
      m_buf = (char *)malloc((init.size() + 1) * sizeof *m_buf);
      memcpy(m_buf, init.c_str(), (init.size() + 1) * sizeof *m_buf);
    }

    void reset_str(const std::string &newstr) {
      free(m_buf);
      m_buf = (char *)malloc((newstr.size() + 1) * sizeof *m_buf);
      memcpy(m_buf, newstr.c_str(), (newstr.size() + 1) * sizeof *m_buf);
    }

    ~ValObj() {
      // if (m_refcount == -1) return;
      if ((--m_refcount) == 0) {
        m_refcount = -1;
        free(m_buf);
        delete this;
      }
    }

    ValObj *newref() {
      m_refcount++;
      return this;
    }

    char *m_buf;
    int m_refcount;
  };
  ValObj *m_valobj;
};
}

#endif
