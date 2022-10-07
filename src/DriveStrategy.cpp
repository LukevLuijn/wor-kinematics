//
// Created by luke on 06-10-22.
//

#include "DriveStrategy.h"

#include "AbstractSensor.hpp"
#include "Logger.hpp"
#include "Robot.hpp"

#include "LidarSensor.h"
#include "OdometerSensor.h"
#include "CompassSensor.h"

namespace Model
{
    DriveStrategy::DriveStrategy()
    {
    }
    DriveStrategy::DriveStrategy(Robot* robot) : agent(robot), actuator(new SteeringActuator(robot))
    {
    }
    Robot* DriveStrategy::getRobotPtr()
    {
        return dynamic_cast<Robot*>(agent);
    }
    void DriveStrategy::sortPercepts(Base::Queue<std::shared_ptr<AbstractPercept>>& queue)
    {
//        compassPercepts.clear();
//        odometerPercepts.clear();
//        lidarPercepts.clear();

        while (queue.size() != 0)
        {
            std::shared_ptr<AbstractPercept> percept = queue.dequeue().value();

            if (percept->asString() == "CompassPercept")
            {
                auto* compassPercept = castPercept<CompassPercept>(percept);
                compassPercepts.emplace_back(compassPercept);
            }
            else if (percept->asString() == "OdometerPercept")
            {
                auto* odometerPercept = castPercept<OdometerPercept>(percept);
                odometerPercepts.emplace_back(odometerPercept);
            }
            else // if (percept->asString() == "OdometerPercept")
            {
                auto* lidarPercept = castPercept<LidarPercept>(percept);
                lidarPercepts.emplace_back(lidarPercept);
            }
        }
    }


}// namespace Model