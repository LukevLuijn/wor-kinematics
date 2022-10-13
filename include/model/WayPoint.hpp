#ifndef WAYPOINT_HPP_
#define WAYPOINT_HPP_

#include "Config.hpp"

#include "ModelObject.hpp"
#include "Point.hpp"
#include "Region.hpp"
#include "Size.hpp"

namespace Model
{
    class WayPoint;
    typedef std::shared_ptr<WayPoint> WayPointPtr;

    class WayPoint : public ModelObject
    {
    public:
        /**
			 *
			 */
        WayPoint();
        /**
			 *
			 */
        explicit WayPoint(const std::string& aName);
        /**
			 *
			 */
        WayPoint(const std::string& aName, const Point& aPosition);
        /**
			 *
			 */
        virtual ~WayPoint();
        /**
			 *
			 */
        std::string getName() const
        {
            return name;
        }
        /**
			 *
			 */
        void setName(const std::string& aName, bool aNotifyObservers = true);
        /**
			 *
			 */
        Size getSize() const;
        /**
			 *
			 */
        void setSize(const Size& aSize, bool aNotifyObservers = true);
        /**
			 *
			 */
        Point getPosition() const
        {
            return position;
        }
        /**
			 *
			 */
        void setPosition(const Point& aPosition, bool aNotifyObservers = true);
        /**
			 *
			 */
        Region getRegion() const;
        /**
			 *
			 */
        bool intersects(const Region& aRegion) const;
        /**
			 * @name Debug functions
			 */
        //@{
        /**
			 * Returns a 1-line description of the object
			 */
        virtual std::string asString() const override;
        /**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
        virtual std::string asDebugString() const override;
        //@}
    protected:
    private:
        std::string name;
        Size size;
        Point position;
    };
}// namespace Model
#endif// WAYPOINT_HPP_
