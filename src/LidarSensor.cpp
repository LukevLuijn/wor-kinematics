//
// Created by luke on 05-10-22.
//

#include "LidarSensor.h"
#include "MathUtils.hpp"
#include "Robot.hpp"
#include "Shape2DUtils.hpp"

#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Widgets.hpp"

namespace Model
{
    LidarSensor::LidarSensor() : NoisySensor(0, 10)
    {
    }
    LidarSensor::LidarSensor(Robot* robot) : AbstractSensor(robot), NoisySensor(0, 10)
    {
    }
    AbstractStimulusPtr LidarSensor::getStimulus() const
    {
        double currentAngle = Utils::Shape2DUtils::getAngle(dynamic_cast<Robot*>(agent)->getFront());
        Point currentPosition = dynamic_cast<Robot*>(agent)->getPosition();

        LidarData data = getReadingFromLocation(currentPosition, currentAngle);

        return AbstractStimulusPtr(new LidarStimulus(data));
    }
    AbstractPerceptPtr LidarSensor::getPerceptFor(AbstractStimulusPtr aStimulus)
    {
        auto* stimulus = dynamic_cast<LidarStimulus*>(aStimulus.get());

        for (LidarReading& reading : stimulus->measurements)
        {
            reading.distance = noisify(reading.distance);
        }

        return AbstractPerceptPtr(new LidarPercept(*stimulus));
    }
    std::string LidarSensor::asString() const
    {
        return "LidarSensor";
    }
    std::string LidarSensor::asDebugString() const
    {
        return asString();
    }
    LidarData LidarSensor::getReadingFromLocation(const Point& location, double angle)
    {
        LidarData data;

        for (uint8_t i = 0; i < 180; ++i)
        {
            double angleInRads = Utils::MathUtils::toRadians((angle + (2 * i)));
            double minimalMeasuredDistance = 1500;//std::numeric_limits<double>::max();

            const Point BEAM_END_POINT(
                    static_cast<int32_t>(location.x + minimalMeasuredDistance * std::cos(angleInRads)),
                    static_cast<int32_t>(location.y + minimalMeasuredDistance * std::sin(angleInRads)));


            for (const WallPtr& wall : Model::RobotWorld::getRobotWorld().getWalls())
            {
                Point beamIntersection = Utils::Shape2DUtils::getIntersection(location,
                                                                              BEAM_END_POINT,
                                                                              wall->getPoint1(),
                                                                              wall->getPoint2());
                if (beamIntersection != DefaultPosition)
                {
                    double measuredDistanceX = std::pow(location.x - beamIntersection.x, 2);
                    double measuredDistanceY = std::pow(location.y - beamIntersection.y, 2);
                    double measuredDistance = std::sqrt(measuredDistanceX + measuredDistanceY);

                    minimalMeasuredDistance = std::min(minimalMeasuredDistance, measuredDistance);
                }
            }
            data[i] = LidarReading{Utils::MathUtils::toDegrees(angleInRads), minimalMeasuredDistance};
        }
        return data;
    }

}// namespace Model