#ifndef STEERINGACTUATOR_HPP_
#define STEERINGACTUATOR_HPP_

#include "Config.hpp"

#include "AbstractActuator.hpp"
#include "Point.hpp"

#include <random>

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class SteeringCommand : public AbstractCommand
    {
    public:
        explicit SteeringCommand(Point aPositionRequest) : positionRequest(aPositionRequest) {}
        Point positionRequest;
    };

    class SteeringActuator : public AbstractActuator
    {
    public:
        SteeringActuator();
        SteeringActuator(Robot* robot);
        ~SteeringActuator() override = default;

        virtual void handleCommand(AbstractCommand& anAbstractCommand) override;

        virtual std::string asString() const override;
        virtual std::string asDebugString() const override;

    private:
        std::default_random_engine generator;
    };
}// namespace Model
#endif// STEERINGACTUATOR_HPP_
