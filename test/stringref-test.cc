#include <cstring>
#include <string>

#include "gmock/gmock.h"

#include "string_ref.h"

using fmt::StringRef;

TEST(StringRefTest, CtorFromCString) {
  const char * const cstr = "abc";
  StringRef sref(cstr);
  EXPECT_STREQ(cstr, sref.data());
  EXPECT_EQ(std::strlen(cstr), sref.size());
}

TEST(StringRefTest, CtorFromStdString) {
  const std::string str = "defg";
  StringRef sref(str);
  EXPECT_STREQ(str.data(), sref.data());
  EXPECT_EQ(str.size(), sref.size());
  EXPECT_EQ(str.length(), sref.size());
}

TEST(StringRefTest, CtorFromStrbufAndLength) {
  const char * const cstr = "123456789";
  StringRef sref(&cstr[3], 3);
  EXPECT_EQ(3u, sref.size());
  EXPECT_EQ(3u, sref.length());
  EXPECT_EQ(0, std::memcmp(sref.data(), "456", 3));
}

TEST(StringRefTest, ConvertToString) {
  std::string s = StringRef("abc");
  EXPECT_EQ("abc", s);
}

TEST(StringRefTest, BeginEnd) {
  const char * const cstr = "123456789";
  StringRef sref(&cstr[3], 3);
  std::string s1;
#if __cplusplus >= 201103L
  for (char c : sref) {
    s1 += c;
  }
#else
  for (StringRef::iterator it = sref.begin(); it != sref.end(); ++it) {
    s1 += *it;
  }
#endif
  EXPECT_EQ("456", s1);

  std::string s2(sref.cbegin(), sref.cend());
  EXPECT_EQ("456", s2);
}

TEST(StringRefTest, RbeginRend) {
  const char * const cstr = "987654321";
  StringRef sref(&cstr[3], 3);
  std::string s(sref.rbegin(), sref.rend());
  EXPECT_EQ("456", s);
  
  EXPECT_EQ('4', *sref.crbegin());
  EXPECT_EQ('6', *(sref.crend() - 1));
}

TEST(StringRefTest, Subscript) {
  const char * const cstr = "987654321";
  StringRef sref(&cstr[3], 3);
  std::string s;
  for (size_t i = 0; i < sref.size(); ++i) {
    s += sref[i];
  }
  EXPECT_EQ("654", s);
}

TEST(StringRefTest, IndexWithAt) {
  const char * const cstr = "123456789";
  StringRef sref(&cstr[3], 3);
  EXPECT_EQ('4', sref.at(0));
  EXPECT_EQ('5', sref.at(1));
  EXPECT_EQ('6', sref.at(2));

  // FIXME: This should work
  //try {
  //  sref.at(3);
  //  FAIL();
  //} catch (std::out_of_range& e) {
  //  EXPECT_STREQ("BasicStringRef::at()", e.what());
  //}
}

TEST(StringRefTest, EmptyString) {
  StringRef sref("1234", 0);
  EXPECT_EQ(0, sref.size());
  EXPECT_TRUE(sref.empty());
}

TEST(StringRefTest, CompareLess) {
  EXPECT_LT(StringRef("0000").compare("0123"), 0);
  EXPECT_LT(StringRef("0123").compare("123"), 0);
  EXPECT_LT(StringRef("012").compare("0123"), 0);
  EXPECT_LT(StringRef("").compare("0"), 0);
}

TEST(StringRefTest, CompareGreater) {
  EXPECT_GT(StringRef("0123").compare("0000"), 0);
  EXPECT_GT(StringRef("123").compare("0123"), 0);
  EXPECT_GT(StringRef("0123").compare("012"), 0);
  EXPECT_GT(StringRef("0").compare(""), 0);
}

TEST(StringRefTest, CompareEqual) {
  EXPECT_EQ(StringRef("1234"), StringRef(std::string("1234")));
  EXPECT_EQ(StringRef("234"), StringRef(&"12345"[1], 3));
  EXPECT_EQ(StringRef(""), "");
  EXPECT_EQ(StringRef(""), std::string());
  EXPECT_EQ(StringRef(""), StringRef(&"123456"[6], 0));

  EXPECT_NE(StringRef("1234"), StringRef("1234", 2));
  EXPECT_NE(StringRef("1234"), "");
}

