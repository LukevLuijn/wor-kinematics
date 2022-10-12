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
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    struct LidarReading {
        double angle = 0, distance = 0;
    };

    typedef std::array<LidarReading, 180> LidarData;

    class LidarStimulus : public AbstractStimulus
    {
    public:
        explicit LidarStimulus(const LidarData& lidarData) : measurements(lidarData)
        {
        }

        std::string asString() override
        {
            return "LidarStimulus";
        }

        LidarData measurements;
    };
    class LidarPercept : public AbstractPercept
    {
    public:
        explicit LidarPercept(const LidarData& lidarData) : measurements(lidarData)
        {
        }
        explicit LidarPercept(const LidarStimulus& stimulus) : measurements(stimulus.measurements)
        {
        }

        std::string asString() override
        {
            return "LidarPercept";
        }

        LidarData measurements;
    };


    class LidarSensor : public AbstractSensor, NoisySensor
    {
    public:
        LidarSensor();
        explicit LidarSensor(Robot* robot);

        ~LidarSensor() override = default;

        AbstractStimulusPtr getStimulus() const override;
        AbstractPerceptPtr getPerceptFor(AbstractStimulusPtr aStimulus) override;

        static LidarData getReadingFromLocation(const Point& location, double orientation);

        std::string asString() const override;
        std::string asDebugString() const override;

    private:
        void readFromConfigFile();
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_LIDARSENSOR_H
