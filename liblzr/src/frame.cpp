

#include <vector>
#include <lzr.h>


/*
 * Helper functions
 */

void Frame::add()
{
    push_back(Point());
}

void Frame::add(const Point& p)
{
    push_back(Point(p));
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
