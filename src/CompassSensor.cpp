//
// Created by luke on 04-10-22.
//

#include "CompassSensor.h"
#include "Robot.hpp"
#include "Shape2DUtils.hpp"

namespace Model
{
    CompassSensor::CompassSensor() : NoisySensor(0, 2.0 * M_PI / 180.0)
    {
    }
    CompassSensor::CompassSensor(Robot* aRobot) : AbstractSensor(aRobot), NoisySensor(0, 2.0 * M_PI / 180.0)
    {
    }
    std::shared_ptr<AbstractStimulus> CompassSensor::getStimulus() const
    {
        double orientation = Utils::Shape2DUtils::getAngle(dynamic_cast<Robot*>(agent)->getFront());
        std::shared_ptr<AbstractStimulus> stimulus (new OrientationStimulus(orientation));

        return stimulus;
    }
    std::shared_ptr<AbstractPercept>
    CompassSensor::getPerceptFor(std::shared_ptr<AbstractStimulus> aStimulus)
    {
        auto* stimulus = dynamic_cast<OrientationStimulus*>(aStimulus.get());

        stimulus->orientation = noisify(stimulus->orientation);

        return std::shared_ptr< AbstractPercept >(new OrientationPercept(*stimulus));
    }
    std::string CompassSensor::asString() const
    {
        return "Compass sensor";
    }
    std::string CompassSensor::asDebugString() const
    {
        return asString();
    }
}// namespace Model