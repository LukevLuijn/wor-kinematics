//
// Created by luke on 04-10-22.
//

#include "CompassSensor.h"
#include "Robot.hpp"
#include "Shape2DUtils.hpp"

namespace Model
{
    CompassSensor::CompassSensor() : NoisySensor(0, Utils::MathUtils::toRadians(2))
    {
    }
    CompassSensor::CompassSensor(Robot* aRobot) : AbstractSensor(aRobot), NoisySensor(0, Utils::MathUtils::toRadians(2))
    {
    }
    AbstractStimulusPtr CompassSensor::getStimulus() const
    {
        double orientation = Utils::Shape2DUtils::getAngle(dynamic_cast<Robot*>(agent)->getFront());
        AbstractStimulusPtr stimulus(new CompassStimulus(orientation));

        return stimulus;
    }
    AbstractPerceptPtr CompassSensor::getPerceptFor(AbstractStimulusPtr aStimulus)
    {
        auto* stimulus = dynamic_cast<CompassStimulus*>(aStimulus.get());

        stimulus->orientation = noisify(stimulus->orientation);

        return AbstractPerceptPtr(new CompassPercept(*stimulus));
    }
    std::string CompassSensor::asString() const
    {
        return "CompassSensor";
    }
    std::string CompassSensor::asDebugString() const
    {
        return asString();
    }
}// namespace Model