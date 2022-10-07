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

//    class SteeringCommand : public AbstractCommand
//    {
//
//    };
//
//    class AbsoluteMovementCommand : public AbstractCommand
//    {
//    public:
//        explicit AbsoluteMovementCommand(Point aAbsolutePositionRequest)
//            : absolutePositionRequest(aAbsolutePositionRequest){}
//
//        std::string asString() override { return "AbsoluteMovementCommand";}
//
//        Point absolutePositionRequest;
//    };
    class RelativeMovementCommand : public AbstractCommand
    {
    public:
        explicit RelativeMovementCommand(Point aRelativePositionRequest)
            : relativePositionRequest(aRelativePositionRequest){}

        std::string asString() override { return "RelativeMovementCommand";}

        Point relativePositionRequest;
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
