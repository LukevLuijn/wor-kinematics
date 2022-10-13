#ifndef GOAL_HPP_
#define GOAL_HPP_

#include "Config.hpp"

#include "WayPoint.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace Model
{
    class Goal;
    typedef std::shared_ptr<Goal> GoalPtr;

    class Goal : public WayPoint
    {
    public:
        /**
			 *
			 */
        Goal();
        /**
			 *
			 */
        explicit Goal(const std::string& aName);
        /**
			 *
			 */
        Goal(const std::string& aName, const Point& aPosition);
        /**
			 *
			 */
        virtual ~Goal();
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
    };
}// namespace Model
#endif// GOAL_HPP_
