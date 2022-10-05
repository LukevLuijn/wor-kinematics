//
// Created by luke on 04-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_COMPASSSENSOR_H
#define WOR_WORLD_KINEMATICA_COMPASSSENSOR_H

#include "Config.hpp"
#include "AbstractSensor.hpp"
#include "NoisySensor.h"

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class OrientationStimulus : public AbstractStimulus
    {
    public:
        explicit OrientationStimulus(double anOrientation) : orientation(anOrientation){};
        double orientation;
    };

    class OrientationPercept : public AbstractPercept
    {
    public:
        explicit OrientationPercept(double anOrientation) : orientation(anOrientation) {};
        explicit OrientationPercept(const OrientationStimulus& aStimulus) : orientation(aStimulus.orientation) {}
        double orientation;
    };

    class CompassSensor : public AbstractSensor, NoisySensor
    {
    public:
        CompassSensor();
        explicit CompassSensor(Robot* aRobot);

        ~CompassSensor() override = default;

        std::shared_ptr<AbstractStimulus> getStimulus () const override;
        std::shared_ptr<AbstractPercept> getPerceptFor (std::shared_ptr< AbstractStimulus > aStimulus) override;

        std::string asString() const override;
        std::string asDebugString() const override;
    };
}

#endif//WOR_WORLD_KINEMATICA_COMPASSSENSOR_H
