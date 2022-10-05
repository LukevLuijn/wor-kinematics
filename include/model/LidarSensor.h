//
// Created by luke on 05-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_LIDARSENSOR_H
#define WOR_WORLD_KINEMATICA_LIDARSENSOR_H

#include "AbstractSensor.hpp"
#include "NoisySensor.h"
#include "Point.hpp"

namespace Model
{
    struct LidarReading
    {
        double angle = 0, distance = 0;
    };

    typedef std::array<LidarReading,180> LidarData;

    class LidarStimulus : public AbstractStimulus
    {
    public:
        explicit LidarStimulus(const LidarData& lidarData) : measurements(lidarData) {}
        LidarData measurements;
    };
    class LidarPercept : public AbstractPercept
    {
    public:
        explicit LidarPercept(const LidarData& lidarData) : measurements(lidarData) {}
        explicit LidarPercept(const LidarStimulus& stimulus) : measurements(stimulus.measurements) {}
        LidarData measurements;
    };

    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class LidarSensor : public AbstractSensor, NoisySensor
    {
    public:
        LidarSensor();
        explicit LidarSensor(Robot* robot);

        ~LidarSensor() override = default;

        std::shared_ptr<AbstractStimulus> getStimulus () const override;
        std::shared_ptr<AbstractPercept> getPerceptFor (std::shared_ptr< AbstractStimulus > aStimulus) override;

        std::string asString() const override;
        std::string asDebugString() const override;
    private:
//        LidarReading TakeReading(const Point& location, double angle);
//        LidarData TakeMeasurement(const Point& location, double startAngle);
    private:

    };
}

#endif//WOR_WORLD_KINEMATICA_LIDARSENSOR_H
