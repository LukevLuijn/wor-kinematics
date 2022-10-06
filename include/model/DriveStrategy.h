//
// Created by luke on 06-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H
#define WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H

#include "AbstractAgent.hpp"
#include "AbstractActuator.hpp"
#include "Goal.hpp"
#include "AStar.hpp"

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class DriveStrategy : public ModelObject
    {
    public:

        DriveStrategy();
        DriveStrategy(Robot* robot);

        virtual ~DriveStrategy() = default;

        void drive(const GoalPtr& goal, const PathAlgorithm::Path& path);
        void stop();

    protected:
        virtual void steer(GoalPtr aGoal, PathAlgorithm::Path path, uint32_t& pathPoint) = 0;
        bool checkCurrentPosition(GoalPtr aGoal);
        Robot* getAsRobot();

    protected:
        AbstractAgent* agent;
        AbstractActuator* actuator;
    private:
        bool isDriving;

    };
}

#endif//WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H
