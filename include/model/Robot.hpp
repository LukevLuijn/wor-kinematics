#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include "Config.hpp"

#include "AStar.hpp"
#include "AbstractAgent.hpp"
#include "AbstractFilter.h"
#include "BoundedVector.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "Observer.hpp"
#include "Point.hpp"
#include "Region.hpp"
#include "Size.hpp"
#include "SteeringActuator.hpp"

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

    class Path;
    typedef std::shared_ptr<Path> PathPtr;

    class Robot : public AbstractAgent, public Messaging::MessageHandler, public Base::Observer
    {
    public:
        /**
         *
         */
        Robot();
        /**
         *
         */
        explicit Robot(const std::string& aName);
        /**
         *
         */
        Robot(const std::string& aName, const Point& aPosition);
        /**
         *
         */
        ~Robot() override;
        /**
         *
         */
        virtual void drive();
        /**
         *
         */
        void resetSensor(const std::string& sensorName);
        /**
         *
         */
        bool intersects(const Region& aRegion) const;
        /**
         *
         */
        bool arrived(GoalPtr aGoal);
        /**
         *
         */
        bool collision();
        /**
         *
         */
        bool outOfBounds(uint32_t pathPoint);
        /**
         *
         */
        void startActing() override;
        /**
         *
         */
        void stopActing() override;
        /**
         *
         */
        virtual void startDriving();
        /**
         *
         */
        virtual void stopDriving();
        /**
         *
         */
        std::string asString() const override;
        /**
         *
         */
        std::string asDebugString() const override;
        /**
         *
         */
        void setName(const std::string& aName, bool aNotifyObservers = true);
        /**
         *
         */
        void setSize(const Size& aSize, bool aNotifyObservers = true);
        /**
         *
         */
        void setPosition(const Point& aPosition, bool aNotifyObservers = true);
        /**
         *
         */
        void setFront(const BoundedVector& aVector, bool aNotifyObservers = true);
        /**
         *
         */
        void setSpeed(float aNewSpeed, bool aNotifyObservers = true);
        /**
         *
         */
        void setActiveFilters(const std::vector<Filters_e>& activeFilters);
        /**
         *
         */
        void addFilter(Filters_e newFilter);
        /**
         *
         */
        void removeFilter(Filters_e newFilter);
        /**
         *
         */
        void addPathPointer(const PathPtr& aPath);
        /**
         *
         */
        Region getRegion() const;
        /**
         *
         */
        Point getFrontLeft() const;
        /**
         *
         */
        Point getFrontRight() const;
        /**
         *
         */
        Point getBackLeft() const;
        /**
         *
         */
        Point getBackRight() const;
        /**
         *
         */
        void startCommunicating();
        /**
         *
         */
        void stopCommunicating();
        /**
         *
         */
        void handleNotification() override;
        /**
         *
         */
        void handleRequest(Messaging::Message& aMessage) override;
        /**
         *
         */
        void handleResponse(const Messaging::Message& aMessage) override;
        /**
         *
         */
        std::string getName() const
        {
            return name;
        }
        /**
         *
         */
        Point getPosition() const
        {
            return position;
        }
        /**
         *
         */
        bool isActing() const
        {
            return acting;
        }
        /**
         *
         */
        bool isDriving() const
        {
            return driving;
        }
        /**
         *
         */
        bool isCommunicating() const
        {
            return communicating;
        }
        /**
         *
         */
        PathAlgorithm::OpenSet getOpenSet() const
        {
            return astar.getOpenSet();
        }
        /**
         *
         */
        PathAlgorithm::Path getPath() const
        {
            return path;
        }
        /**
         *
         */
        float getSpeed() const
        {
            return speed;
        }
        /**
         *
         */
        Size getSize() const
        {
            return size;
        }
        /**
         *
         */
        BoundedVector getFront() const
        {
            return front;
        }

    protected:
        /**
         *
         */
        void calculateRoute(GoalPtr aGoal);

    private:
        /**
         *
         */
        void activateSensors(bool activate);
        /**
         *
         */
        void attachSensors();
        /**
         *
         */
        void attachActuators();
        /**
         *
         */
        void activateFilterVisualization(Filters_e filter, bool activate);

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

        std::vector<AbstractFilter*> filters;
        std::shared_ptr<SteeringActuator> steeringActuator;
        std::vector<PathPtr> paths;
    };
}// namespace Model
#endif// ROBOT_HPP_
