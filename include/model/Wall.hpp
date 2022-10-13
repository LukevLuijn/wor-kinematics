#ifndef WALL_HPP_
#define WALL_HPP_

#include "Config.hpp"

#include "ModelObject.hpp"
#include "Point.hpp"

namespace Model
{
    class Wall;
    typedef std::shared_ptr<Wall> WallPtr;

    class Wall : public ModelObject
    {
    public:
        /**
			 *
			 */
        Wall();
        /**
			 *
			 */
        Wall(const Point& aPoint1, const Point& aPoint2);
        /**
			 *
			 */
        virtual ~Wall();
        /**
			 *
			 */
        Point getPoint1() const
        {
            return point1;
        }
        /**
			 *
			 */
        void setPoint1(const Point& aPoint1, bool aNotifyObservers = true);
        /**
			 *
			 */
        Point getPoint2() const
        {
            return point2;
        }
        /**
			 *
			 */
        void setPoint2(const Point& aPoint2, bool aNotifyObservers = true);
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
        Point point1;
        Point point2;
    };
}// namespace Model
#endif// WALL_HPP_
