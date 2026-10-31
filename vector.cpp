#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>

class IntArray {
private:
  int m_length{};
  int* m_data{};

public:
  IntArray() = default;

  IntArray(int length) : m_length{length} {
    assert(length > 0);
    if (length > 0) {
      m_data = new int[static_cast<std::size_t>(length)];
    }
  }

  int& operator[](int idx) {

    assert(idx >= 0 && idx <= m_length);
    return m_data[idx];
  }

  ~IntArray() { delete[] m_data; }

  IntArray(const IntArray& array) : IntArray(array.getLength()) {
    std::copy_n(array.m_data, m_length, m_data);
  }

  int getLength() const { return m_length; }

  IntArray& operator=(IntArray& other) {
    if (&other == this) {
      return *this;
    }
    reallocate(other.getLength());
    std::copy_n(other.m_data, m_length, m_data);

    return *this;
  }

  void reallocate(int newLength) {
    erase();
    assert(newLength >= 0);

    m_data = new int[static_cast<std::size_t>(newLength)];
    m_length = newLength;
  }

  void erase() {
    delete[] m_data;
    m_data = {nullptr};
    m_length = 0;
  }

  void resize(int newLength) {
    if (newLength == m_length) {
      return;
    }

    if (m_length <= 0) {
      erase();
      return;
    }
    int* data{new int[static_cast<std::size_t>(newLength)]};

    std::copy_n(m_data, m_length, data);

    delete[] m_data;
    m_data = data;
    m_length = newLength;
  }

  void insertBefore(int value, int idx) {
    assert(idx <= m_length && idx >= 0);

    int* data{new int[static_cast<std::size_t>(m_length + 1)]};

    std::copy_n(m_data, idx, data);

    data[idx] = value;

    std::copy_n(m_data + idx, m_length - idx, data + idx + 1);

    delete[] m_data;
    m_data = data;
    m_length++;
  }

  void remove(int idx) {
    assert(idx <= m_length && idx >= 0);

    if (m_length == 1) {
      erase();
      return;
    }
    int* data{new int[static_cast<std::size_t>(m_length)]};

    std::copy_n(m_data, idx, data);
    std::copy_n(m_data + idx + 1, m_length - idx - 1, data + idx);

    delete[] m_data;
    m_data = data;
    m_length--;
  }

  void insertAtBeginning(int val) { return insertBefore(val, 0); }
  void insertAtEnd(int val) { return insertBefore(val, m_length); }
};

int main() {

  IntArray array(10);


  for (int i{0}; i < 10; ++i)
    array[i] = i + 1;


  array.resize(8);

  array.insertBefore(20, 5);

  array.remove(3);

  array.insertAtEnd(30);
  array.insertAtBeginning(40);

  {
    IntArray b{array};
    b = array;
    b = b;
    array = array;
  }

  for (int i{0}; i < array.getLength(); ++i)
    std::cout << array[i] << ' ';

  std::cout << '\n';

  return 0;
}
