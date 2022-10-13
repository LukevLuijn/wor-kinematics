//
// Created by luke on 07-10-22.
//

#include "KalmanFilter.h"

#include "CompassSensor.h"
#include "Logger.hpp"
#include "OdometerSensor.h"

namespace Model
{
    KalmanFilter::KalmanFilter()
        : AbstractFilter(Filters_e::KALMAN_FILTER), belief(Utils::Matrix<double, 2, 1>{0.0, 0.0}),
          error(Utils::Matrix<double, 2, 2>{{100, 0}, {0, 100}})
    {
    }
    KalmanFilter::KalmanFilter(const Point& aInitialPosition)
        : AbstractFilter(Filters_e::KALMAN_FILTER),
          belief(Utils::Matrix<double, 2, 1>{static_cast<double>(aInitialPosition.x),
                                             static_cast<double>(aInitialPosition.y)}),
          error(Utils::Matrix<double, 2, 2>{{100, 0}, {0, 100}})
    {
    }
    void KalmanFilter::iterate(Point& perceivedPosition, const Point& targetPosition,
                               std::vector<AbstractSensorPtr>& sensors)
    {
        Point measuredPosition = getMeasuredPosition(perceivedPosition, sensors);

        // control vector
        Utils::Matrix<double, 2, 1> control = {static_cast<double>(targetPosition.x - perceivedPosition.x),
                                               static_cast<double>(targetPosition.y - perceivedPosition.y)};
        // measurement vector
        Utils::Matrix<double, 2, 1> measured = {static_cast<double>(measuredPosition.x),
                                                static_cast<double>(measuredPosition.y)};

        // == CONTROL UPDATE ==
        // predicted state vector
        Utils::Matrix<double, 2, 1> PSV = (A * belief) + (A /*B*/ * control);// + predicted process noise
        // predicted process covariance
        Utils::Matrix<double, 2, 2> PPC = A * error * A /*A^T*/;// + process noise covariance

        // kalman gain vector
        Utils::Matrix<double, 2, 2> KG = (PPC * A /*C*/) * (A /*C*/ * PPC * A /*C^T*/ + R).inverse();

        // == MEASUREMENT UPDATE ==
        // adjusted state vector
        belief = PSV + KG * (measured - A /*C*/ * PSV);
        // adjusted process covariance
        error = PPC * (A /*I*/ - KG * A /*C*/);

        perceivedPosition = Point(static_cast<int32_t>(belief[0][0]), static_cast<int32_t>(belief[1][0]));
    }
    std::string KalmanFilter::asString() const
    {
        return "KalmanFilter";
    }
    Point KalmanFilter::getMeasuredPosition(const Point& position, std::vector<AbstractSensorPtr>& sensors)
    {
        double distance = std::numeric_limits<double>::max();
        double orientation = std::numeric_limits<double>::max();

        for (AbstractSensorPtr& sensor : sensors)
        {
            if (sensor.get()->asString() == "CompassSensor")
            {
                auto* compass = dynamic_cast<CompassSensor*>(sensor.get());
                std::shared_ptr<AbstractStimulus> stimulus = compass->getStimulus();
                std::shared_ptr<AbstractPercept> percept = compass->getPerceptFor(stimulus);

                orientation = dynamic_cast<CompassPercept*>(percept.get())->orientation;
            }
            else if (sensor.get()->asString() == "OdometerSensor")
            {
                auto* odometer = dynamic_cast<OdometerSensor*>(sensor.get());
                std::shared_ptr<AbstractStimulus> stimulus = odometer->getStimulus();
                std::shared_ptr<AbstractPercept> percept = odometer->getPerceptFor(stimulus);

                distance = dynamic_cast<OdometerPercept*>(percept.get())->distanceTravelled;
            }
        }

        if (distance == std::numeric_limits<double>::max() || orientation == std::numeric_limits<double>::max())
        {
            LOG("something went wrong while retrieving sensor percepts");
            std::cerr << "something went wrong while retrieving sensor percepts" << std::endl;
            return Point(0, 0);
        }

        auto measuredPositionX = static_cast<int32_t>(distance * std::cos(orientation) + position.x);
        auto measuredPositionY = static_cast<int32_t>(distance * std::sin(orientation) + position.x);

        return Point(measuredPositionX, measuredPositionY);
    }
}// namespace Model
