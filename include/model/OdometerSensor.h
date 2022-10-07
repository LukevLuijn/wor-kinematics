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
        OdometerStimulus(Point previous, Point current, double aDistanceTravelled)
            : previousPosition(previous), currentPosition(current), distanceTravelled(aDistanceTravelled) {}

        std::string asString() override { return "OdometerStimulus";}

        Point previousPosition;
        Point currentPosition;
        double distanceTravelled;
    };
    class OdometerPercept : public AbstractPercept
    {
    public:
        OdometerPercept(Point previous, Point current, double aDistanceTravelled)
            : previousPosition(previous), currentPosition(current), distanceTravelled(aDistanceTravelled) {}

        explicit OdometerPercept(const OdometerStimulus& aStimulus)
            : previousPosition(aStimulus.previousPosition), currentPosition(aStimulus.currentPosition),
              distanceTravelled(aStimulus.distanceTravelled) {}

        std::string asString() override { return "OdometerPercept";}

        Point previousPosition;
        Point currentPosition;
        double distanceTravelled;
    };
    class OdometerSensor : public AbstractSensor, NoisySensor
    {
    public:
        OdometerSensor();
        explicit OdometerSensor(Robot* robot);

        ~OdometerSensor() override = default;

        std::shared_ptr<AbstractStimulus> getStimulus() const override;
        std::shared_ptr<AbstractPercept> getPerceptFor(std::shared_ptr<AbstractStimulus> aStimulus) override;

        std::string asString() const override;
        std::string asDebugString() const override;

    private:
        Point previousPosition;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_ODOMETERSENSOR_H
