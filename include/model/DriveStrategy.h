//
// Created by luke on 06-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H
#define WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H

#include "AStar.hpp"
#include "AbstractActuator.hpp"
#include "AbstractAgent.hpp"
#include "Goal.hpp"

namespace Model
{
    class Robot;
    typedef std::shared_ptr<Robot> RobotPtr;

    class CompassPercept;
    typedef std::shared_ptr<CompassPercept> CompassPerceptPtr;
    typedef std::vector<CompassPerceptPtr> CompassPercepts;

    class OdometerPercept;
    typedef std::shared_ptr<OdometerPercept> OdometerPerceptPtr;
    typedef std::vector<OdometerPerceptPtr> OdometerPercepts;

    class LidarPercept;
    typedef std::shared_ptr<LidarPercept> LidarPerceptPtr;
    typedef std::vector<LidarPerceptPtr> LidarPercepts;


    class DriveStrategy : public ModelObject
    {
    public:
        DriveStrategy();
        explicit DriveStrategy(Robot* robot);

        ~DriveStrategy() override = default;

        virtual void start() = 0;
        virtual void drive(const PathAlgorithm::Path& path, uint32_t& pathPoint,
                           Base::Queue<std::shared_ptr<AbstractPercept>>& queue) = 0;

    protected:
        Robot* getRobotPtr();
        void sortPercepts(Base::Queue<std::shared_ptr<AbstractPercept>>& queue);

        template<typename T>
        T* castPercept(const std::shared_ptr<AbstractPercept>& percept)
        {
            return dynamic_cast<T*>(percept.get());
        }

    protected:
        AbstractAgent* agent;
        AbstractActuator* actuator;

        LidarPercepts lidarPercepts;
        OdometerPercepts odometerPercepts;
        CompassPercepts compassPercepts;
    };
}// namespace Model

#endif//WOR_WORLD_KINEMATICA_DRIVESTRATEGY_H
