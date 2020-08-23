#include "gtest/gtest.h"

#include "zim/zim.h"
#include "zim/file.h"


TEST(Zimcheck, getFileSize)
{
  std::string fn = "data/zimfiles/wikibooks_be_all_nopic_2017-02.zim";

//    ASSERT_TRUE(fileExists(fn));

    zim::File file(fn);
    

    

}
