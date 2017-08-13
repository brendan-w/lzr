
#include <vector>
#include <liblzr.hpp>

namespace lzr {


/*
 * functions
 */

Frame& Frame::add(const Point& p)
{
    push_back(p);
    return *this;
}


Frame& Frame::add(const Frame& other)
{
    insert(end(), other.begin(), other.end());
    return *this;
}

// private utility for creating a blanked segment
Frame& Frame::add_blank_jump_to(const Point& p)
{
    //only works when we already have a point in the frame
    if(!empty())
    {
        Point begin = back();
        Point end = p;

        begin.blank();
        end.blank();

        add(begin);
        add(end);
    }

    return *this;
}

Frame& Frame::add_with_blank_jump(const Point& p)
{
    add_blank_jump_to(p);
    add(p);
    return *this;
}

Frame& Frame::add_with_blank_jump(const Frame& other)
{
    if(other.size() > 0)
    {
        add_blank_jump_to(other.front());
        add(other);
    }

    return *this;
}

void Frame::bounding_box(Point& min, Point& max) const
{
    // initialize edges at opposite extremes
    min.x = 1.0;
    min.y = -1.0;
    max.x = 1.0;
    max.y = -1.0;

    for(const Point& p : *this)
    {
        if(p.x < min.x) min.x = p.x; // left
        if(p.x > min.y) min.y = p.x; // right
        if(p.y < max.x) max.x = p.y; // bottom
        if(p.y > max.y) max.y = p.y; // top
    }
}

Point Frame::bounding_box_center() const
{
    Point min;
    Point max;
    bounding_box(min, max);

    Point center;
    center.x = (min.x + max.x) / 2.0;
    center.y = (min.y + max.y) / 2.0;

    return center;
}


Point Frame::average_center() const
{
    Point center;
    center.x = 0.0;
    center.y = 0.0;

    for(const Point& p : *this)
    {
        center.x += p.x;
        center.y += p.y;
    }

    center.x /= (double) size();
    center.y /= (double) size();

    return center;
}

FrameList split_frame(const Frame& frame)
{
    FrameList paths;

    bool was_lit = false;
    Frame path;

    for(const Point& p : frame)
    {
        if(p.is_lit()) //if the laser just turned on
        {
            path.add(p);
        }
        else if(p.is_blanked() && was_lit) //if the laser just turned off
        {
            paths.push_back(path);
            path.clear();
        }

        was_lit = p.is_lit();
    }

    //if a path was left open, close it
    if(was_lit)
    {
        paths.push_back(path);
        path.clear();
    }

    return paths;
}

Frame combine_frames(const FrameList& frames)
{
    Frame frame;
    for (const Frame& f : frames)
    {
        frame.add_with_blank_jump(f);
    }
    return frame;
}


} // namespace lzr
