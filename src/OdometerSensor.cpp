//
// Created by luke on 05-10-22.
//

#include "OdometerSensor.h"
#include "Robot.hpp"

namespace Model
{
    OdometerSensor::OdometerSensor() : NoisySensor(0, 1), previousPosition(0, 0)
    {
    }
    OdometerSensor::OdometerSensor(Robot* robot)
        : AbstractSensor(robot), NoisySensor(0, 1), previousPosition(robot->getPosition())
    {
    }
    std::shared_ptr<AbstractStimulus> OdometerSensor::getStimulus() const
    {
        Point robotPosition = dynamic_cast<Robot*>(agent)->getPosition();

        double distanceX = previousPosition.x - robotPosition.x;
        double distanceY = previousPosition.y - robotPosition.y;

        double distance = (std::sqrt(std::pow(distanceX, 2) + std::pow(distanceY, 2)));

        return std::shared_ptr<AbstractStimulus>(new OdometerStimulus(previousPosition, robotPosition, distance));
    }
    std::shared_ptr<AbstractPercept> OdometerSensor::getPerceptFor(std::shared_ptr<AbstractStimulus> aStimulus)
    {
        auto* stimulus = dynamic_cast<OdometerStimulus*>(aStimulus.get());

        previousPosition = stimulus->currentPosition;
        stimulus->distanceTravelled = noisify(stimulus->distanceTravelled) * stimulus->distanceTravelled / 10;

        return std::shared_ptr<AbstractPercept>(new OdometerPercept(*stimulus));
    }
    std::string OdometerSensor::asString() const
    {
        return "OdometerSensor";
    }
    std::string OdometerSensor::asDebugString() const
    {
        return asString();
    }
}// namespace Model