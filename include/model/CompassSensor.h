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

    class CompassStimulus : public AbstractStimulus
    {
    public:
        explicit CompassStimulus(double anOrientation) : orientation(anOrientation){};

        std::string asString() override { return "CompassStimulus";}

        double orientation;
    };

    class CompassPercept : public AbstractPercept
    {
    public:
        explicit CompassPercept(double anOrientation) : orientation(anOrientation) {};
        explicit CompassPercept(const CompassStimulus& aStimulus) : orientation(aStimulus.orientation) {}

        std::string asString() override { return "CompassPercept";}

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
