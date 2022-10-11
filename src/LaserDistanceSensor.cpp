#include "LaserDistanceSensor.hpp"

#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "Widgets.hpp"

namespace Model
{

    /**
	 *
	 */
    LaserDistanceSensor::LaserDistanceSensor()
    {
    }
    /**
	 *
	 */
    LaserDistanceSensor::LaserDistanceSensor(Robot* aRobot) : AbstractSensor(aRobot)
    {
    }
    /**
	 *
	 */
    LaserDistanceSensor::~LaserDistanceSensor()
    {
    }
    /**
	 *
	 */
    std::shared_ptr<AbstractStimulus> LaserDistanceSensor::getStimulus() const
    {
        std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();

        for (std::shared_ptr< Wall > wall : walls)
        {
            Point wallPoint1 = wall->getPoint1();
            Point wallPoint2 = wall->getPoint2();

            Robot* robot = dynamic_cast<Robot*>(agent);
            if(robot)
            {
                Point robotLocation = robot->getPosition();

                double angle = Utils::Shape2DUtils::getAngle( robot->getFront()) + 0.5 * Utils::PI;

                Point laserEndpoint{ static_cast<int>(robotLocation.x + std::cos( angle - 0.5 * Utils::PI) * 1024),
                                     static_cast<int>(robotLocation.y + std::sin( angle - 0.5 * Utils::PI) * 1024)};

                Point interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, robotLocation, laserEndpoint);

                if(interSection != DefaultPosition)
                {
                    double distance = Utils::Shape2DUtils::distance(robotLocation,interSection);
                    return std::shared_ptr< AbstractStimulus >( new DistanceStimulus( angle,distance));
                }
            }
        }

        std::shared_ptr< AbstractStimulus > distanceStimulus( new DistanceStimulus( 666,666));
        return distanceStimulus;
    }
    /**
	 *
	 */
    std::shared_ptr<AbstractPercept>
    LaserDistanceSensor::getPerceptFor(std::shared_ptr<AbstractStimulus> anAbstractStimulus)
    {
        auto* distanceStimulus = dynamic_cast<DistanceStimulus*>(anAbstractStimulus.get());
        if (distanceStimulus)
        {
            return AbstractPerceptPtr(new DistancePercept(distanceStimulus->angle, distanceStimulus->distance));
        }
        return std::shared_ptr<AbstractPercept>(new DistancePercept(666, 666));
    }
    /**
	 *
	 */
    std::string LaserDistanceSensor::asString() const
    {
        return "LaserDistanceSensor";
    }
    /**
	 *
	 */
    std::string LaserDistanceSensor::asDebugString() const
    {
        return asString();
    }
}// namespace Model
