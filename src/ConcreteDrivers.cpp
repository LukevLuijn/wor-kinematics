//
// Created by luke on 06-10-22.
//

#include "ConcreteDrivers.h"

#include "Logger.hpp"
#include "Robot.hpp"
#include <memory>

#include "CompassSensor.h"
#include "LidarSensor.h"
#include "OdometerSensor.h"

#include "MathUtils.hpp"

namespace Model
{
    /* ================================================ */
    /*                  DEFAULT DRIVER                  */
    /* ================================================ */
    void DefaultDriver::start()
    {
        LOG("Starting the default driver");
    }
    void DefaultDriver::drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                              Base::Queue<std::shared_ptr<AbstractPercept>>& queue)
    {
        const PathAlgorithm::Vertex& vertex = path[pathPoint += static_cast<unsigned int>(getRobotPtr()->getSpeed())];

        Point newPosition = vertex.asPoint();

        RelativeMovementCommand command(newPosition);
        actuator->handleCommand(command);

        getRobotPtr()->setFront(BoundedVector(command.relativePositionRequest, getRobotPtr()->getPosition()));
        getRobotPtr()->setPosition(command.relativePositionRequest);

        while (queue.size() != 0)
        {
            // empty queue because unused.
            UNUSEDCAST(queue.dequeue());
        }
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
    void KalmanDriver::start()
    {
        LOG("Starting the kalman driver");

        Point position = getRobotPtr()->getPosition();

        belief = Utils::Matrix<double, 2, 1>{static_cast<double>(position.x), static_cast<double>(position.y)};
        error = Utils::Matrix<double, 2, 2>{{100, 0}, {0, 100}};
    }
    void KalmanDriver::drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                             Base::Queue<std::shared_ptr<AbstractPercept>>& queue)
    {
        const PathAlgorithm::Vertex& vertex = path[pathPoint += static_cast<uint32_t>(getRobotPtr()->getSpeed())];

        const Utils::Matrix<double, 2, 1> target = {static_cast<double>(vertex.x), static_cast<double>(vertex.y)};

        // control vector
        const Utils::Matrix<double, 2, 1> control = {static_cast<double>(vertex.x - belief[0][0]),
                                                     static_cast<double>(vertex.y - belief[1][0])};

        LOG("BELIEF", belief.to_string());
        LOG("TARGET", target.to_string());

        // predicted state vector
        const Utils::Matrix<double, 2, 1> psv = A * belief + B * control + PPNV;
        // Predicted process covariance matrix
        const Utils::Matrix<double, 2, 2> ppcm = A * error * A.transpose() + PNCM;
        // kalman gain
//        const Utils::Matrix<double, 2, 2> kg = (ppcm * C.transpose()) * (C * ppcm * C.transpose() + SCM).inverse();
        const Utils::Matrix<double, 2, 2> kg = (ppcm) * (ppcm + SCM).inverse();

        // update robot
        RelativeMovementCommand command(
                Point(static_cast<int32_t>(control[0][0]), static_cast<int32_t>(control[1][0])));
        actuator->handleCommand(command);

        getRobotPtr()->setFront(BoundedVector(command.relativePositionRequest, getRobotPtr()->getPosition()));
        getRobotPtr()->setPosition(command.relativePositionRequest);


        // perform measurement
        sortPercepts(queue);

        if (!odometerPercepts.empty())
        {
            believedDistanceTravelled = odometerPercepts.back()->distanceTravelled;
        }
        else
        {
            LOG("NO odometer percept");
        }
        if (!compassPercepts.empty())
        {
            believedCurrentOrientation = compassPercepts.back()->orientation;
        }
        else
        {
            LOG("NO compass percept");
        };

        // Measured movement
        const Utils::Matrix<double, 2, 1> msv = {
                believedDistanceTravelled * -std::cos(believedCurrentOrientation),
                believedDistanceTravelled * -std::sin(believedCurrentOrientation)};

        // new state vector
        const Utils::Matrix<double, 2, 1> nsv = belief + msv;
        // Measurement vector
        const Utils::Matrix<double, 2, 1> mv = C * nsv + MN;

        // adjusted state vector
        belief = psv + kg * (mv - (C * psv));
        // adjusted process covariance matrix
        error = (kg.identity() - kg) * ppcm;
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
    void ParticleDriver::drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                               Base::Queue<std::shared_ptr<AbstractPercept>>& queue)
    {
        UNUSEDCAST(path);
        UNUSEDCAST(pathPoint);
        UNUSEDCAST(queue);

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