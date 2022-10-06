//
// Created by luke on 06-10-22.
//

#include "ConcreteDrivers.h"
#include "Logger.hpp"
#include "Robot.hpp"

namespace Model
{
    /* ================================================ */
    /*                  DEFAULT DRIVER                  */
    /* ================================================ */
    void DefaultDriver::steer(GoalPtr goal, PathAlgorithm::Path path, uint32_t& pathPoint)
    {
        const PathAlgorithm::Vertex& vertex = path[pathPoint += static_cast<unsigned int>(getAsRobot()->getSpeed())];

        Point newPosition = vertex.asPoint();

        SteeringCommand command(newPosition);
        actuator->handleCommand(command);

        getAsRobot()->setFront(BoundedVector(command.positionRequest, getAsRobot()->getPosition()));
        getAsRobot()->setPosition(command.positionRequest);

        if (checkCurrentPosition(goal))
        {
            LOG("Arrived at goal location or collision with world objects");
            getAsRobot()->notifyObservers();
            stop();
            return;
        }

        notifyObservers();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));// @suppress("Avoid magic numbers")
    }
    std::string DefaultDriver::asString() const
    {
        return "DefaultDriver";
    }
    std::string DefaultDriver::asDebugString() const
    {
        return asString();
    }
    /* ================================================ */
    /*                  KALMAN DRIVER                   */
    /* ================================================ */
    void KalmanDriver::steer(GoalPtr goal, PathAlgorithm::Path path, uint32_t& pathPoint)

    {
        // TODO
    }
    std::string KalmanDriver::asString() const
    {
        return "KalmanDriver";
    }
    std::string KalmanDriver::asDebugString() const
    {
        return asString();
    }
    /* ================================================ */
    /*                  PARTICLE DRIVER                 */
    /* ================================================ */
    void ParticleDriver::steer(GoalPtr goal, PathAlgorithm::Path path, uint32_t& pathPoint)
    {
        // TODO
    }
    std::string ParticleDriver::asString() const
    {
        return "ParticleDriver";
    }
    std::string ParticleDriver::asDebugString() const
    {
        return asString();
    }
}// namespace Model