//
// Created by luke on 05-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_ODOMETERSENSOR_H
#define WOR_WORLD_KINEMATICA_ODOMETERSENSOR_H

#include "AbstractSensor.hpp"
#include "NoisySensor.h"
#include "Point.hpp"

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class OdometerStimulus : public AbstractStimulus
    {
    public:
        OdometerStimulus(Point current, double aDistanceTravelled)
            : currentPosition(current), distanceTravelled(aDistanceTravelled)
        {
        }
        std::string asString() override
        {
            return "OdometerStimulus";
        }
        Point currentPosition;
        double distanceTravelled;
    };
    class OdometerPercept : public AbstractPercept
    {
    public:
        explicit OdometerPercept(double aDistanceTravelled) : distanceTravelled(aDistanceTravelled)
        {
        }
        explicit OdometerPercept(const OdometerStimulus& aStimulus) : distanceTravelled(aStimulus.distanceTravelled)
        {
        }
        std::string asString() override
        {
            return "OdometerPercept";
        }
        double distanceTravelled;
    };
    class OdometerSensor : public AbstractSensor, NoisySensor
    {
    public:
        /**
         *
         */
        OdometerSensor();
        /**
         *
         */
        explicit OdometerSensor(Robot* robot);
        /**
         *
         */
        ~OdometerSensor() override = default;
        /**
         *
         */
        AbstractStimulusPtr getStimulus() const override;
        /**
         *
         */
        AbstractPerceptPtr getPerceptFor(AbstractStimulusPtr aStimulus) override;
        /**
         *
         */
        std::string asString() const override;
        /**
         *
         */
        std::string asDebugString() const override;
        /**
         * Reset sensor standard deviation to configured value.
         */
        void recalibrate() override;

    private:
        Point previousPosition;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_ODOMETERSENSOR_H
