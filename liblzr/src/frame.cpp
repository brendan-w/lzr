

#include <vector>
#include <lzr.h>


/*
 * functions
 */

Frame& Frame::operator+=(const Point& p)
{
    push_back(Point(p));
    return *this;
}

Frame& Frame::operator+=(const Frame& other)
{
    for(Point p : other)
    {
        push_back(Point(p));
    }

    return *this;
}

bool Frame::operator==(const Frame& other)
{
    if(size() != other.size())
        return false;

    for(size_t i = 0; i < size(); i++)
    {
        if(this->at(i) != other[i])
            return false;
    }

    return true;
}
