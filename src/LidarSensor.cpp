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
    std::shared_ptr<AbstractStimulus> LidarSensor::getStimulus() const
    {
        auto takeReading = [](const Point& location, double angleInRads) -> LidarReading
        {
          double minimalMeasuredDistance = 1450; //std::numeric_limits<double>::max();
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
                  double measuredDistance = std::sqrt(std::pow((location.x - beamIntersection.x), 2) +
                                                      std::pow((location.y - beamIntersection.y), 2));
                  minimalMeasuredDistance = std::min(minimalMeasuredDistance, measuredDistance);
              }
          }

          return LidarReading{Utils::MathUtils::toDegrees(angleInRads), minimalMeasuredDistance};
        };

        double currentAngle = Utils::Shape2DUtils::getAngle(dynamic_cast<Robot*>(agent)->getFront());
        Point currentPosition = dynamic_cast<Robot*>(agent)->getPosition();

        LidarData data;

        for(uint8_t i = 0; i < 180; ++i)
        {
            double angleInRads = Utils::MathUtils::toRadians( (currentAngle + (2 * i)) );
            LidarReading reading = takeReading(currentPosition, angleInRads);

            data[i] = reading;
        }
        return std::shared_ptr<AbstractStimulus> (new LidarStimulus(data));
    }
    std::shared_ptr<AbstractPercept> LidarSensor::getPerceptFor(std::shared_ptr<AbstractStimulus> aStimulus)
    {
        auto* stimulus = dynamic_cast<LidarStimulus*>(aStimulus.get());

//        for(LidarReading& reading : stimulus->measurements)
//        {
//            reading.distance = noisify(reading.distance);
//        }

        return std::shared_ptr<AbstractPercept>(new LidarPercept(stimulus->measurements));
    }
    std::string LidarSensor::asString() const
    {
        return "LidarSensor";
    }
    std::string LidarSensor::asDebugString() const
    {
        return asString();
    }
}// namespace Model