#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include "Config.hpp"

#include "AStar.hpp"
#include "AbstractAgent.hpp"
#include "BoundedVector.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "Observer.hpp"
#include "Point.hpp"
#include "Region.hpp"
#include "Size.hpp"
#include "SteeringActuator.hpp"
#include "AbstractFilter.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Messaging
{
    class Message;
    class Server;
    typedef std::shared_ptr<Server> ServerPtr;
}// namespace Messaging

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class Goal;
    typedef std::shared_ptr<Goal> GoalPtr;

    class Robot : public AbstractAgent, public Messaging::MessageHandler, public Base::Observer
    {
    public:
        Robot();
        explicit Robot(const std::string& aName);
        Robot(const std::string& aName, const Point& aPosition);
        ~Robot() override;

        virtual void drive();

        bool intersects(const Region& aRegion) const;
        bool arrived(GoalPtr aGoal);
        bool collision();
        bool outOfBounds(uint32_t pathPoint);

        virtual void startActing() override;
        virtual void stopActing() override;

        virtual void startDriving();
        virtual void stopDriving();

        std::string asString() const override;
        std::string asDebugString() const override;

        void setName(const std::string& aName, bool aNotifyObservers = true);
        void setSize(const Size& aSize, bool aNotifyObservers = true);
        void setPosition(const Point& aPosition, bool aNotifyObservers = true);
        void setFront(const BoundedVector& aVector, bool aNotifyObservers = true);
        void setSpeed(float aNewSpeed, bool aNotifyObservers = true);
        void setFilter(Filters_e newFilter);

        Region getRegion() const;
        Point getFrontLeft() const;
        Point getFrontRight() const;
        Point getBackLeft() const;
        Point getBackRight() const;

        void startCommunicating();
        void stopCommunicating();

        void handleNotification() override;
        void handleRequest(Messaging::Message& aMessage) override;
        void handleResponse(const Messaging::Message& aMessage) override;

        std::string getName() const
        {
            return name;
        }
        Point getPosition() const
        {
            return position;
        }
        bool isActing() const
        {
            return acting;
        }
        bool isDriving() const
        {
            return driving;
        }
        bool isCommunicating() const
        {
            return communicating;
        }
        PathAlgorithm::OpenSet getOpenSet() const
        {
            return astar.getOpenSet();
        }
        PathAlgorithm::Path getPath() const
        {
            return path;
        }
        float getSpeed() const
        {
            return speed;
        }
        Size getSize() const
        {
            return size;
        }
        BoundedVector getFront() const
        {
            return front;
        }

    protected:
        void calculateRoute(GoalPtr aGoal);
    private:
        void activateSensors(bool activate);

        void attachSensors();
        void attachActuators();

    private:
        std::string name;
        Size size;
        Point position;
        BoundedVector front;
        float speed;
        GoalPtr goal;
        PathAlgorithm::AStar astar;
        PathAlgorithm::Path path;
        bool acting;
        bool driving;
        bool communicating;
        std::thread robotThread;
        mutable std::recursive_mutex robotMutex;
        Messaging::ServerPtr server;

        AbstractFilter* filter;
        std::shared_ptr<SteeringActuator> steeringActuator;

    };
}// namespace Model
#endif// ROBOT_HPP_
