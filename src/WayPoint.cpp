#include "WayPoint.hpp"

#include "Logger.hpp"

#include <sstream>

namespace Model
{
    /**
	 *
	 */
    WayPoint::WayPoint() : name("")
    {
    }
    /**
	 *
	 */
    WayPoint::WayPoint(const std::string& aName) : name(aName)
    {
    }
    /**
	 *
	 */
    WayPoint::WayPoint(const std::string& aName, const Point& aPosition) : name(aName), position(aPosition)
    {
    }
    /**
	 *
	 */
    WayPoint::~WayPoint()
    {
    }
    /**
	 *
	 */
    void WayPoint::setName(const std::string& aName, bool aNotifyObservers /*= true*/)
    {
        name = aName;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    Size WayPoint::getSize() const
    {
        return size;
    }
    /**
	 *
	 */
    void WayPoint::setSize(const Size& aSize, bool aNotifyObservers /*= true*/)
    {
        size = aSize;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void WayPoint::setPosition(const Point& aPosition, bool aNotifyObservers /*= true*/)
    {
        position = aPosition;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    Region WayPoint::getRegion() const
    {
        // x and y are pointing to top left now
        int x = position.x - (size.x / 2);
        int y = position.y - (size.y / 2);

        Point originalUpperLeft(x, y);
        Point originalUpperRight(x + size.x, y);
        Point originalBottomLeft(x, y + size.y);
        Point originalBottomRight(x + size.x, y + size.y);

        Point originalPoints[] = {originalUpperRight, originalUpperLeft, originalBottomLeft, originalBottomRight};

        return Region(4, originalPoints);
    }
    /**
	 *
	 */
    bool WayPoint::intersects(const Region& aRegion) const
    {
        return getRegion().Intersect(aRegion);
    }
    /**
	 *
	 */
    std::string WayPoint::asString() const
    {
        std::ostringstream os;

        os << "WayPoint " << name << " at (" << position.x << "," << position.y << ")";

        return os.str();
    }
    /**
	 *
	 */
    std::string WayPoint::asDebugString() const
    {
        std::ostringstream os;

        os << "WayPoint:\n";
        os << name << " at (" << position.x << "," << position.y << ")";

        return os.str();
    }
}// namespace Model
