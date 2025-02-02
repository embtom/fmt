// Formatting library for C++ - the core API
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.
//
// Copyright (c) 2018 - present, Remotion (Igor Schulz)
// All Rights Reserved
// {fmt} support for ranges, containers and types tuple interface.

/// Check if  'if constexpr' is supported.
#if (__cplusplus > 201402L) || \
    (defined(_MSVC_LANG) && _MSVC_LANG > 201402L && _MSC_VER >= 1910)

#  include "fmt/ranges.h"
#  include <gmock/gmock.h>

#  include <array>
#  include <map>
#  include <string>
#  include <vector>

TEST(RangesTest, FormatVector) {
  std::vector<int32_t> iv{1, 2, 3, 5, 7, 11};
  auto ivf = fmt::format("{}", iv);
  EXPECT_EQ("{1, 2, 3, 5, 7, 11}", ivf);
}

TEST(RangesTest, FormatVector2) {
  std::vector<std::vector<int32_t>> ivv{{1, 2}, {3, 5}, {7, 11}};
  auto ivf = fmt::format("{}", ivv);
  EXPECT_EQ("{{1, 2}, {3, 5}, {7, 11}}", ivf);
}

TEST(RangesTest, FormatMap) {
  std::map<std::string, int32_t> simap{{"one", 1}, {"two", 2}};
  EXPECT_EQ("{(\"one\", 1), (\"two\", 2)}", fmt::format("{}", simap));
}

TEST(RangesTest, FormatPair) {
  std::pair<int64_t, float> pa1{42, 1.5f};
  EXPECT_EQ("(42, 1.5)", fmt::format("{}", pa1));
}

TEST(RangesTest, FormatTuple) {
  std::tuple<int64_t, float, std::string, char> tu1{42, 1.5f, "this is tuple",
                                                    'i'};
  EXPECT_EQ("(42, 1.5, \"this is tuple\", 'i')", fmt::format("{}", tu1));
}

struct my_struct {
  int32_t i;
  std::string str;  // can throw
  template <std::size_t N> decltype(auto) get() const noexcept {
    if constexpr (N == 0)
      return i;
    else if constexpr (N == 1)
      return fmt::string_view{str};
  }
};

template <std::size_t N> decltype(auto) get(const my_struct& s) noexcept {
  return s.get<N>();
}

namespace std {

template <>
struct tuple_size<my_struct> : std::integral_constant<std::size_t, 2> {};

template <std::size_t N> struct tuple_element<N, my_struct> {
  using type = decltype(std::declval<my_struct>().get<N>());
};

}  // namespace std

TEST(RangesTest, FormatStruct) {
  my_struct mst{13, "my struct"};
  EXPECT_EQ("(13, \"my struct\")", fmt::format("{}", mst));
}

TEST(RangesTest, FormatTo) {
  char buf[10];
  auto end = fmt::format_to(buf, "{}", std::vector{1, 2, 3});
  *end = '\0';
  EXPECT_STREQ(buf, "{1, 2, 3}");
}

struct path_like {
  const path_like* begin() const;
  const path_like* end() const;

  operator std::string() const;
};

TEST(RangesTest, PathLike) {
  EXPECT_FALSE((fmt::is_range<path_like, char>::value));
}

#endif  // (__cplusplus > 201402L) || (defined(_MSVC_LANG) && _MSVC_LANG >
        // 201402L && _MSC_VER >= 1910)
