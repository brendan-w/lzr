
#include <vector>
#include <liblzr.hpp>

namespace lzr {


/*
 * constructors
 */

Frame::Frame() : std::vector<Point>() { }
Frame::Frame(size_t n) : std::vector<Point>(n) { }



/*
 * functions
 */


Frame& Frame::add(const Point& p)
{
    push_back(Point(p));
    return *this;
}


Frame& Frame::add(const Frame& other)
{
    for(Point p : other)
    {
        push_back(Point(p));
    }

    return *this;
}


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


Frame& Frame::add_blank_jump_to(const Frame& other)
{
    if(other.size() > 0)
        add_blank_jump_to(other.front());

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
    add_blank_jump_to(other);
    add(other);
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


} // namespace lzr
