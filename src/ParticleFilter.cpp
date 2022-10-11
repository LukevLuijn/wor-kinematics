//
// Created by luke on 07-10-22.
//

#include "ParticleFilter.h"

namespace Model
{
    ParticleFilter::ParticleFilter() = default;
    ParticleFilter::ParticleFilter(const Point& aInitialPosition)
    {
        // TODO

        UNUSEDCAST(aInitialPosition);
    }
    Point ParticleFilter::getMeasuredPosition(const Point& position, std::vector<AbstractSensorPtr>& sensors)
    {
        // TODO

        UNUSEDCAST(position);
        UNUSEDCAST(sensors);

        return Point(0,0);
    }
    void ParticleFilter::iterate(Point& perceivedPosition, const Point& targetPosition, const Point& measuredPosition)
    {
        /*
         * move
         * update
         *
         * pick best
         *
         * resample
         */


        // TODO

        UNUSEDCAST(perceivedPosition);
        UNUSEDCAST(targetPosition);
        UNUSEDCAST(measuredPosition);
    }
    std::string ParticleFilter::asString() const
    {
        return "ParticleFilter";
    }
}