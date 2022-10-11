#include "Robot.hpp"

#include "Client.hpp"
#include "CommunicationService.hpp"
#include "Goal.hpp"
#include "Logger.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "RobotWorld.hpp"
#include "Server.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"
#include "WayPoint.hpp"

#include "CompassSensor.h"
#include "LaserDistanceSensor.hpp"
#include "LidarSensor.h"
#include "OdometerSensor.h"

#include "SteeringActuator.hpp"

#include "KalmanFilter.h"
#include "ParticleFilter.h"

#include <chrono>
#include <ctime>
#include <memory>
#include <sstream>
#include <thread>

namespace Model
{
    /**
	 *
	 */
    Robot::Robot()
        : name(""), size(DefaultSize), position(DefaultPosition), front(0, 0), speed(0.0), acting(false),
          driving(false), communicating(false)
    {
        attachSensors();
        attachActuators();

        setFilter(Filters_e::KALMAN_FILTER);
    }
    /**
	 *
	 */
    Robot::Robot(const std::string& aName)
        : name(aName), size(DefaultSize), position(DefaultPosition), front(0, 0), speed(0.0), acting(false),
          driving(false), communicating(false)
    {
        attachSensors();
        attachActuators();

        setFilter(Filters_e::KALMAN_FILTER);
    }
    /**
	 *
	 */
    Robot::Robot(const std::string& aName, const Point& aPosition)
        : name(aName), size(DefaultSize), position(aPosition), front(0, 0), speed(0.0), acting(false), driving(false),
          communicating(false)
    {
        attachSensors();
        attachActuators();

        setFilter(Filters_e::KALMAN_FILTER);
    }
    /**
	 *
	 */
    Robot::~Robot()
    {
        if (driving)
        {
            Robot::stopDriving();
        }
        if (acting)
        {
            Robot::stopActing();
        }
        if (communicating)
        {
            stopCommunicating();
        }
    }
    void Robot::drive()
    {
        try
        {
            if (std::fabs(speed - 0.0) <= std::numeric_limits<float>::epsilon())
            {
                setSpeed(10.0, false);// @suppress("Avoid magic numbers")
            }

            uint32_t pathPoint = 0;
            Point previousPosition = position;

            while (!outOfBounds(pathPoint))
            {
                if (arrived(goal))
                {
                    LOG("robot arrived at goal position");
                    notifyObservers();
                    break;
                }
                else if (collision())
                {
                    LOG("robot collided with a world object");
                    notifyObservers();
                    break;
                }
                else
                {
                    position = previousPosition;

                    const PathAlgorithm::Vertex& vertex = path[pathPoint += static_cast<unsigned int>(getSpeed())];
                    const Point target = vertex.asPoint();

                    RelativeMovementCommand command(target);
                    steeringActuator->handleCommand(command);

                    const Point actualTarget = command.relativePositionRequest;
                    front = BoundedVector(actualTarget, position);

                    notifyObservers();

                    filter->iterate(previousPosition, actualTarget, filter->getMeasuredPosition(position, sensors));

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

                if (!driving)
                {
                    return;
                }
            }
            driving = false;
        }
        catch (std::exception& e)
        {
            LOG("exception caught", e.what());
            std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
        }
        catch (...)
        {
            LOG("unknown exception caught");
            std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
        }
    }
    /**
	 *
	 */
    bool Robot::intersects(const Region& aRegion) const
    {
        Region region = getRegion();
        region.Intersect(aRegion);
        return !region.IsEmpty();
    }
    /**
	 *
	 */
    bool Robot::arrived(GoalPtr aGoal)
    {
        if (aGoal && intersects(aGoal->getRegion()))
        {
            return true;
        }
        return false;
    }
    /**
	 *
	 */
    bool Robot::collision()
    {
        Point frontLeft = getFrontLeft();
        Point frontRight = getFrontRight();
        Point backLeft = getBackLeft();
        Point backRight = getBackRight();

        const std::vector<WallPtr>& walls = RobotWorld::getRobotWorld().getWalls();
        for (WallPtr wall : walls)
        {
            if (Utils::Shape2DUtils::intersect(frontLeft, frontRight, wall->getPoint1(), wall->getPoint2()) ||
                Utils::Shape2DUtils::intersect(frontLeft, backLeft, wall->getPoint1(), wall->getPoint2()) ||
                Utils::Shape2DUtils::intersect(frontRight, backRight, wall->getPoint1(), wall->getPoint2()))
            // cppcheck-suppress useStlAlgorithm
            {
                return true;
            }
        }
        const std::vector<RobotPtr>& robots = RobotWorld::getRobotWorld().getRobots();
        for (RobotPtr robot : robots)
        {
            if (getObjectId() == robot->getObjectId())
            {
                continue;
            }
            if (intersects(robot->getRegion()))
            {
                return true;
            }
        }
        return false;
    }
    /**
     *
     */
    bool Robot::outOfBounds(uint32_t pathPoint)
    {
        if ((position.x <= 0) || (position.x >= 1024) || (position.y <= 0) || (position.y >= 1024))
        {
            LOG("Robot out of bounds");
        }
        if (pathPoint > path.size())
        {
            LOG("Path point of of bounds");
        }


        return (position.x <= 0) || (position.x >= 1024) || (position.y <= 0) || (position.y >= 1024) ||
               (pathPoint > path.size());
    }
    /**
     *
     */
    void Robot::activateSensors(bool activate)
    {
        for (const std::shared_ptr<AbstractSensor>& sensor : sensors)
        {
            (activate) ? sensor->setOn() : sensor->setOff();
        }
    }
    /**
     *
     */
    void Robot::startActing()
    {
        acting = true;
        std::thread newRobotThread([this] {
            startDriving();
        });
        robotThread.swap(newRobotThread);
    }
    /**
	 *
	 */
    void Robot::stopActing()
    {
        acting = false;
        driving = false;
        robotThread.join();
    }
    /**
     *
     */
    void Robot::startDriving()
    {
        driving = true;

        goal = RobotWorld::getRobotWorld().getGoal("Goal");

        calculateRoute(goal);
        drive();
    }
    /**
	 *
	 */
    void Robot::stopDriving()
    {
        driving = false;
    }
    /**
     *
     */
    std::string Robot::asString() const
    {
        std::ostringstream os;

        os << "Robot " << name << " at (" << position.x << "," << position.y << ")";

        return os.str();
    }
    /**
	 *
	 */
    std::string Robot::asDebugString() const
    {
        std::ostringstream os;

        os << "Robot:\n";
        os << AbstractAgent::asDebugString();
        os << "Robot " << name << " at (" << position.x << "," << position.y << ")\n";

        return os.str();
    }
    /**
     *
     */
    void Robot::setName(const std::string& aName, bool aNotifyObservers /*= true*/)
    {
        name = aName;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void Robot::setSize(const Size& aSize, bool aNotifyObservers /*= true*/)
    {
        size = aSize;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void Robot::setPosition(const Point& aPosition, bool aNotifyObservers /*= true*/)
    {
        position = aPosition;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void Robot::setFront(const BoundedVector& aVector, bool aNotifyObservers /*= true*/)
    {
        front = aVector;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
	 * TODO odometer needs speed parameter for error.
	 */
    void Robot::setSpeed(float aNewSpeed, bool aNotifyObservers /*= true*/)
    {
        speed = aNewSpeed;
        if (aNotifyObservers == true)
        {
            notifyObservers();
        }
    }
    /**
     *
     */
    void Robot::setFilter(Filters_e newFilter)
    {
        switch (newFilter)
        {
            case Filters_e::KALMAN_FILTER:
                filter = new KalmanFilter(getPosition());
                break;
            case Filters_e::PARTICLE_FILTER:
                filter = new ParticleFilter(getPosition());
                break;
            default:
                LOG("undefined filter requested", static_cast<uint64_t>(newFilter));
                std::cerr << "undefined filter requested" << std::endl;
                return;
        }
        LOG("New filter set", filter->asString());
    }
    /**
     *
     */
    Region Robot::getRegion() const
    {
        Point translatedPoints[] = {getFrontRight(), getFrontLeft(), getBackLeft(), getBackRight()};
        return Region(4, translatedPoints);// @suppress("Avoid magic numbers")
    }
    /**
     *
     */
    Point Robot::getFrontLeft() const
    {
        // x and y are pointing to top left now
        int x = position.x - (size.x / 2);
        int y = position.y - (size.y / 2);

        Point originalFrontLeft(x, y);
        double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

        Point frontLeft(static_cast<int>((originalFrontLeft.x - position.x) * std::cos(angle) -
                                         (originalFrontLeft.y - position.y) * std::sin(angle) + position.x),
                        static_cast<int>((originalFrontLeft.y - position.y) * std::cos(angle) +
                                         (originalFrontLeft.x - position.x) * std::sin(angle) + position.y));

        return frontLeft;
    }
    /**
	 *
	 */
    Point Robot::getFrontRight() const
    {
        // x and y are pointing to top left now
        int x = position.x - (size.x / 2);
        int y = position.y - (size.y / 2);

        Point originalFrontRight(x + size.x, y);
        double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

        Point frontRight(static_cast<int>((originalFrontRight.x - position.x) * std::cos(angle) -
                                          (originalFrontRight.y - position.y) * std::sin(angle) + position.x),
                         static_cast<int>((originalFrontRight.y - position.y) * std::cos(angle) +
                                          (originalFrontRight.x - position.x) * std::sin(angle) + position.y));

        return frontRight;
    }
    /**
	 *
	 */
    Point Robot::getBackLeft() const
    {
        // x and y are pointing to top left now
        int x = position.x - (size.x / 2);
        int y = position.y - (size.y / 2);

        Point originalBackLeft(x, y + size.y);

        double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

        Point backLeft(static_cast<int>((originalBackLeft.x - position.x) * std::cos(angle) -
                                        (originalBackLeft.y - position.y) * std::sin(angle) + position.x),
                       static_cast<int>((originalBackLeft.y - position.y) * std::cos(angle) +
                                        (originalBackLeft.x - position.x) * std::sin(angle) + position.y));

        return backLeft;
    }
    /**
	 *
	 */
    Point Robot::getBackRight() const
    {
        // x and y are pointing to top left now
        int x = position.x - (size.x / 2);
        int y = position.y - (size.y / 2);

        Point originalBackRight(x + size.x, y + size.y);

        double angle = Utils::Shape2DUtils::getAngle(front) + 0.5 * Utils::PI;

        Point backRight(static_cast<int>((originalBackRight.x - position.x) * std::cos(angle) -
                                         (originalBackRight.y - position.y) * std::sin(angle) + position.x),
                        static_cast<int>((originalBackRight.y - position.y) * std::cos(angle) +
                                         (originalBackRight.x - position.x) * std::sin(angle) + position.y));

        return backRight;
    }
    /**
     *
     */
    void Robot::startCommunicating()
    {
        if (!communicating)
        {
            communicating = true;

            std::string localPort = "12345";
            if (Application::MainApplication::isArgGiven("-local_port"))
            {
                localPort = Application::MainApplication::getArg("-local_port").value;
            }

            if (Messaging::CommunicationService::getCommunicationService().isStopped())
            {
                TRACE_DEVELOP("Restarting the Communication service");
                Messaging::CommunicationService::getCommunicationService().restart();
            }

            server = std::make_shared<Messaging::Server>(static_cast<unsigned short>(std::stoi(localPort)),
                                                         toPtr<Robot>());
            Messaging::CommunicationService::getCommunicationService().registerServer(server);
        }
    }
    /**
	 *
	 */
    void Robot::stopCommunicating()
    {
        if (communicating)
        {
            communicating = false;

            std::string localPort = "12345";
            if (Application::MainApplication::isArgGiven("-local_port"))
            {
                localPort = Application::MainApplication::getArg("-local_port").value;
            }

            Messaging::Client c1ient("localhost", static_cast<unsigned short>(std::stoi(localPort)), toPtr<Robot>());
            Messaging::Message message(Messaging::StopCommunicatingRequest, "stop");
            c1ient.dispatchMessage(message);
        }
    }
    /**
	 *
	 */
    void Robot::handleNotification()
    {
        //	std::unique_lock<std::recursive_mutex> lock(robotMutex);

        static int update = 0;
        if ((++update % 200) == 0)// @suppress("Avoid magic numbers")
        {
            notifyObservers();
        }
    }
    /**
	 *
	 */
    void Robot::handleRequest(Messaging::Message& aMessage)
    {
        FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

        switch (aMessage.getMessageType())
        {
            case Messaging::StopCommunicatingRequest:
            {
                aMessage.setMessageType(Messaging::StopCommunicatingResponse);
                aMessage.setBody("StopCommunicatingResponse");
                // Handle the request. In the limited context of this works. I am not sure
                // whether this works OK in a real application because the handling is time sensitive,
                // i.e. 2 async timers are involved:
                // see CommunicationService::stopServer and Server::stopHandlingRequests
                Messaging::CommunicationService::getCommunicationService().stopServer(
                        12345,
                        true);// @suppress("Avoid magic numbers")

                break;
            }
            case Messaging::EchoRequest:
            {
                aMessage.setMessageType(Messaging::EchoResponse);
                aMessage.setBody("Messaging::EchoResponse: " + aMessage.asString());
                break;
            }
            default:
            {
                TRACE_DEVELOP(__PRETTY_FUNCTION__ + std::string(": default not implemented"));
                break;
            }
        }
    }
    /**
	 *
	 */
    void Robot::handleResponse(const Messaging::Message& aMessage)
    {
        FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

        switch (aMessage.getMessageType())
        {
            case Messaging::StopCommunicatingResponse:
            {
                //Messaging::CommunicationService::getCommunicationService().stop();
                break;
            }
            case Messaging::EchoResponse:
            {
                break;
            }
            default:
            {
                TRACE_DEVELOP(__PRETTY_FUNCTION__ + std::string(": default not implemented, ") + aMessage.asString());
                break;
            }
        }
    }
    /**
     *
     */
    void Robot::calculateRoute(GoalPtr aGoal)
    {
        path.clear();
        if (aGoal)
        {
            front = BoundedVector(aGoal->getPosition(), position);

            auto start = std::chrono::high_resolution_clock::now();

            path = astar.search(position, aGoal->getPosition(), size + Size(size.x / 2, size.y / 2));

            auto stop = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            LOG("path found in: " + std::to_string(time.count()) + " milliseconds");
        }
    }
    /**
     *
     */
    void Robot::attachSensors()
    {
        std::array<std::shared_ptr<AbstractSensor>, 3> robotSensors{
                std::shared_ptr<AbstractSensor>(new CompassSensor(this)),
                std::shared_ptr<AbstractSensor>(new LidarSensor(this)),
                std::shared_ptr<AbstractSensor>(new OdometerSensor(this)),
        };

        for (const std::shared_ptr<AbstractSensor>& sensor : robotSensors)
        {
            attachSensor(sensor);
        }
    }
    /**
         *
         */
    void Robot::attachActuators()
    {
        steeringActuator = std::make_shared<SteeringActuator>(this);// need local instance

        std::array<std::shared_ptr<AbstractActuator>, 1> robotActuators{steeringActuator};

        for (const std::shared_ptr<AbstractActuator>& actuator : robotActuators)
        {
            attachActuator(actuator);
        }
    }
}// namespace Model
