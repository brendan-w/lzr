#include <string.h>

#include "gtest/gtest.h"

#include "lzr/lzr.hpp"

const char TMP_ILDA_FILE[] = "/tmp/output.ilda";
const char NAME[] = "NAME----";
const char COMPANY[] = "COMPANY-";

using namespace lzr;

TEST(ILDA, ReadSaveRead)
{
    // Open ILDA file
    FrameList original;

    // load the ILDA file
    ILDA* f = ilda_open("share/ildatest.ild", "r");
    ASSERT_NE(f, nullptr);
    ASSERT_EQ(ilda_read(f, 0, original), LZR_SUCCESS);
    ilda_close(f);

    // Write ILDA file
    f = ilda_open(TMP_ILDA_FILE, "w");
    ASSERT_NE(f, nullptr);
    ASSERT_EQ(ilda_write(f, 0, original, NAME, COMPANY), LZR_SUCCESS);
    ilda_close(f);

    // Read the ILDA file we just wrote
    char name[9];
    char company[9];
    FrameList frames;

    f = ilda_open(TMP_ILDA_FILE, "r");
    ASSERT_NE(f, nullptr);
    ASSERT_EQ(ilda_read(f, 0, frames, name, company), LZR_SUCCESS);
    ilda_close(f);

    // Cleanup the tmpfile
    std::remove(TMP_ILDA_FILE);

    EXPECT_EQ(strcmp(NAME, name), 0);
    EXPECT_EQ(strcmp(COMPANY, company), 0);
    EXPECT_EQ(original, frames);
}
