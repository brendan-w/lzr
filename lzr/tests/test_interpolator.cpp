#include "gtest/gtest.h"

#include "lzr/lzr.hpp"

using namespace lzr;

TEST(Interpolator, XYRGBLine)
{
    Frame orig;
    Frame frame;
    Frame target;

    //a diagonal line
    orig.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    orig.add(Point( 1.0, 1.0, 200, 200, 200, 200));

    //max_distance = 0.75 -----------------------------
    target.clear();
    target.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    target.add(Point(-0.6, -0.6, 120, 120, 120, 120));
    target.add(Point(-0.2, -0.2, 140, 140, 140, 140));
    target.add(Point( 0.2,  0.2, 160, 160, 160, 160));
    target.add(Point( 0.6,  0.6, 180, 180, 180, 180));
    target.add(Point( 1.0,  1.0, 200, 200, 200, 200));

    frame = orig;
    interpolate(frame, 0.75, linear);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);


    //max_distance = 1.0 -----------------------------
    target.clear();
    target.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    target.add(Point(-0.5, -0.5, 125, 125, 125, 125));
    target.add(Point( 0.0,  0.0, 150, 150, 150, 150));
    target.add(Point( 0.5,  0.5, 175, 175, 175, 175));
    target.add(Point( 1.0,  1.0, 200, 200, 200, 200));

    frame = orig;
    interpolate(frame, 1.0, linear);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);
}

TEST(Interpolator, InterpFunc)
{
    Frame orig;
    Frame frame;
    Frame target;

    // each interpolation function will run on this input
    orig.add(Point(-1.0, -1.0, 255, 255, 255, 255));
    orig.add(Point( 1.0, 1.0, 255, 255, 255, 255));

    // LINEAR ----------------------------------------------
    target.clear();
    target.add(Point(-1.0, -1.0, 255, 255, 255, 255));
    target.add(Point(-0.5, -0.5, 255, 255, 255, 255));
    target.add(Point( 0.0,  0.0, 255, 255, 255, 255));
    target.add(Point( 0.5,  0.5, 255, 255, 255, 255));
    target.add(Point( 1.0,  1.0, 255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, linear);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);

    // QUAD --------------------------------------------------
    target.clear();
    target.add(Point(-1.0,  -1.0,  255, 255, 255, 255));
    target.add(Point(-0.75, -0.75, 255, 255, 255, 255));
    target.add(Point( 0.0,   0.0,  255, 255, 255, 255));
    target.add(Point( 0.75,  0.75, 255, 255, 255, 255));
    target.add(Point( 1.0,   1.0,  255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, quad);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);

    // QUART --------------------------------------------------
    target.clear();
    target.add(Point(-1.0,    -1.0,    255, 255, 255, 255));
    target.add(Point(-0.9375, -0.9375, 255, 255, 255, 255));
    target.add(Point( 0.0,     0.0,    255, 255, 255, 255));
    target.add(Point( 0.9375,  0.9375, 255, 255, 255, 255));
    target.add(Point( 1.0,     1.0,    255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, quart);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);
}

TEST(Interpolator, SkipBlankJumps)
{
    Frame orig;
    Frame frame;
    Frame target;

    //a diagonal line
    orig.add(Point(-1.0, 0.0, 255, 255, 255, 255));
    orig.add(Point(-0.5, 0.0, 255, 255, 255, 255));
    orig.add(Point(-0.5, 0.0, 0  , 0  , 0  , 0  ));
    orig.add(Point( 0.5, 0.0, 0  , 0  , 0  , 0  ));
    orig.add(Point( 0.5, 0.0, 255, 255, 255, 255));
    orig.add(Point( 1.0, 0.0, 255, 255, 255, 255));

    // ----------------------------------------------------
    target.clear();
    target.add(Point(-1.0,   0.0, 255, 255, 255, 255));
    target.add(Point(-0.875, 0.0, 255, 255, 255, 255));
    target.add(Point(-0.75,  0.0, 255, 255, 255, 255));
    target.add(Point(-0.625, 0.0, 255, 255, 255, 255));
    target.add(Point(-0.5,   0.0, 255, 255, 255, 255));
    target.add(Point(-0.5,   0.0, 0  , 0  , 0  , 0  ));
    target.add(Point( 0.5,   0.0, 0  , 0  , 0  , 0  ));
    target.add(Point( 0.5,   0.0, 255, 255, 255, 255));
    target.add(Point( 0.625, 0.0, 255, 255, 255, 255));
    target.add(Point( 0.75,  0.0, 255, 255, 255, 255));
    target.add(Point( 0.875, 0.0, 255, 255, 255, 255));
    target.add(Point( 1.0,   0.0, 255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 0.2, linear);
    // print_frame(frame);
    EXPECT_TRUE(frame == target);
}
