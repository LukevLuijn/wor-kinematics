//
// Created by luke on 06-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H
#define WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H

#include "DriveStrategy.h"
#include "Matrix.hpp"

namespace Model
{
    class CompassSensor;
    typedef std::shared_ptr<CompassSensor> CompassPtr;

    class LidarSensor;
    typedef std::shared_ptr<LidarSensor> LidarPtr;

    class OdometerSensor;
    typedef std::shared_ptr<OdometerSensor> OdometerPtr;


    class DefaultDriver : public DriveStrategy
    {
    public:
        DefaultDriver() = default;
        explicit DefaultDriver(Robot* robot) : DriveStrategy(robot)
        {
        }
        ~DefaultDriver() override = default;

        void start() override;
        void drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                   Base::Queue<std::shared_ptr<AbstractPercept>>& queue) override;

        std::string asString() const override;
        std::string asDebugString() const override;
    };
    class KalmanDriver : public DriveStrategy
    {
    public:
        KalmanDriver() = default;
        explicit KalmanDriver(Robot* robot) : DriveStrategy(robot)
        {
        }
        ~KalmanDriver() override = default;

        void start() override;
        virtual void drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                           Base::Queue<std::shared_ptr<AbstractPercept>>& queue) override;

        std::string asString() const override;
        std::string asDebugString() const override;

    private:
        double believedDistanceTravelled;
        double believedCurrentOrientation;

        Utils::Matrix<double, 2, 1> belief;
        Utils::Matrix<double, 2, 2> error;

        // process noise covariance matrix
        const Utils::Matrix<double, 2, 2> PNCM = {{25, 0}, {0, 25}};
        // sensor covariance matrix
        const Utils::Matrix<double, 2, 2> SCM = {{4, 0}, {0, 4}};
        // predicted process noise vector
        const Utils::Matrix<double, 2, 1> PPNV = {0, 0};
        // measured noise.
        const Utils::Matrix<double, 2, 1> MN = {0, 0};

        const Utils::Matrix<double, 2, 2> A = {{1, 0}, {0, 1}};
        const Utils::Matrix<double, 2, 2> B = {{1, 0}, {0, 1}};
        const Utils::Matrix<double, 2, 2> C = {{1, 0}, {0, 1}};
    };
    class ParticleDriver : public DriveStrategy
    {
    public:
        ParticleDriver() = default;
        explicit ParticleDriver(Robot* robot) : DriveStrategy(robot)
        {
        }
        ~ParticleDriver() override = default;

        void start() override;
        virtual void drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                           Base::Queue<std::shared_ptr<AbstractPercept>>& queue) override;

        std::string asString() const override;
        std::string asDebugString() const override;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_DEFAULTDRIVER_H
