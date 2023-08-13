//
// Created by happy on 2023/8/13.
//
#include <iostream>
#include <string>

template<typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string_view {
 public:
  using traits_type = Traits;
  using size_type = std::size_t;

  using value_type = CharT;

  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  using const_iterator = const value_type *;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;

  static constexpr size_type npos = size_type(-1);

 public:
  constexpr basic_string_view() noexcept: str_{nullptr}, size_{0} {}

  constexpr basic_string_view(const basic_string_view &) noexcept = default;

  constexpr basic_string_view(basic_string_view &&) noexcept = default;

  constexpr explicit basic_string_view(const CharT *str) noexcept: str_{str}, size_{traits_type::length(str)} {}

  constexpr basic_string_view(const CharT *str, size_type len) noexcept: str_{str}, size_{len} {}

  constexpr basic_string_view &operator=(const basic_string_view &) noexcept = default;

  constexpr explicit basic_string_view(const std::basic_string<CharT, Traits> &str) noexcept
      : str_{str.c_str()}, size_{str.size()} {};

  constexpr explicit operator std::basic_string<CharT, Traits>() const {
    return std::basic_string<CharT, Traits>(str_, size_);
  }

 public:
  constexpr iterator begin() const noexcept { return str_; }

  constexpr iterator end() const noexcept { return str_ + size_; }

  constexpr const_iterator cbegin() const noexcept { return str_; }

  constexpr const_iterator cend() const noexcept { return str_ + size_; }

  constexpr iterator rbegin() const noexcept { return reverse_iterator(this->end()); }

  constexpr iterator rend() const noexcept { return reverse_iterator(this->begin()); }

  constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(this->cend()); }

  constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(this->cbegin()); }

 public:
  constexpr size_type size() const noexcept { return size_; }

  constexpr size_type length() const noexcept { return size_; }

  constexpr size_type max_size() const noexcept {
    return (npos - sizeof(size_type) - sizeof(void *)) / sizeof(value_type) / 4;
  }

  constexpr bool empty() const noexcept { return size_ == 0; }

 public:
  constexpr const_pointer data() const noexcept { return str_; }

  constexpr const_reference operator[](size_type pos) const noexcept { return *(this->str_ + size_); }

  constexpr const_reference at(size_type pos) const {
    if (pos >= this->size_)
      throw std::out_of_range{"out of range"};

    return *(this->str_ + size_);
  }

  constexpr const_reference front() const noexcept { return *str_; }

  constexpr const_reference back() const noexcept { return *(str_ + size_ - 1); }

  constexpr void remove_prefix(size_type n) noexcept {
    str_ += n;
    size_ -= n;
  }

  constexpr void remove_suffix(size_type n) noexcept { size_ -= n; }

  constexpr void swap(basic_string_view &v) {
    auto temp = *this;
    *this = v;
    v = temp;
  }

  constexpr basic_string_view substr(size_type pos = 0, size_type len = npos) {
    if (pos > size_)
      throw std::out_of_range{"out of range"};

    const size_type n = std::min(len, size_ - pos);
    return basic_string_view(str_ + pos, n);
  }

 private:
  const CharT *str_;
  size_type size_;
};

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os,
                                              const basic_string_view<CharT, Traits> &str) {
  os.write(str.data(), static_cast<std::streamsize>(str.size()));
  return os;
}

using string_view = basic_string_view<char>;

int main() {
  const char *data = "helloWorld 12345678";

  string_view sv1{data};
  string_view sv2{data, 5};
  string_view sv3{data + 3, 3};
  string_view sv4{data + 5};

  std::string str{"helloWorld"};
  string_view sv5{str};
  std::string string = static_cast<std::string>(sv5);

  std::cout << data << std::endl;
  std::cout << sv1 << std::endl;
  std::cout << sv2 << std::endl;
  std::cout << sv3 << std::endl;
  std::cout << sv4 << std::endl;
  std::cout << sv5 << std::endl;
  std::cout << string << std::endl;

  return 0;
}
