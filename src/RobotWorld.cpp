#include "RobotWorld.hpp"

#include "Goal.hpp"
#include "Logger.hpp"
#include "Robot.hpp"
#include "Wall.hpp"
#include "WayPoint.hpp"

#include "CompassSensor.h"

#include <algorithm>
#include <memory>

namespace Model
{
    /**
	 *
	 */
    /* static */ RobotWorld& RobotWorld::RobotWorld::getRobotWorld()
    {
        static RobotWorld robotWorld;
        return robotWorld;
    }
    /**
	 *
	 */
    RobotPtr RobotWorld::newRobot(const std::string& aName /*= "New Robot"*/, const Point& aPosition /*= Point(-1,-1)*/,
                                  bool aNotifyObservers /*= true*/)
    {
        RobotPtr robot(new Robot(aName, aPosition));

        robots.push_back(robot);
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }

        return robot;
    }
    /**
	 *
	 */
    WayPointPtr RobotWorld::newWayPoint(const std::string& aName /*= "new WayPoint"*/,
                                        const Point& aPosition /*= Point(-1,-1)*/, bool aNotifyObservers /*= true*/)
    {
        WayPointPtr wayPoint(new WayPoint(aName, aPosition));
        wayPoints.push_back(wayPoint);
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
        return wayPoint;
    }
    /**
	 *
	 */
    GoalPtr RobotWorld::newGoal(const std::string& aName /*= "New Goal"*/, const Point& aPosition /*= Point(-1,-1)*/,
                                bool aNotifyObservers /*= true*/)
    {
        GoalPtr goal(new Goal(aName, aPosition));
        goals.push_back(goal);
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
        return goal;
    }
    /**
	 *
	 */
    WallPtr RobotWorld::newWall(const Point& aPoint1, const Point& aPoint2, bool aNotifyObservers /*= true*/)
    {
        WallPtr wall(new Wall(aPoint1, aPoint2));
        walls.push_back(wall);
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
        return wall;
    }
    /**
	 *
	 */
    void RobotWorld::deleteRobot(RobotPtr aRobot, bool aNotifyObservers /*= true*/)
    {
        auto i = std::find_if(robots.begin(), robots.end(), [aRobot](RobotPtr r) {
            return aRobot->getName() == r->getName();
        });
        if (i != robots.end())
        {
            robots.erase(i);
            if (aNotifyObservers == true)
            {
                notifyObservers();
            }
        }
    }
    /**
	 *
	 */
    void RobotWorld::deleteWayPoint(WayPointPtr aWayPoint, bool aNotifyObservers /*= true*/)
    {
        auto i = std::find_if(wayPoints.begin(), wayPoints.end(), [aWayPoint](WayPointPtr w) {
            return aWayPoint->getName() == w->getName();
        });
        if (i != wayPoints.end())
        {
            wayPoints.erase(i);
            if (aNotifyObservers == true)
            {
                notifyObservers();
            }
        }
    }
    /**
	 *
	 */
    void RobotWorld::deleteGoal(GoalPtr aGoal, bool aNotifyObservers /*= true*/)
    {
        auto i = std::find_if(goals.begin(), goals.end(), [aGoal](GoalPtr g) {
            return aGoal->getName() == g->getName();
        });
        if (i != goals.end())
        {
            goals.erase(i);

            if (aNotifyObservers == true)
            {
                notifyObservers();
            }
        }
    }
    /**
	 *
	 */
    void RobotWorld::deleteWall(WallPtr aWall, bool aNotifyObservers /*= true*/)
    {
        auto i = std::find_if(walls.begin(), walls.end(), [aWall](WallPtr w) {
            return aWall->getPoint1() == w->getPoint1() && aWall->getPoint2() == w->getPoint2();
        });
        if (i != walls.end())
        {
            walls.erase(i);

            if (aNotifyObservers == true)
            {
                notifyObservers();
            }
        }
    }
    /**
	 *
	 */
    RobotPtr RobotWorld::getRobot(const std::string& aName) const
    {
        if (auto i = std::find_if(robots.begin(),
                                  robots.end(),
                                  [&aName](RobotPtr robot) {
                                      return robot->getName() == aName;
                                  });
            i != robots.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    RobotPtr RobotWorld::getRobot(const Base::ObjectId& anObjectId) const
    {
        if (auto i = std::find_if(robots.begin(),
                                  robots.end(),
                                  [&anObjectId](RobotPtr robot) {
                                      return robot->getObjectId() == anObjectId;
                                  });
            i != robots.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    WayPointPtr RobotWorld::getWayPoint(const std::string& aName) const
    {
        if (auto i = std::find_if(wayPoints.begin(),
                                  wayPoints.end(),
                                  [&aName](WayPointPtr wayPoint) {
                                      return wayPoint->getName() == aName;
                                  });
            i != wayPoints.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    WayPointPtr RobotWorld::getWayPoint(const Base::ObjectId& anObjectId) const
    {
        if (auto i = std::find_if(wayPoints.begin(),
                                  wayPoints.end(),
                                  [&anObjectId](WayPointPtr wayPoint) {
                                      return wayPoint->getObjectId() == anObjectId;
                                  });
            i != wayPoints.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    GoalPtr RobotWorld::getGoal(const std::string& aName) const
    {
        if (auto i = std::find_if(goals.begin(),
                                  goals.end(),
                                  [&aName](GoalPtr goal) {
                                      return goal->getName() == aName;
                                  });
            i != goals.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    GoalPtr RobotWorld::getGoal(const Base::ObjectId& anObjectId) const
    {
        if (auto i = std::find_if(goals.begin(),
                                  goals.end(),
                                  [&anObjectId](GoalPtr goal) {
                                      return goal->getObjectId() == anObjectId;
                                  });
            i != goals.end())
        {
            return *i;
        }
        return nullptr;
    }
    /**
	 *
	 */
    WallPtr RobotWorld::getWall(const Base::ObjectId& anObjectId) const
    {
        if (auto i = std::find_if(walls.begin(),
                                  walls.end(),
                                  [&anObjectId](WallPtr wall) {
                                      return wall->getObjectId() == anObjectId;
                                  });
            i != walls.end())
        {
            return *i;
        }
        return nullptr;
    }

    /**
	 *
	 */
    const std::vector<RobotPtr>& RobotWorld::getRobots() const
    {
        return robots;
    }
    /**
	 *
	 */
    const std::vector<WayPointPtr>& RobotWorld::getWayPoints() const
    {
        return wayPoints;
    }
    /**
	 *
	 */
    const std::vector<GoalPtr>& RobotWorld::getGoals() const
    {
        return goals;
    }
    /**
	 *
	 */
    const std::vector<WallPtr>& RobotWorld::getWalls() const
    {
        return walls;
    }
    /**
	 *
	 */
    void RobotWorld::populate(int aNumberOfWalls /* = 2*/)
    {
        // define world dimensions.

        const uint16_t max = 1024;
        const uint16_t min = 0;

        // set world border coordinates.

        const Point C0(min, min);
        const Point C1(max, min);
        const Point C2(max, max);
        const Point C3(min, max);

        // add world borders.

        RobotWorld::getRobotWorld().newWall(C0, C1, true);
        RobotWorld::getRobotWorld().newWall(C1, C2, true);
        RobotWorld::getRobotWorld().newWall(C2, C3, true);
        RobotWorld::getRobotWorld().newWall(C3, C0, true);

        // set maze wall coordinates.

        const std::vector<std::pair<Point, Point>> wallCoordinates =
                {
                        {Point(min, max/5*1),Point(max/5*3, max/5*1)},
                        {Point(min + max/5*2,max/5*2), Point(max, max/5*2)},
                        {Point(min,max/5*3),Point(max/5*3, max/5*3)},
                        {Point(min + max/5*2,max/5*4), Point(max, max/5*4)},
                };

        // add maze walls.

        for(std::size_t i = 0; i < static_cast<std::size_t>(aNumberOfWalls); ++i)
        {
            RobotWorld::getRobotWorld().newWall(wallCoordinates[i].first, wallCoordinates[i].second, false);
        }

        // add robot and goal.

        const Point robotLocation (min + max/10, min + max/10);
        const Point goalLocation (max - max/10, max - max/10); // actual
//        Point goalLocation = robotLocation; // TODO testing
//        goalLocation.y += max/5*1;


        RobotWorld::getRobotWorld().newRobot("Robot", robotLocation, false);// @suppress("Avoid magic numbers")
        RobotWorld::getRobotWorld().newGoal( "Goal", goalLocation, false); // @suppress("Avoid magic numbers")

        notifyObservers();
    }
    /**
	 *
	 */
    void RobotWorld::unpopulate(bool aNotifyObservers /*= true*/)
    {
        robots.clear();
        wayPoints.clear();
        goals.clear();
        walls.clear();

        if (aNotifyObservers)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void RobotWorld::unpopulate(const std::vector<Base::ObjectId>& aKeepObjects, bool aNotifyObservers /*= true*/)
    {
        if (robots.size() > 0)
        {
            robots.erase(std::remove_if(robots.begin(),
                                        robots.end(),
                                        [&aKeepObjects](RobotPtr aRobot) {
                                            return std::find(aKeepObjects.begin(),
                                                             aKeepObjects.end(),
                                                             aRobot->getObjectId()) == aKeepObjects.end();
                                        }),
                         robots.end());
        }
        if (wayPoints.size() > 0)
        {
            wayPoints.erase(std::remove_if(wayPoints.begin(),
                                           wayPoints.end(),
                                           [&aKeepObjects](WayPointPtr aWayPoint) {
                                               return std::find(aKeepObjects.begin(),
                                                                aKeepObjects.end(),
                                                                aWayPoint->getObjectId()) == aKeepObjects.end();
                                           }),
                            wayPoints.end());
        }
        if (goals.size() > 0)
        {
            goals.erase(std::remove_if(goals.begin(),
                                       goals.end(),
                                       [&aKeepObjects](GoalPtr aGoal) {
                                           return std::find(aKeepObjects.begin(),
                                                            aKeepObjects.end(),
                                                            aGoal->getObjectId()) == aKeepObjects.end();
                                       }),
                        goals.end());
        }
        if (walls.size() > 0)
        {
            walls.erase(std::remove_if(walls.begin(),
                                       walls.end(),
                                       [&aKeepObjects](WallPtr aWall) {
                                           return std::find(aKeepObjects.begin(),
                                                            aKeepObjects.end(),
                                                            aWall->getObjectId()) == aKeepObjects.end();
                                       }),
                        walls.end());
        }

        if (aNotifyObservers)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    std::string RobotWorld::asString() const
    {
        return ModelObject::asString();
    }
    /**
	 *
	 */
    std::string RobotWorld::asDebugString() const
    {
        std::ostringstream os;

        os << asString() << '\n';

        for (RobotPtr ptr : robots)
        {
            os << ptr->asDebugString() << '\n';
        }
        for (WayPointPtr ptr : wayPoints)
        {
            os << ptr->asDebugString() << '\n';
        }
        for (GoalPtr ptr : goals)
        {
            os << ptr->asDebugString() << '\n';
        }
        for (WallPtr ptr : walls)
        {
            os << ptr->asDebugString() << '\n';
        }

        return os.str();
    }
    /**
	 *
	 */
    RobotWorld::RobotWorld()
    {
    }
    /**
	 *
	 */
    RobotWorld::~RobotWorld()
    {
        // No notification while I am in the destruction mode!
        disableNotification();
        unpopulate();
    }

}// namespace Model
