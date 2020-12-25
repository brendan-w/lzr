#include "gtest/gtest.h"

#include "lzr/lzr.hpp"

using namespace lzr;

// Helpers to avoid a lot of "255, 255, 255, 255"
inline Point LitPoint(float x, float y) { return Point(x, y, 255, 255, 255, 255); }
inline Point BlankedPoint(float x, float y) { return Point(x, y, 0, 0, 0, 0); }

TEST(Decimator, StackedPointsBeam)
{
    Frame frame;
    frame.add(LitPoint(0.0, 0.0));  // Dupe 1
    frame.add(LitPoint(0.0, 0.0));  // Dupe 1
    frame.add(LitPoint(0.0, 0.0));  // Dupe 1

    ASSERT_EQ(decimate(frame, 3 /* beam_threshold */), LZR_SUCCESS);

    ASSERT_EQ(frame.size(), 1);
    EXPECT_TRUE(frame[0].beam);
}

TEST(Decimator, StackedPointsNotBeam)
{
    Frame frame;
    frame.add(LitPoint(0.0, 0.0));  // Dupe 1
    frame.add(LitPoint(0.0, 0.0));  // Dupe 1

    ASSERT_EQ(decimate(frame, 3 /* beam_threshold */), LZR_SUCCESS);

    ASSERT_EQ(frame.size(), 1);
    EXPECT_FALSE(frame[0].beam);
}

TEST(Decimator, ColorChangesStack)
{
    Frame frame;
    frame.add(Point(0.0, 0.0, 255, 255, 255, 255));
    frame.add(Point(0.0, 0.0, 0,   255, 255, 255));
    frame.add(Point(0.0, 0.0, 255, 0,   255, 255));
    frame.add(Point(0.0, 0.0, 255, 255, 0,   255));
    frame.add(Point(0.0, 0.0, 255, 255, 255, 255));

    ASSERT_EQ(decimate(frame), LZR_SUCCESS);
    ASSERT_EQ(frame.size(), 1);
}

TEST(Decimator, DiscardIntermediateBlanks)
{
    Frame frame;
    frame.add(    LitPoint(0.0, 0.0));
    frame.add(BlankedPoint(1.0, 0.0));  // Crazy circuitous blanked route
    frame.add(BlankedPoint(-1.0, 0.0));
    frame.add(BlankedPoint(1.0, 0.0));  // Dupe
    frame.add(BlankedPoint(1.0, 0.0));  // Dupe
    frame.add(    LitPoint(1.0, 0.0));


    ASSERT_EQ(decimate(frame, 3 /* beam_threshold */), LZR_SUCCESS);

    ASSERT_EQ(frame.size(), 3);

    EXPECT_EQ(frame[0].x, 0.0);
    EXPECT_TRUE(frame[0].is_lit());

    EXPECT_EQ(frame[1].x, 1.0);
    EXPECT_TRUE(frame[1].is_blanked());
    EXPECT_FALSE(frame[1].beam);

    EXPECT_EQ(frame[2].x, 1.0);
    EXPECT_TRUE(frame[2].is_lit());
}

TEST(Decimator, LeadingTrailingBlanks)
{
    Frame frame;
    frame.add(BlankedPoint(0.0, 0.0));
    frame.add(BlankedPoint(0.0, 0.0));
    frame.add(    LitPoint(0.0, 0.0));
    frame.add(BlankedPoint(0.0, 0.0));
    frame.add(BlankedPoint(0.0, 0.0));

    ASSERT_EQ(decimate(frame), LZR_SUCCESS);

    ASSERT_EQ(frame.size(), 1);
    EXPECT_TRUE(frame[0].is_lit());
}

TEST(Decimator, RemoveInterpolation)
{
    Frame frame;
    frame.add(LitPoint(0.0, 0.0));
    frame.add(LitPoint(0.1, 0.0));
    frame.add(LitPoint(0.2, 0.0));
    frame.add(LitPoint(0.3, 0.0));
    frame.add(LitPoint(0.4, 0.0));
    frame.add(LitPoint(0.4, 0.1));
    frame.add(LitPoint(0.4, 0.2));
    frame.add(LitPoint(0.4, 0.3));
    frame.add(LitPoint(0.4, 0.4));
    frame.add(BlankedPoint(0.5, 0.5));
    frame.add(LitPoint(0.5, 0.5));
    frame.add(LitPoint(0.6, 0.6));
    frame.add(LitPoint(0.7, 0.7));
    frame.add(LitPoint(0.8, 0.8));

    ASSERT_EQ(decimate(frame), LZR_SUCCESS);

    ASSERT_EQ(frame.size(), 6);

    EXPECT_EQ(frame[0].x, 0.0);
    EXPECT_EQ(frame[0].y, 0.0);
    EXPECT_TRUE(frame[0].is_lit());

    EXPECT_EQ(frame[1].x, 0.4f);
    EXPECT_EQ(frame[1].y, 0.0);
    EXPECT_TRUE(frame[1].is_lit());

    EXPECT_EQ(frame[2].x, 0.4f);
    EXPECT_EQ(frame[2].y, 0.4f);
    EXPECT_TRUE(frame[2].is_lit());

    EXPECT_EQ(frame[3].x, 0.5f);
    EXPECT_EQ(frame[3].y, 0.5f);
    EXPECT_TRUE(frame[3].is_blanked());

    EXPECT_EQ(frame[4].x, 0.5f);
    EXPECT_EQ(frame[4].y, 0.5f);
    EXPECT_TRUE(frame[4].is_lit());

    EXPECT_EQ(frame[5].x, 0.8f);
    EXPECT_EQ(frame[5].y, 0.8f);
    EXPECT_TRUE(frame[5].is_lit());
}
