//
// Created by luke on 06-10-22.
//

#include "DriveStrategy.h"

#include <utility>
#include "Robot.hpp"
#include "Logger.hpp"

namespace Model
{
    DriveStrategy::DriveStrategy() : isDriving(false)
    {
    }
    DriveStrategy::DriveStrategy(Robot* robot) : agent(robot), actuator(new SteeringActuator(robot)), isDriving(false)
    {
    }
    void DriveStrategy::drive(const GoalPtr& goal, const PathAlgorithm::Path& path)
    {
        getAsRobot()->activateSensors(true);

        uint32_t pathPoint = 0;

        isDriving = true;
        while(isDriving)
        {
            steer(goal, path, pathPoint);

            if (getAsRobot()->outOfBounds(pathPoint))
            {
                LOG("robot out of bounds");
                break;
            }
        }

        getAsRobot()->activateSensors(false);
    }
    void DriveStrategy::stop()
    {
        isDriving = false;
    }
    bool DriveStrategy::checkCurrentPosition(GoalPtr aGoal)
    {
        return getAsRobot()->arrived(std::move(aGoal)) || getAsRobot()->collision();
    }
    Robot* DriveStrategy::getAsRobot()
    {
        return dynamic_cast<Robot*>(agent);
    }

}// namespace Model