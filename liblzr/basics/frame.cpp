

#include <vector>
#include <lzr.h>


/*
 * Constructors
 */

Frame::Frame()
{

}

Frame::~Frame()
{
    clear();
}



/*
 * Getters
 */

size_t Frame::size()
{
    return points.size();
}

Point& Frame::operator[](size_t i)
{
    return *points[i];
}

const std::vector<Point*>::iterator Frame::begin()
{
    return points.begin();
}

const std::vector<Point*>::iterator Frame::end()
{
    return points.end();
}




/*
 * Modifiers
 */

void Frame::add()
{
    points.push_back(new Point());
}

void Frame::add(const Point& p)
{
    points.push_back(new Point(p));
}

void Frame::remove(size_t i)
{
    points.erase(points.begin() + i);
}

void Frame::insert(const Point& p, size_t i)
{
    if(i > points.size())
        i = points.size();

    points.insert(points.begin() + i, new Point(p));
}

void Frame::clear()
{
    //dealocate everything
    for(Point* p : points)
    {
        delete p;
    }

    points.clear();
}
