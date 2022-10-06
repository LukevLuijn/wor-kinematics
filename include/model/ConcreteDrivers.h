//
// Created by luke on 06-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H
#define WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H

#include "DriveStrategy.h"

namespace Model
{
    class DefaultDriver : public DriveStrategy
    {
    public:
        DefaultDriver() = default;
        explicit DefaultDriver(Robot* robot) : DriveStrategy(robot) {}
        ~DefaultDriver() override = default;

        std::string asString() const override;
        std::string asDebugString() const override;
    private:
        void steer(GoalPtr aGoal, PathAlgorithm::Path path, uint32_t& pathPoint) override;
    };
    class KalmanDriver : public DriveStrategy
    {
    public:
        KalmanDriver() = default;
        explicit KalmanDriver(Robot* robot)  : DriveStrategy(robot) {}
        ~KalmanDriver() override = default;

        std::string asString() const override;
        std::string asDebugString() const override;
    private:
        void steer(GoalPtr aGoal, PathAlgorithm::Path path, uint32_t& pathPoint) override;
    };
    class ParticleDriver : public DriveStrategy
    {
    public:
        ParticleDriver() = default;
        explicit ParticleDriver(Robot* robot)  : DriveStrategy(robot) {}
        ~ParticleDriver() override = default;

        std::string asString() const override;
        std::string asDebugString() const override;
    private:
        void steer(GoalPtr aGoal, PathAlgorithm::Path path, uint32_t& pathPoint) override;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H
