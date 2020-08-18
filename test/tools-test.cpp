#include "gtest/gtest.h"

#include "../src/tools.h"
#include <magic.h>
#include <unordered_map>

std::string directoryPath;
magic_t magic;
bool inflateHtmlFlag = false;
bool isVerbose() { return false; }

TEST(CommonTools, getFileSize)
{
  std::string fn = "data/minimal-content/favicon.png";

  ASSERT_TRUE(fileExists(fn));
  EXPECT_EQ(getFileSize(fn), 2725);
}

TEST(CommonTools, base64_encode)
{
  unsigned char data[] = { 0xff, 0x00, 0x7a };
  std::string txt = base64_encode(data, sizeof(data));
  EXPECT_EQ(txt, "/wB6");
}

TEST(CommonTools, decodeUrl)
{
  std::string src = "%00";
  std::string res = decodeUrl(src);
  EXPECT_EQ(res.size(), 1);
  EXPECT_EQ(res[0], '\0');

  src = "%ff";
  res = decodeUrl(src);
  EXPECT_EQ(res.size(), 1);
  EXPECT_EQ(res[0], '\xff');

  std::unordered_map<const char *, const std::string> expectationsMap = {
    // test normal use
    { "https://www.example.com/cgi-bin/search.cgi?q=example%20search",
      "https://www.example.com/cgi-bin/search.cgi?q=example search" },
    { "%2a", "*" },
    // test corner cases
    { "%", "%" },
    { "%2", "%2" },
    { "%%","%%" },
    { "%%%", "%%%" },
    { "%at", "%at" },
    { "%%ft", "%%ft" },
    { "%%53", "%S"},
    { "%%5t", "%%5t"}
  };

  for (auto p : expectationsMap) {
    std::string res = decodeUrl(p.first);
    EXPECT_EQ(res, p.second);
  }
}

TEST(CommonTools, computeAbsolutePath)
{
  std::string str;

  str = computeAbsolutePath("", "");
  EXPECT_EQ(str, "");

  str = computeAbsolutePath("/home/alex/oz/zim-tools/test/data/", "minimal-content/hello.html");
  EXPECT_EQ(str, "/home/alex/oz/zim-tools/test/data/minimal-content/hello.html");

  str = computeAbsolutePath("../test/data", "minimal-content/hello.html");
  EXPECT_EQ(str, "../test/minimal-content/hello.html");

  // without trailing /  'data' component will be stripped from path:
  str = computeAbsolutePath("/home/alex/oz/zim-tools/test/data", "minimal-content/hello.html");
  EXPECT_EQ(str, "/home/alex/oz/zim-tools/test/minimal-content/hello.html");
}

TEST(CommonTools, computeRelativePath)
{
  std::string str;

  str = computeRelativePath("", "");
  EXPECT_EQ(str, "");

  str = computeRelativePath("a", "a");
  EXPECT_EQ(str, "");

  str = computeRelativePath("aa/b", "aa/c");
  EXPECT_EQ(str, "c");

  str = computeRelativePath("b", "ab");
  EXPECT_EQ(str, "ab");

  str = computeRelativePath("a", "a/b");
  EXPECT_EQ(str, "b");

  str = computeRelativePath("a/b", "a/b");
  EXPECT_EQ(str, "");

  str = computeRelativePath("old/article1.html", "new/article1.html");
  EXPECT_EQ(str, "../new/article1.html");
}

TEST(CommonTools, replaceStringInPlaceOnce)
{
  std::string str;

  str = "";
  replaceStringInPlaceOnce(str, "", "");
  EXPECT_EQ(str, "");

  str = "abcd";
  replaceStringInPlace(str, "a", "");
  EXPECT_EQ(str, "bcd");

  str = "abcd";
  replaceStringInPlaceOnce(str, "a", "b");
  EXPECT_EQ(str, "bbcd");

  str = "aabcd";
  replaceStringInPlaceOnce(str, "a", "b");
  EXPECT_EQ(str, "babcd");
}

TEST(CommonTools, replaceStringInPlace)
{
  std::string str;

  str = "";
  replaceStringInPlace(str, "", "");
  EXPECT_EQ(str, "");

  str = "abcd";
  replaceStringInPlace(str, "a", "b");
  EXPECT_EQ(str, "bbcd");

  str = "abcd";
  replaceStringInPlace(str, "a", "");
  EXPECT_EQ(str, "bcd");

  str = "aabcd";
  replaceStringInPlace(str, "a", "b");
  EXPECT_EQ(str, "bbbcd");
}

TEST(CommonTools, stripTitleInvalidChars)
{
  std::string str;

  str = "\u202Aheader\u202A";
  stripTitleInvalidChars(str);
  EXPECT_EQ(str, "header");
}

TEST(CommonTools, getNamespaceForMimeType)
{
  // with uniq namespace
  EXPECT_EQ(getNamespaceForMimeType("text/html", true), "A");
  EXPECT_EQ(getNamespaceForMimeType("text/xml", true), "A");
  EXPECT_EQ(getNamespaceForMimeType("image/png", true), "A");
  EXPECT_EQ(getNamespaceForMimeType("application/json", true), "A");

  // without uniq namespace
  EXPECT_EQ(getNamespaceForMimeType("text/html", false), "A");
  EXPECT_EQ(getNamespaceForMimeType("text/xml", false), "-");
  EXPECT_EQ(getNamespaceForMimeType("image/png", false), "I");
  EXPECT_EQ(getNamespaceForMimeType("application/json", false), "-");
}