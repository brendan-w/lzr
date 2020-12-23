#include "gtest/gtest.h"

#include "lzr/lzr.hpp"

using namespace lzr;

TEST(Interpolator, BlankingInterpolation)
{
    Frame frame;
    Frame target;

    //two lone points, with a blank jump in between
    frame.add(                Point(0.5, 0.0, 255, 255, 255, 255));
    frame.add_with_blank_jump(Point(1.0, 0.0, 255, 255, 255, 255));

    target = frame;

    Optimizer opt;
    opt.run(frame);

    //make sure that the right points are blanked
    for(int i = 0; i <= 5; i++)
    {
        EXPECT_TRUE(frame[i].is_blanked());
    }

    EXPECT_TRUE(frame[6].is_lit());

    for(int i = 7; i <= 12; i++)
    {
       EXPECT_TRUE(frame[i].is_blanked());
    }

    EXPECT_TRUE(frame[13].is_lit());

    //check positioning
    EXPECT_TRUE(frame[0] == Point(0.0, 0.0, 0, 0, 0, 0));
    EXPECT_TRUE(frame[5].same_position_as(target[0]));
    EXPECT_TRUE(frame[7].same_position_as(target[0]));
    EXPECT_TRUE(frame[12].same_position_as(target[3]));
}


TEST(Interpolator, RasterZigZag)
{
    Frame frame;
    Frame target;

    //construct a row of vertical lines

    Frame line;
    //the base line
    line.add(Point(-1.0, 1.0, 255, 255, 255, 255));
    line.add(Point(1.0,  1.0, 255, 255, 255, 255));

    for(int i = 0; i < 5; i++)
    {
        frame.add_with_blank_jump(line);
        translate(line, 0.0, -0.5);
    }

    Optimizer opt(Point(-1.0, 1.0, 255, 255, 255, 255));
    opt.run(frame);
}
